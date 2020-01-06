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
        queryDebug(&query, "create table problems (id int primary key, name text, task text, solution text)");
        queryDebug(&query, "insert into problems values(0, 'Задача про колесо', 'Колесо катится без проскальзывания. Найти его скорость.', 'Скорость равна 2 м/с.')");
        queryDebug(&query, "insert into problems values(1, 'Задача про уробороса', 'Уроборос плотно пообедал. Кого он съел?', 'Свой хвост, очевидно.')");
        queryDebug(&query, "insert into problems values(2, 'Сопротивление бесполезно!', 'На рисунке изображена схема. Найти ее.', '404: схема не найдена. (Это шутка, а не реальный ответ БД)')");
        queryDebug(&query, "insert into problems values(3, 'Вечный вопрос', 'Любит ли меня жена?', 'Чья?')");
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

    QStringList problemNames;
    QSqlQuery query("", *db);
    queryDebug(&query, "select name from problems");
    while (query.next())
        problemNames << query.record().value(0).toString();
    problemNamesModel = new QStringListModel(problemNames);

    QWidget * widgetCentral = new QWidget;
    setCentralWidget(widgetCentral);

    problemWidget = new ProblemWidget;
    listViewProblems = new QListView;
    listViewProblems->setSelectionMode(QAbstractItemView::SingleSelection);
    listViewProblems->setModel(problemNamesModel);

    QHBoxLayout * layoutMain = new QHBoxLayout;
    layoutMain->addWidget(listViewProblems);
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

    listViewProblems->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listViewProblems, &QListView::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(listViewProblems->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::problemSelectionChanged);
}

MainWindow::~MainWindow()
{
    delete db;
    QSqlDatabase::removeDatabase(dataBaseName);
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = listViewProblems->indexAt(pos);

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
    contextMenu.exec(listViewProblems->mapToGlobal(pos));
}

void MainWindow::newProblem()
{

}

void MainWindow::renameProblem()
{/*
    QList<QListWidgetItem*> selectedItems = listWidgetProblems->selectedItems();
    if (!selectedItems.empty())
    {
        QListWidgetItem *item = selectedItems.first();
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        listWidgetProblems->editItem(item);
    }*/
}

void MainWindow::deleteProblem()
{/*
    QList<QListWidgetItem*> selectedItems = listWidgetProblems->selectedItems();
    if (!selectedItems.empty())
    {
        QListWidgetItem * item = listWidgetProblems->takeItem(listWidgetProblems->currentRow());
        delete item;
    }*/
}

void MainWindow::problemSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
    if (previous.isValid())
    {
        QSqlQuery query("", *db);
        query.prepare(QString("update problems set task = '%1', solution = '%2' where name = '%3'")
                      .arg(problemWidget->task())
                      .arg(problemWidget->solution())
                      .arg(previous.data().toString()));
        queryDebug(&query);
    }

    if (current.isValid())
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

    /*
        QListWidgetItem * item = selectedItems.first();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    */
}
