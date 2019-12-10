#include "mainwindow.h"
#include <QLayout>

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

    problemWidget->updateProblem(problem);
}

MainWindow::~MainWindow()
{
}

