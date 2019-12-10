#include "mainwindow.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    problem.setTask("Условие");
    problem.setSolution("Решение");

    problemWidget = new ProblemWidget;

//    QHBoxLayout * layoutMain = new QHBoxLayout;
//    layoutMain->addWidget(problemWidget);

    setCentralWidget(problemWidget);

    problemWidget->updateProblem(problem);
}

MainWindow::~MainWindow()
{
}

