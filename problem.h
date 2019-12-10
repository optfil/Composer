#ifndef PROBLEM_H
#define PROBLEM_H

#include <QString>

class Problem
{
public:
    Problem() {}

    void setTask(const QString & task) {task_ = task;}
    void setSolution(const QString & solution) {solution_ = solution;}
    QString task() const {return task_;}
    QString solution() const {return solution_;}

private:
    QString task_;
    QString solution_;
};

#endif // PROBLEM_H
