#include "mainwindow.h"
#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QDebug>
#include <QMessageBox>

constexpr char dataBaseName[] = "MyDataBase";
constexpr char dataBaseFileName[] = "database.sdb";

void queryDebug(QSqlQuery * query)
{
    if (!query->exec())
        qDebug() << query->lastQuery() << " failed; " << query->lastError().text();
}

void queryDebug(QSqlQuery * query, const QString& str)
{
    if (!query->exec(str))
        qDebug() << str << " failed; " << query->lastError().text();
}

bool initDb(QSqlDatabase * db)
{
    QSqlQuery query("", *db);

    if (!db->tables().contains("problems"))
    {
        queryDebug(&query, "create table problems (id integer primary key autoincrement, name text, task text, solution text)");
        queryDebug(&query, "insert into problems values(NULL, 'Задача про колесо', 'Колесо катится без проскальзывания. Найти его скорость.', 'Скорость равна 2 м/с.')");
        queryDebug(&query, "insert into problems values(NULL, 'Задача про уробороса', 'Уроборос плотно пообедал. Кого он съел?', 'Свой хвост, очевидно.')");
        queryDebug(&query, "insert into problems values(NULL, 'Сопротивление бесполезно!', 'На рисунке изображена схема. Найти ее.', '404: схема не найдена. (Это шутка, а не реальный ответ БД)')");
        queryDebug(&query, "insert into problems values(NULL, 'Вечный вопрос', 'Любит ли меня жена?', 'Чья?')");
    }

    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    db = new QSqlDatabase{QSqlDatabase::addDatabase("QSQLITE", dataBaseName)};
    db->setDatabaseName(dataBaseFileName);
    if (!db->open())
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                        "This example needs SQLite support. Please read "
                        "the Qt SQL driver documentation for information how "
                        "to build it.\n\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
    if (db->tables().empty())
        initDb(db);

    QWidget * widgetCentral = new QWidget;
    setCentralWidget(widgetCentral);

    problemWidget = new ProblemWidget;
    listWidgetProblems = new QListWidget;
    listWidgetProblems->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout * layoutMain = new QHBoxLayout;
    layoutMain->addWidget(listWidgetProblems);
    layoutMain->addWidget(problemWidget);

    widgetCentral->setLayout(layoutMain);

//    statusBar();  // needed to create status bar
//    statusBar()->showMessage("message here");
//    setWindowTitle("Composer 0.1");
//    setMinimumSize(300, 300);
//    resize(480, 320);

    QAction * actionQuit = new QAction(tr("&Quit"), this);
    actionQuit->setShortcuts(QKeySequence::Quit);  // not for Windows, alas
//    actionQuit->setStatusTip(tr("Close application"));
    connect(actionQuit, &QAction::triggered, this, &MainWindow::close);
    QMenu * menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionQuit);

    listWidgetProblems->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listWidgetProblems, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(listWidgetProblems->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::problemSelectionChanged);
    connect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);

    reloadData();
}

MainWindow::~MainWindow()
{
    delete db;
    QSqlDatabase::removeDatabase(dataBaseName);
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = listWidgetProblems->indexAt(pos);

    QMenu contextMenu;
    QAction * action;
    action = contextMenu.addAction("New", this, &MainWindow::newProblem);
    action->setToolTip("Create new problem");
    action = contextMenu.addAction("Rename", this, &MainWindow::renameProblem);
    if (index.isValid())
        action->setToolTip("Rename the problem");
    action->setEnabled(index.isValid());
    action = contextMenu.addAction("Delete", this, &MainWindow::deleteProblem);
    if (index.isValid())
        action->setToolTip("Delete the problem");
    action->setEnabled(index.isValid());

    contextMenu.setToolTipsVisible(true);
    contextMenu.exec(listWidgetProblems->mapToGlobal(pos));
}

void MainWindow::newProblem()
{
    disconnect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // needed not to trigger updateProblemNames too many times

    old_problem_name_ = "";  // MUST nullify old name
    QString new_name = tr("New task-%1");  // template name for new problems
    int count = 1;
    while (!listWidgetProblems->findItems(new_name.arg(count), Qt::MatchExactly).empty())
        ++count;

    listWidgetProblems->addItem(new_name.arg(count));

    connect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // restore the connection
    // the given disconnect-connect pair doesnot allow to trigger updateProblemNames

    QListWidgetItem * item = listWidgetProblems->item(listWidgetProblems->model()->rowCount()-1);
    item->setFlags(item->flags() | Qt::ItemIsEditable);  // trigger updateProblemNames one time
    listWidgetProblems->editItem(item);
}

void MainWindow::renameProblem()
{
    QList<QListWidgetItem*> selected = listWidgetProblems->selectedItems();
    if (selected.empty())  // cannot happen
        return;

    old_problem_name_ = selected[0]->data(Qt::DisplayRole).toString();  // MUST save old name to pass it to 'where' case of sql update command
    listWidgetProblems->editItem(selected[0]);
}

void MainWindow::deleteProblem()
{
    QList<QListWidgetItem*> selected = listWidgetProblems->selectedItems();
    if (selected.empty())  // cannot happen
        return;

    QString problem_name = selected[0]->data(Qt::DisplayRole).toString();

    // ask for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Delete problem"),
                                  tr("Do you really want to delete problem \"%1\" ?").arg(problem_name),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QSqlQuery query("", *db);
        query.prepare(QString("delete from problems where name = '%1'").arg(problem_name));
        queryDebug(&query);

        // first move current item, then delete the selected one
        // otherwise problemSelectionChanged will be called twice
        listWidgetProblems->setCurrentItem(nullptr);
        listWidgetProblems->model()->removeRow(listWidgetProblems->row(selected[0]));
    }
}

void MainWindow::problemSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    if (!deselected.indexes().empty())
    {
        QModelIndex previous = deselected.indexes()[0];
        if (previous.isValid())  // may be redundant
        {
            QSqlQuery query("", *db);
            query.prepare(QString("update problems set task = '%1', solution = '%2' where name = '%3'")
                          .arg(problemWidget->task())
                          .arg(problemWidget->solution())
                          .arg(previous.data().toString()));
            queryDebug(&query);
        }
    }

    if (!selected.indexes().empty())
    {
        QModelIndex current = selected.indexes()[0];
        if (current.isValid())  // may be redundant
        {
            QSqlQuery query("", *db);
            query.prepare(QString("select task,solution from problems where name = '%1'")
                          .arg(current.data().toString()));
            queryDebug(&query);

            if (!query.next())  // cannot happened
                problemWidget->updateProblem();
            else
            {
                QSqlRecord record = query.record();
                problemWidget->updateProblem(record.value(0).toString(), record.value(1).toString());
            }
        }
        else
            problemWidget->updateProblem();
    }
    else
        problemWidget->updateProblem();
}

void MainWindow::reloadData()
{
    disconnect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // needed not to trigger updateProblemNames too many times

    QSqlQuery query("", *db);
    queryDebug(&query, "select name from problems");

    listWidgetProblems->clear();
    while (query.next())
    {
        listWidgetProblems->addItem(query.record().value(0).toString());
        QListWidgetItem *item = listWidgetProblems->item(listWidgetProblems->model()->rowCount()-1);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

    problemWidget->updateProblem();

    connect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // restore the connection
    // the given disconnect-connect pair doesnot allow to trigger updateProblemNames
}

void MainWindow::updateProblemNames(QListWidgetItem *item)
{
    QString new_name(item->data(Qt::DisplayRole).toString());
    if (old_problem_name_ == "")  // new problem
    {
        QSqlQuery query("", *db);
        query.prepare(QString("insert into problems values(NULL, '%1', '%2', '%3')")
                      .arg(new_name)
                      .arg("")
                      .arg(""));
        queryDebug(&query);

        item->setSelected(true);
        listWidgetProblems->setCurrentItem(item);
        old_problem_name_ = new_name;
    }
    else  // rename
    {
        if (new_name == "")  // user error: empty name
        {
            QMessageBox::critical(nullptr, tr("Cannot rename problem"),
                                  tr("Unable to rename problem.\nProblem name must not be empty."),
                                  QMessageBox::Ok);
            disconnect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // needed not to trigger updateProblemNames
            item->setData(Qt::DisplayRole, old_problem_name_);  // revert renaming
            connect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // restore the connection
        }
        else if (listWidgetProblems->findItems(new_name, Qt::MatchExactly).size() > 1)  // user error: name collision
        {
            QMessageBox::critical(nullptr, tr("Cannot rename problem"),
                                  tr("Unable to rename problem.\nProblem with the name \"%1\" already exists.").arg(new_name),
                                  QMessageBox::Ok);
            disconnect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // needed not to trigger updateProblemNames
            item->setData(Qt::DisplayRole, old_problem_name_);  // revert renaming
            connect(listWidgetProblems, &QListWidget::itemChanged, this, &MainWindow::updateProblemNames);  // restore the connection
        }
        else
        {
            QSqlQuery query("", *db);
            query.prepare(QString("update problems set name = '%1' where name = '%2'")
                          .arg(new_name)
                          .arg(old_problem_name_));
            queryDebug(&query);
        }
    }
}
