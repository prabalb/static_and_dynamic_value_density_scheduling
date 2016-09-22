/* Dynamic Value Density (DVD) Scheduling Algorithm
 * Author: PRABAL BASU (A02049867)
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Task {

  friend class Util; // Util objects need to have access to the private members of Task
  friend struct order; // for sorting tasks based on priority

  private:
    std::string id;
    int arrivalTime;
    int wcet;
    int deadline;
    int value; // higher indicates more value
    int tVal; // an atrribute to track only reduction in value, needed for value-sum calculation
    int priority; // higher indicates more priority
    int rem_execTime;
    int status; // 0: not arrived, 1: waiting, 2: executing, 3: missed deadline
    int tardiness;

  public:
    // constructor
    Task(std::string tid, int arrivalTime, int wcet, int deadline, int val, int p) : id(tid), arrivalTime(arrivalTime), wcet(wcet), deadline(deadline), value(val), priority(p) { rem_execTime = wcet; status = 0; tardiness = 0, tVal = value; }

};

class Util {

  public:
    std::vector<Task *> alltaskList;
    std::vector<Task *> readytaskList;
    std::vector<Task *> missedDeadlinetaskList;
    std::vector<Task *> completedTasks;

    int num_preemptions;
    int num_missed_deadlines;
    int num_tasks;
    int sumWcetAll;
    int sumValueAll;

  public:
    Util() : num_preemptions(0), num_missed_deadlines(0), num_tasks(0), sumWcetAll(0), sumValueAll(0) {};
    void simulateClock();
    void printStatistics();
};
