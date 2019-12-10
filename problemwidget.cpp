#include "problemwidget.h"
#include <QLayout>

ProblemWidget::ProblemWidget(QWidget *parent) : QWidget(parent)
{
    textEditTask = new QTextEdit;
    textEditSolution = new QTextEdit;

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addWidget(textEditTask);
    layoutMain->addWidget(textEditSolution);

    setLayout(layoutMain);
}
