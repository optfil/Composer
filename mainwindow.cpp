#include "mainwindow.h"
#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
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
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = listWidgetProblems->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Rename", this, &MainWindow::renameItem);
    myMenu.addAction("Delete", this, &MainWindow::deleteItem);

    // Show context menu at handling position
    myMenu.exec(globalPos);
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
