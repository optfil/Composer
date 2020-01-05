#include "mainwindow.h"
#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QDebug>
#include <QMessageBox>

constexpr char dataBaseName[] = "MyDataBase";
constexpr char dataBaseFileName[] = "database.sdb";

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
    : QMainWindow(parent), queryModel(nullptr)
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
    //if (db->tables().empty())
    //    initDb(db);


    mock_create_problems();

    QWidget * widgetCentral = new QWidget;
    setCentralWidget(widgetCentral);

    problemWidget = new ProblemWidget;
    listWidgetProblems = new QListWidget;
    listWidgetProblems->setSelectionMode(QAbstractItemView::SingleSelection);


    QHBoxLayout * layoutMain = new QHBoxLayout;
    layoutMain->addWidget(listWidgetProblems);
    layoutMain->addWidget(problemWidget);

    for (const auto &problem: problems){
        QListWidgetItem * item = new QListWidgetItem;
        item->setData(Qt::UserRole, QVariant::fromValue(problem));
        item->setText(problem.name());
        listWidgetProblems->addItem(item);
    }

    widgetCentral->setLayout(layoutMain);    
    statusBar();  // needed to create status bar
//    statusBar()->showMessage("message here");
//    setWindowTitle("Composer 0.1");
//    setMinimumSize(300, 300);
//    resize(480, 320);

    QAction * actionQuit = new QAction(tr("&Quit"), this);
//    actionQuit->setShortcuts(QKeySequence::New);
    actionQuit->setStatusTip(tr("Close application"));
    connect(actionQuit, &QAction::triggered, this, &MainWindow::close);
    QMenu * menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionQuit);

    listWidgetProblems->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listWidgetProblems, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(listWidgetProblems, &QListWidget::itemSelectionChanged, this, &MainWindow::problemSelectionChanged);
}

MainWindow::~MainWindow()
{
    delete queryModel;
    delete db;
    QSqlDatabase::removeDatabase(dataBaseName);
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = listWidgetProblems->mapToGlobal(pos);
    QListWidgetItem * item = listWidgetProblems->itemAt(pos);

    QMenu menu;
    menu.addAction("New", this, &MainWindow::newItem);
    menu.addAction("Rename", this, &MainWindow::renameItem)->setEnabled(static_cast<bool>(item));
    menu.addAction("Delete", this, &MainWindow::deleteItem)->setEnabled(static_cast<bool>(item));

    menu.exec(globalPos);
}

void MainWindow::newItem()
{

}

void MainWindow::renameItem()
{
    QList<QListWidgetItem*> selectedItems = listWidgetProblems->selectedItems();
    if (!selectedItems.empty())
    {
        QListWidgetItem *item = selectedItems.first();
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        listWidgetProblems->editItem(item);
    }
}

void MainWindow::deleteItem()
{
    QList<QListWidgetItem*> selectedItems = listWidgetProblems->selectedItems();
    if (!selectedItems.empty())
    {
        QListWidgetItem * item = listWidgetProblems->takeItem(listWidgetProblems->currentRow());
        delete item;
    }
}

void MainWindow::problemSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = listWidgetProblems->selectedItems();
    if (!selectedItems.empty())
    {
        QListWidgetItem * item = selectedItems.first();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        const Problem & problem = item->data(Qt::UserRole).value<Problem>();
        problemWidget->updateProblem(problem);
    }
    else
        problemWidget->updateProblem();
}

void MainWindow::mock_create_problems()
{
    problems.append(Problem("Задача про колесо"));
    problems.last().setTask("Колесо катится без проскальзывания. Найти его скорость.");
    problems.last().setSolution("Скорость равна 2 м/с.");

    problems.append(Problem("Задача про уробороса"));
    problems.last().setTask("Уроборос плотно пообедал. Кого он съел?");
    problems.last().setSolution("Свой хвост, очевидно.");

    problems.append(Problem("Сопротивление бесполезно!"));
    problems.last().setTask("На рисунке изображена схема. Найти ее.");
    problems.last().setSolution("404: схема не найдена. (Это шутка, а не реальный ответ БД)");

    problems.append(Problem("Вечный вопрос"));
    problems.last().setTask("Любит ли меня жена?");
    problems.last().setSolution("Чья?");
}
