#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QtSql>

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
    void problemSelectionChanged(const QModelIndex& current, const QModelIndex& previous);
    void reloadData();

private:
    QListView *listViewProblems;
    ProblemWidget *problemWidget;

    QSqlDatabase *db;
};

#endif // MAINWINDOW_H
