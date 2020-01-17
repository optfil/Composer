#include "problemwidget.h"
#include <QLayout>

ProblemWidget::ProblemWidget(QWidget *parent) : QWidget(parent)
{
    checkBoxEditable = new QCheckBox(tr("Allow problem edit"));
    textEditTask = new QTextEdit;
    textEditSolution = new QTextEdit;
    listWidgetTags = new QListWidget;

    textEditTask->setReadOnly(true);
    textEditSolution->setReadOnly(true);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addWidget(checkBoxEditable);
    layoutMain->addWidget(textEditTask);
    layoutMain->addWidget(textEditSolution);
    layoutMain->addWidget(listWidgetTags);

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

void ProblemWidget::updateTags(const QList<QString> &tags)
{
    listWidgetTags->clear();
    for (const auto& tag: tags)
        listWidgetTags->addItem(tag);
}

void ProblemWidget::updateProblem(const QString &task, const QString &solution, const QList<QString>& tags)
{
    updateTask(task);
    updateSolution(solution);
    updateTags(tags);
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

QList<QString> ProblemWidget::tags() const
{
    return {};
}
