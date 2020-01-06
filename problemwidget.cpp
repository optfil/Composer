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

void ProblemWidget::updateTask(const QString &task)
{
    textEditTask->setPlainText(task);
}

void ProblemWidget::updateSolution(const QString &solution)
{
    textEditSolution->setPlainText(solution);
}

void ProblemWidget::updateProblem(const QString &task, const QString &solution)
{
    updateTask(task);
    updateSolution(solution);
}

void ProblemWidget::updateProblem()
{
    textEditTask->clear();
    textEditSolution->clear();
}

QString ProblemWidget::task() const
{
    return textEditTask->toPlainText();
}

QString ProblemWidget::solution() const
{
    return textEditSolution->toPlainText();
}
