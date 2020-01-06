#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QtSql>

#include "problem.h"
#include "problemwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showContextMenu(const QPoint & pos);
    void newProblem();
    void renameProblem();
    void deleteProblem();
    void problemSelectionChanged();

private:
    //QList<Problem> problems;

    QListView *listViewProblems;
    ProblemWidget *problemWidget;

    QStringListModel *problemNamesModel;
    QSqlDatabase *db;
};

#endif // MAINWINDOW_H
