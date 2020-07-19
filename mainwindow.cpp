#include "mainwindow.h"
#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QDebug>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget * centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    textEditTables = new QTextEdit;
    textEditTables->setReadOnly(true);
    lineEditQuery = new QLineEdit;
    pushButtonRequest = new QPushButton("Request");
    textEditResponse = new QTextEdit;

    QHBoxLayout *layoutRequest = new QHBoxLayout;
    layoutRequest->addWidget(lineEditQuery);
    layoutRequest->addWidget(pushButtonRequest);

    QVBoxLayout * layoutMain = new QVBoxLayout;
    layoutMain->addWidget(textEditTables);
    layoutMain->addLayout(layoutRequest);
    layoutMain->addWidget(textEditResponse);

    centralWidget->setLayout(layoutMain);

    connect(pushButtonRequest, &QPushButton::clicked, this, &MainWindow::request);

    statusBar();  // needed to create status bar

    QAction * actionQuit = new QAction(tr("&Quit"), this);
//    actionQuit->setShortcuts(QKeySequence::New);
    actionQuit->setStatusTip(tr("Close application"));
    connect(actionQuit, &QAction::triggered, this, &MainWindow::close);
    QMenu * menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionQuit);

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(
                    this,
                    "Unable to load database",
                    "This demo needs the SQLITE driver"
                    );
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("c:\\users\\lidiy\\Composer\\tasks.db");

    if (!db.open())
        processError(db.lastError());

    updateTables();
}

MainWindow::~MainWindow()
{
}

void MainWindow::request()
{
    QSqlQuery q;
    if (!q.exec(lineEditQuery->text()))
    {
        processError(q.lastError());
        return;
    }

    textEditResponse->clear();
    while (q.next())
    {
        QSqlRecord record = q.record();
        QString str;
        for (int idx = 0; idx < record.count(); ++idx)
        {
            str.append(record.value(idx).toString());
            if (idx != record.count() - 1)
                str.append(" || ");
        }
        textEditResponse->append(str);
    }

    updateTables();
}

void MainWindow::updateTables()
{
    textEditTables->clear();
    QStringList tables = QSqlDatabase::database().tables();
    for (const auto& table: tables)
        textEditTables->append(table);
}

void MainWindow::processError(const QSqlError& error)
{
    textEditResponse->clear();
    textEditResponse->append(error.text());
}
