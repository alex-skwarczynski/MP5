#include "FCFS.h"

//Use Base class's function to extract process information from file
//And fill in the fcfs_queue; the priority in this queue is the arrival time; 
//Make sure you did the ArrivalEarlierThan Comparator
FCFS::FCFS(string file)
{
	cout << "Constructing FCFS" << endl;
	// Open file
	cout << "Extracting Process Info FCFS" << endl;
	extractProcessInfo(file);
	//print();
	// Populate the priority queue
	for(ProcessInfo ele:process_info)
	{
		Process proc = Process(get<0>(ele), get<1>(ele), get<2>(ele));
		fcfs_queue.push(proc);
	}
}

void FCFS::print(){
	cout<<"pid\t"<<"  arrival time\t"<<"\tburst time:\n";
	for(auto ele:process_info){
		cout<<get<0>(ele)<<"\t"<<get<1>(ele)<<"\t"<<get<2>(ele)<<endl;
	}
}

//Assuming the time the CPU starts working with process is system time 0
void FCFS::schedule_tasks(){
	int waiting_time_sum = 0;
	int response_time_sum = 0;
	int system_time = 0;
	auto proc = fcfs_queue.top();
	while(!fcfs_queue.empty())
	{
		cout << "System Time[" << system_time << "]........."
			<< "Process[PID=" << proc.getPid() << "] ";
		if(proc.is_Completed())
		{
			cout << "Finished its job!" << endl;
			response_time_sum += system_time - proc.get_arrival_time();
			waiting_time_sum += system_time - proc.get_arrival_time() - proc.get_cpu_burst_time();
			fcfs_queue.pop();
			if(!fcfs_queue.empty())
			{
				proc = fcfs_queue.top();
			}
		}
		else
		{
			cout << "is Running" << endl;
		}
		proc.Run(1);
		system_time++;
	}

	printf("Average waiting time %.2f seconds\n", ((float) waiting_time_sum) / process_info.size());
	printf("Average response time %.2f seconds\n", ((float) response_time_sum) / process_info.size());
}

FCFS::FCFS(){}