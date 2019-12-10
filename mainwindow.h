#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "problem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Problem problem;
};
#endif // MAINWINDOW_H
