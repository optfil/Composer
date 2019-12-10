#ifndef PROBLEMWIDGET_H
#define PROBLEMWIDGET_H

#include <QWidget>
#include <QTextEdit>

class ProblemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProblemWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QTextEdit *textEditTask;
    QTextEdit *textEditSolution;
};

#endif // PROBLEMWIDGET_H
