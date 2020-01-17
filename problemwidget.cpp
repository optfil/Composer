#include "problemwidget.h"
#include <QLayout>

ProblemWidget::ProblemWidget(QWidget *parent) : QWidget(parent)
{
    checkBoxEditable = new QCheckBox(tr("Allow problem edit"));
    textEditTask = new QTextEdit;
    textEditSolution = new QTextEdit;
    listViewTags = new QListView;

    textEditTask->setReadOnly(true);
    textEditSolution->setReadOnly(true);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addWidget(checkBoxEditable);
    layoutMain->addWidget(textEditTask);
    layoutMain->addWidget(textEditSolution);
    layoutMain->addWidget(listViewTags);

    connect(checkBoxEditable, &QCheckBox::toggled, this, &ProblemWidget::setReadOnly);

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

void ProblemWidget::setReadOnly(bool allowed)
{
    textEditTask->setReadOnly(!allowed);
    textEditSolution->setReadOnly(!allowed);
}

QString ProblemWidget::task() const
{
    return textEditTask->toPlainText();
}

QString ProblemWidget::solution() const
{
    return textEditSolution->toPlainText();
}
