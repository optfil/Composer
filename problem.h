#ifndef PROBLEM_H
#define PROBLEM_H

#include <QString>

class Problem
{
public:
    Problem(const QString & name)
        : name_{name}, task_{}, solution_{}
    {}

    void setTask(const QString & task) {task_ = task;}
    void setSolution(const QString & solution) {solution_ = solution;}
    QString name() const {return name_;}
    QString task() const {return task_;}
    QString solution() const {return solution_;}

private:
    QString name_;
    QString task_;
    QString solution_;
};

#endif // PROBLEM_H
