#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "problem.h"
#include "problemwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateProblem(int currentRow);

private:
    QList<Problem> problems;

    QListWidget * listWidgetProblems;
    ProblemWidget * problemWidget;

    void mock_create_problems();  // psst, need some problems?
};

#endif // MAINWINDOW_H
