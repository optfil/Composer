#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QtSql/QSqlError>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void request();

private:
    QTextEdit *textEditTables;
    QLineEdit *lineEditQuery;
    QPushButton * pushButtonRequest;
    QTextEdit *textEditResponse;

    void updateTables();
    void processError(const QSqlError& error);
};

#endif // MAINWINDOW_H
