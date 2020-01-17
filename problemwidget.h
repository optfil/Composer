#ifndef PROBLEMWIDGET_H
#define PROBLEMWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QListWidget>

class ProblemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProblemWidget(QWidget *parent = nullptr);

signals:

public slots:
    void updateProblem(const QString& task, const QString& solution, const QList<QString>& tags);
    void updateProblem();

private slots:
    void setReadOnly(bool allowed);

public:
    QString task() const;
    QString solution() const;
    QList<QString> tags() const;

private:
    QCheckBox *checkBoxEditable;
    QTextEdit *textEditTask;
    QTextEdit *textEditSolution;
    QListWidget *listWidgetTags;

    void updateTask(const QString& task);
    void updateSolution(const QString& solution);
    void updateTags(const QList<QString>& tags);
};

#endif // PROBLEMWIDGET_H
