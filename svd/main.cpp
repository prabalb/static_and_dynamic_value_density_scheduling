/* Static Value Density (SVD) Scheduling Algorithm
 * Author: PRABAL BASU (A02049867)
 */


#include "scheduler.h"
#include <fstream>
#include <cstring>
#include <algorithm>

int main() {

  Util utilObj;

  std::string fileName = ""; 
  ifstream file;

  std::cout << "Please specify full path of the input file: ";
  cin >> fileName;

  file.open(fileName.c_str());

  if (file.is_open()) {
    while (!file.eof())
    {   
      char buf[100];
      file.getline(buf, 100);

      const char* token[5] = {};
      token[0] = strtok(buf, " ");
      if(token[0]) {
        
        std::string taskID = token[0];
        std::cout << "task id: " << taskID << std::endl;

        token[1] = strtok(0, " ");
        int arrivalTime = atoi(token[1]);
        std::cout << "arrival time: " << arrivalTime << std::endl;

        token[2] = strtok(0, " ");
        int wcet = atoi(token[2]);
        std::cout << "WCET: " << wcet << std::endl;

        token[3] = strtok(0, " ");
        int deadline = atoi(token[3]);
        std::cout << "Absolute deadline: " << deadline << std::endl;

        token[4] = strtok(0, " ");
        int value = atoi(token[4]);
        std::cout << "Value: " << value << std::endl;
        
        Task *tTask = new Task(taskID, arrivalTime, wcet, deadline, value, 0);
        utilObj.alltaskList.push_back(tTask);

      }

    }
    std::cout << "size of taskList: " << utilObj.alltaskList.size() << std::endl;
    utilObj.num_tasks = int(utilObj.alltaskList.size());

    file.close();
  } else {
    std::cout << "Could not open the input file specified, please try again ..." << "\n";
    exit(1);
  }

  utilObj.simulateClock();
  utilObj.printStatistics();

  return 0;
}
