#ifndef PROBLEMWIDGET_H
#define PROBLEMWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include "problem.h"

class ProblemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProblemWidget(QWidget *parent = nullptr);

signals:

public slots:
    void updateTask(const Problem& problem);
    void updateSolution(const Problem& problem);
    void updateProblem(const Problem& problem);
    void updateProblem();

private:
    QTextEdit *textEditTask;
    QTextEdit *textEditSolution;
};

#endif // PROBLEMWIDGET_H
