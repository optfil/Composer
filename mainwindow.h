#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
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
    void problemSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void reloadData();
    //void updateProblemNames(const QModelIndex& index);
    void updateProblemNames(QListWidgetItem* item);

private:
    QListWidget *listWidgetProblems;
    ProblemWidget *problemWidget;

    QSqlDatabase *db;

    QString old_problem_name_;  // needed for renaming
};

#endif // MAINWINDOW_H
