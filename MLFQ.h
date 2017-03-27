#ifndef MLFQ_HEADER
#define MLFQ_HEADER

#include <iostream>
#include <fstream>
#include <tuple>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
using namespace std;

//The defined Macro values here can be changed by you for testing your program
//But when you turn in the project, make sure you do change them back to these values!!!
#define LEVEL1_CAPACITY	2
#define LEVEL2_CAPACITY	2
#define LEVEL3_CAPACITY	2

#define LEVLE1_QUANTUM	2
#define LEVLE2_QUANTUM	3
#define LEVLE3_QUANTUM	4

#define FCFS_WORKLOAD	2

#include "Scheduler.h"
#include "ArrivalEarlierThan.h"

class MLFQ:Scheduler {
private:
	Process last_process_fcfs;
public:
	vector<vector<shared_ptr<Process> > > upperLevels;
	priority_queue<Process, vector<Process>, ArrivalEarlierThan> lowestLevel;
	void update_last_process_fcfs(Process process_to_fcfs);
	void degrade_process(shared_ptr<Process> p, unsigned int level, unsigned int pos);
	int level_jump(shared_ptr<Process> p, unsigned int start_level, unsigned int pos, unsigned int goal_level);
	void schedule_tasks();
	MLFQ(string file);
};


#endif
