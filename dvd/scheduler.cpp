/* Dynamic Value Density (DVD) Scheduling Algorithm
 * Author: PRABAL BASU (A02049867)
 */


#include "scheduler.h"
#include <cmath>

struct order
{
  bool operator()(Task *a, Task *b) const
  {
    return (a->priority > b->priority);
  }
};

void Util::simulateClock()
{
  
  int tick = 0;
  int count = 0;

  // collect the sum of wcets and values of all the tasks
  std::vector<Task *>::iterator aIter;
  for(aIter = alltaskList.begin(); aIter != alltaskList.end(); aIter++) {
    sumWcetAll += (*aIter)->wcet;
    sumValueAll += (*aIter)->value;
  }

  while(1) {  // infinite loop: will quit when all the
              // tasks have been removed from readytaskList
  
      std::vector<Task *>::iterator tIter;
      for(tIter = alltaskList.begin(); tIter != alltaskList.end(); tIter++) {
	    // Move tasks from alltaskList to ready list if the arrival time is less than 
	    // or equal to the tick time. Update the task status to 1(waiting) and update priority
        int rArrivalTime = (*tIter)->arrivalTime;
        if(rArrivalTime > tick) break;
        (*tIter)->status = 1; // ready/waiting
        (*tIter)->priority = (*tIter)->value * 100000 / ((*tIter)->rem_execTime * (*tIter)->rem_execTime);
        readytaskList.push_back(*tIter);
        tIter = alltaskList.erase(tIter);
        tIter--;
	  }
   
      // sort readytaskList based on priority
      std::sort(readytaskList.begin(), readytaskList.end(), order());
      //std::cout << "====================" << std::endl;
      //for(tIter = readytaskList.begin(); tIter != readytaskList.end(); tIter++) {
      //  std::cout << "taskID: " << (*tIter)->id << ", priority: " << (*tIter)->priority << std::endl;
      //}
      //std::cout << "====================" << std::endl;

      if(!readytaskList.empty()) { // If there are ready tasks to be executed 
				    //select the task with highest priority.

				    // Compare the priority of the selected task with the
				    // executing task. Decide preempt
				    // or Not. 
				    // if preempt, update Status to 1(waiting).
                    // continue executing if no preemption.
				    // Keep running count of preemptions.

				    // execute the task with the highest priority.
				    // i.e reduce the r_execTime by 1 and updateStatus to 2.
				    // Once reduced check if the r_execTime == 0;
				    // If true delete task signifying successful completion.

				    // Check the readytaskList for tasks whose deadlines have been exceeded
				    // If any found, reduce v, recalculate p, inrement tardyness.
                    // If value = 0, add it to
				    // missedDeadlinetaskList for stat collection later.
				    // update Status to 3(deadline missed).  

        count = 0;
        std::vector<Task *>::iterator rIter;
        for(rIter = readytaskList.begin(); rIter != readytaskList.end(); rIter++) {
          if(tick > (*rIter)->deadline) {
            if((*rIter)->status == 1) {
              (*rIter)->value--;
              (*rIter)->tVal--;
              (*rIter)->priority = (*rIter)->value * 100000 / ((*rIter)->rem_execTime * (*rIter)->rem_execTime);
              //std::cout << "Task: " << (*rIter)->id << ", value: " << (*rIter)->value << ", priority: " << (*rIter)->priority << std::endl;
            }
            (*rIter)->tardiness++; 
            //std::cout << "task: " << (*rIter)->id << ", tardiness: " << (*rIter)->tardiness << std::endl;
            if((*rIter)->value == 0) {
              missedDeadlinetaskList.push_back(*rIter);
              (*rIter)->status = 3;
              rIter = readytaskList.erase(rIter);
              rIter--;
            }
          }
        }

        int tPriority = readytaskList[0]->priority;
        Task *tTask = NULL;
        if(readytaskList.empty()) {
          std::cout << "ReadytaskList is empty ..." << std::endl;
          tick++;
          continue;
        }

        for(rIter = readytaskList.begin(); rIter != readytaskList.end(); rIter++) {
          int curTaskPriority = (*rIter)->priority;
          if(curTaskPriority >= tPriority) {
            tPriority = curTaskPriority;
            tTask = (*rIter);
          }
        }

        if(tTask->status == 2) {
          for(rIter = readytaskList.begin(); rIter != readytaskList.end(); rIter++) {
            if(*rIter != tTask) continue;
            if((*rIter)->tardiness > 0) {
              (*rIter)->value++;
              (*rIter)->priority = (*rIter)->value * 100000 / ((*rIter)->rem_execTime * (*rIter)->rem_execTime);
              //std::cout << "Task: " << (*rIter)->id << ", value: " << (*rIter)->value << ", priority: " << (*rIter)->priority << std::endl;
            }
            (*rIter)->rem_execTime--;
            if((*rIter)->rem_execTime == 0) {
              completedTasks.push_back(*rIter);
              rIter = readytaskList.erase(rIter);
            }
            break;
          }
        } else {
          for(rIter = readytaskList.begin(); rIter != readytaskList.end(); rIter++) {
            int status = (*rIter)->status;
            if(status == 2) {
              (*rIter)->status = 1; // task waiting
              //std::cout << "Preempting task " << (*rIter)->id << std::endl;
              num_preemptions++;
              break;
            }
          }
          for(rIter = readytaskList.begin(); rIter != readytaskList.end(); rIter++) {
            if(*rIter != tTask) continue;
            (*rIter)->status = 2; // task executing

            if((*rIter)->tardiness > 0) {
              (*rIter)->value++;
              (*rIter)->priority = (*rIter)->value * 100000 / ((*rIter)->rem_execTime * (*rIter)->rem_execTime);
              //std::cout << "Task: " << (*rIter)->id << ", value: " << (*rIter)->value << ", priority: " << (*rIter)->priority << std::endl;
            }

            (*rIter)->rem_execTime--;
            if((*rIter)->rem_execTime == 0) {
              completedTasks.push_back(*rIter);
              readytaskList.erase(rIter);
            }
            break;
          }
        }
	  } else {
        if(count > 5000) break;
        count++;
        std::cout << "idle cycle ..." << std::endl;
      }

	  tick++;
  }
}

void Util::printStatistics()
{
  std::cout << "Number of preemptions: " << num_preemptions << std::endl;
  std::cout << "Number of missed deadlines: " << missedDeadlinetaskList.size() << std::endl;

  // success rate
  int total_tasks_completed = num_tasks - missedDeadlinetaskList.size();
  float success = float(total_tasks_completed) * 100 / num_tasks;
  std::cout << "success % " << success << std::endl;

  // preemptions
  float preemptions = float(num_preemptions) * 100 / num_tasks;
  std::cout << "preemption % " << preemptions << std::endl;
 
  // CPU wastage
  int sumWcetMissed = 0;
  std::vector<Task *>::iterator aIter;
  for (aIter = missedDeadlinetaskList.begin(); aIter != missedDeadlinetaskList.end(); aIter++) {
    sumWcetMissed += (*aIter)->wcet;
  }

  int sumExecutedMissed = 0;
  for (aIter = missedDeadlinetaskList.begin(); aIter != missedDeadlinetaskList.end(); aIter++) {
    sumExecutedMissed += (*aIter)->wcet - (*aIter)->rem_execTime;
  }

  float cpu_wastage = float(sumExecutedMissed) * 100 / (sumWcetAll + sumExecutedMissed - sumWcetMissed);
  std::cout << "CPU wastage: " << cpu_wastage << std::endl;

  // tardy statistics
  int total_num_tasks_completed = int(completedTasks.size());
  int num_tardy_tasks = 0;
  int total_tardiness = 0;
  for (aIter = completedTasks.begin(); aIter != completedTasks.end(); aIter++) {
    if((*aIter)->tardiness > 0) {
      num_tardy_tasks++;
      total_tardiness += (*aIter)->tardiness;
    }
  }

  std::cout << "size of completed tasks: " << total_num_tasks_completed << std::endl;
  std::cout << "size of tardy tasks: " << num_tardy_tasks << std::endl;

  float Tardy = float(num_tardy_tasks) * 100 / total_num_tasks_completed;
  std::cout << "Tardy % "  << Tardy << std::endl;

  float Tardiness = float(total_tardiness) / num_tardy_tasks;
  std::cout << "Tardiness: " << Tardiness << std::endl;

  // value-sum
  int total_value_collected = 0;
  for(aIter = completedTasks.begin(); aIter != completedTasks.end(); aIter++) {
    total_value_collected += (*aIter)->tVal;
  }
  float value_sum = float(total_value_collected) * 100 / sumValueAll;
  std::cout << "Value-Sum% " << value_sum << std::endl;
}
