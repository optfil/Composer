#include "mainwindow.h"
#include <QLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget * widgetCentral = new QWidget;
    setCentralWidget(widgetCentral);

    problem.setTask("Условие");
    problem.setSolution("Решение");

    problemWidget = new ProblemWidget;

    QHBoxLayout * layoutMain = new QHBoxLayout;
    layoutMain->addWidget(problemWidget);

    widgetCentral->setLayout(layoutMain);

//    statusBar()->showMessage("message here");
//    setWindowTitle("Composer 0.1");
//    setMinimumSize(300, 300);
//    resize(480, 320);

    problemWidget->updateProblem(problem);
}

MainWindow::~MainWindow()
{
}

