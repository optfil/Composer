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

void ProblemWidget::updateTask(const Problem &problem)
{
    textEditTask->setText(problem.task());
}

void ProblemWidget::updateSolution(const Problem &problem)
{
    textEditSolution->setText(problem.solution());
}

void ProblemWidget::updateProblem(const Problem &problem)
{
    updateTask(problem);
    updateSolution(problem);
}
