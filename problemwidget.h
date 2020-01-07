#ifndef PROBLEMWIDGET_H
#define PROBLEMWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QCheckBox>

class ProblemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProblemWidget(QWidget *parent = nullptr);

signals:

public slots:
    void updateTask(const QString& task);
    void updateSolution(const QString& solution);
    void updateProblem(const QString& task, const QString& solution);
    void updateProblem();

private slots:
    void setReadOnly(bool allowed);

public:
    QString task() const;
    QString solution() const;

private:
    QCheckBox *checkBoxEditable;
    QTextEdit *textEditTask;
    QTextEdit *textEditSolution;
};

#endif // PROBLEMWIDGET_H
