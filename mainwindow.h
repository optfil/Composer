#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

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
};

#endif // MAINWINDOW_H
