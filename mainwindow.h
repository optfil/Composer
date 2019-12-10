#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "problem.h"
#include "problemwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Problem problem;

    ProblemWidget * problemWidget;
};

#endif // MAINWINDOW_H
