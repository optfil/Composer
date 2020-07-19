#include "mainwindow.h"
#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget * centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    lineEditQuery = new QLineEdit;
    textEditResponse = new QTextEdit;

    QVBoxLayout * layoutMain = new QVBoxLayout;
    layoutMain->addWidget(lineEditQuery);
    layoutMain->addWidget(textEditResponse);

    centralWidget->setLayout(layoutMain);

//    mock_create_problems();

    statusBar();  // needed to create status bar

    QAction * actionQuit = new QAction(tr("&Quit"), this);
//    actionQuit->setShortcuts(QKeySequence::New);
    actionQuit->setStatusTip(tr("Close application"));
    connect(actionQuit, &QAction::triggered, this, &MainWindow::close);
    QMenu * menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionQuit);
}

MainWindow::~MainWindow()
{
}
/*
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
*/
