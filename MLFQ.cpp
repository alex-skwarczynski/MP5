#include "MLFQ.h"

//The goal of this function is to fill the 4 levels with process_info 
//You should sort the process_info by the arrival_time of the Process;
//Process with smaller arrival time will have smaller index in the vector

//After filling in the top 3 levels, then do insertion for the fcfs level
MLFQ::MLFQ(string file)
{
	for (int i = 0; i < 3; i++)
	{
		upperLevels.push_back(vector<shared_ptr<Process> >());
	}
	// Extract file
	extractProcessInfo(file);
	// Populate the priority queue
	std::sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
		return get<1>(t1) < get<1>(t2);
	});
	int queue_level = 0;
	for(ProcessInfo ele:process_info)
	{
		Process proc = Process(get<0>(ele), get<1>(ele), get<2>(ele));
		switch (queue_level)
		{
		case 0:
			upperLevels.at(queue_level).push_back(make_shared<Process>(proc));
			if (upperLevels.at(queue_level).size() == LEVEL1_CAPACITY)
				queue_level++;
			break;
		case 1:
			upperLevels.at(queue_level).push_back(make_shared<Process>(proc));
			if (upperLevels.at(queue_level).size() == LEVEL2_CAPACITY)
				queue_level++;
			break;
		case 2: 
			upperLevels.at(queue_level).push_back(make_shared<Process>(proc));
			if (upperLevels.at(queue_level).size() == LEVEL3_CAPACITY)
				queue_level++;
			break;
		default:
			lowestLevel.push(proc);
			update_last_process_fcfs(proc);
			break;
		}
	}
}
//This function is used to keep track of the process who entered the fcfs queue most recently
//The purpose is to properly adjust the upcoming process's arrival time
//If the arrival time is changed, you can still check its original arrival time by accessing the 
//process_info variable
void MLFQ::update_last_process_fcfs(Process process_to_fcfs) {
	last_process_fcfs = process_to_fcfs;
}

//This function has the following assumptions:
/*
	1. The start_level is not equal goal_level;
	2. goal_level is larger then start_level
	3. When you use this function, you should know the process should not jump from level 0 to level 2 or 3 if the level 1 has a space there.
	   Generally, when you degrade a process, it tries to go to the level below by one level and if that level is full, it will keep going down 
	   until it finds a level which has space there.
	4. Successful jump will return 1, else 0
	5. To successfully jump to the goal_level, the process must go to the end of the vector corresponding to goal_level
*/
//start_level is the level the process is located at, it is one value of 0 , 1, 2;
//pos is its index in the vector
//goal_level is the level it tries to enter
int MLFQ::level_jump(shared_ptr<Process> p, unsigned int start_level, unsigned int pos, unsigned int goal_level) {
	try
	{
		if (goal_level == 3)
		{
			p->change_arrival_time(last_process_fcfs.get_arrival_time()+1);
			lowestLevel.push(Process(*p));
			update_last_process_fcfs(*p);
		}
		else
		{
			upperLevels.at(goal_level).push_back(p);
		}
		upperLevels.at(start_level).erase(upperLevels.at(start_level).begin() + pos);
		return 1;
	}
	catch (...)
	{
		return 0;
	}
}


/*
 p is the process which is going to be degrade to lower levle
 levle is the level it is located currently
 legal value of level can be: 0, 1, 2 Since no need to degrade for last level
 pos is the its index in the vector

0: it is located at the top level
1:  it is located at the second highest level
2:  it is located at the third highest level
3: it is in the fcfs level, no need to degrade, it will stay there until finishing the job and leave
*/

//pos is the index of the process in the vector
//Your goal is to degrade this process by one level
//You can use level_jump() function here based on which level the process is going to jump
void MLFQ::degrade_process(shared_ptr<Process> p, unsigned int level, unsigned int pos) {
	if (level == 0)
	{
		if (upperLevels.at(1).size() < LEVEL2_CAPACITY)
		{
			level_jump(p, level, pos, 1);
		}
		else if (upperLevels.at(2).size() < LEVEL3_CAPACITY)
		{
			level_jump(p, level, pos, 2);
		}
		else
		{
			level_jump(p, level, pos, 3);
		}
	}
	if (level == 1)
	{
		if (upperLevels.at(2).size() < LEVEL3_CAPACITY)
		{
			level_jump(p, level, pos, 2);
		}
		else
		{
			level_jump(p, level, pos, 3);
		}
	}
	if (level == 2)
	{
		level_jump(p, level, pos, 3);	
	}
}

/*
You can use multiple loops here to do the job based on the document;
Make sure print out the timing information correctly
*/
void MLFQ::schedule_tasks(){
	int waiting_time_sum = 0;
	int response_time_sum = 0;
	int system_time = 0;
	while (!(upperLevels.at(0).empty() && upperLevels.at(0).empty() && upperLevels.at(0).empty() && lowestLevel.empty()))
	{
		cout << "System Time[" << system_time << "]..........First Level Queue Starts Working" << endl;
		while(!upperLevels.at(0).empty())
		{
			auto ele = upperLevels.at(0).at(0);
			cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Starts Working" << endl;
			for (int i = 0; i < LEVLE1_QUANTUM; i++)
			{
				cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Is Running" << endl;
				ele->Run(1);
				system_time++;
				if(ele->is_Completed())
				{ 
					cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Finished Running" << endl;
					response_time_sum += system_time - ele->get_arrival_time();
					waiting_time_sum += system_time - ele->get_arrival_time() - ele->get_cpu_burst_time();
					break; 
				}
			}
			if(ele->is_Completed())
			{
				upperLevels.at(0).erase(upperLevels.at(0).begin());
			}
			else
			{
				cout << "Degrading [PID=" << ele->getPid() << "]" << endl;
				degrade_process(ele, 0, 0);
			}
		}
		//-----------------------------------------------------------------------------------------------------------------------------------
		cout << "System Time[" << system_time << "]..........Second Level Queue Starts Working" << endl;
		while(!upperLevels.at(1).empty())
		{
			auto ele = upperLevels.at(1).at(0);
			cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Starts Working" << endl;
			for (int i = 0; i < LEVLE2_QUANTUM; i++)
			{
				cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Is Running" << endl;
				ele->Run(1);
				system_time++;
				if(ele->is_Completed())
				{ 
					cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Finished Running" << endl;
					response_time_sum += system_time - ele->get_arrival_time();
					waiting_time_sum += system_time - ele->get_arrival_time() - ele->get_cpu_burst_time();
					break; 
				}
			}
			if(ele->is_Completed())
			{
				upperLevels.at(1).erase(upperLevels.at(1).begin());
			}
			else
			{
				cout << "Degrading [PID=" << ele->getPid() << "]" << endl;
				degrade_process(ele, 1, 0);
			}
		}
		//-----------------------------------------------------------------------------------------------------------------------------------
		cout << "System Time[" << system_time << "]..........Third Level Queue Starts Working" << endl;
		while(!upperLevels.at(2).empty())
		{
			auto ele = upperLevels.at(2).at(0);
			cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Starts Working" << endl;
			for (int i = 0; i < LEVLE3_QUANTUM; i++)
			{
				cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Is Running" << endl;
				ele->Run(1);
				system_time++;
				if(ele->is_Completed())
				{ 
					cout << "System Time[" << system_time << "]..........Process[PID=" << ele->getPid() << "] Finished Running" << endl;
					response_time_sum += system_time - ele->get_arrival_time();
					waiting_time_sum += system_time - ele->get_arrival_time() - ele->get_cpu_burst_time();
					break; 
				}
			}
			if(ele->is_Completed())
			{
				upperLevels.at(2).erase(upperLevels.at(2).begin());
			}
			else
			{
				cout << "Degrading [PID=" << ele->getPid() << "]" << endl;
				degrade_process(ele, 2, 0);
			}
		}
		//-----------------------------------------------------------------------------------------------------------------------------------
		cout << "System Time[" << system_time << "]..........FCFS Queue Starts Working" << endl;
		for (int i = 0; i < FCFS_WORKLOAD; i++)
		{
			if (!lowestLevel.empty())
			{
				auto ele = lowestLevel.top();
				cout << "System Time[" << system_time << "]..........Process[PID=" << ele.getPid() << "] Starts Working" << endl;
				while(!ele.is_Completed())
				{
					cout << "System Time[" << system_time << "]..........Process[PID=" << ele.getPid() << "] Is Running"<< endl;
					ele.Run(1);
					system_time++;
				}
				cout << "System Time[" << system_time << "]..........Process[PID=" << ele.getPid() << "] finished its job!" << endl;
				response_time_sum += system_time - ele.get_og_arrival_time();
				waiting_time_sum += system_time - ele.get_og_arrival_time() - ele.get_cpu_burst_time();
				lowestLevel.pop();
			}
		}
	}
	printf("Average waiting time %.2f seconds\n", ((float) waiting_time_sum) / process_info.size());
	printf("Average response time %.2f seconds\n", ((float) response_time_sum) / process_info.size());
}
