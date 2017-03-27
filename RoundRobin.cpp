#include "RoundRobin.h"

//No need to change it
RoundRobin::RoundRobin() {
	time_quantum = 0;
}


/*
This is a constructor for RoundRobin Scheduler, you should use the extractProcessInfo function first
to load process information to process_info and then sort process by arrival time;
Also, fill in the procesVec with shared_ptr

Also initialize time_quantum
*/
RoundRobin::RoundRobin(string file, int time_quant) {
	extractProcessInfo(file);
	time_quantum = time_quant;
	
}

void RoundRobin::set_time_quantum(int quantum) {
	time_quantum = quantum;
}

int RoundRobin::get_time_quantum() {
	return time_quantum;
}


//Schedule tasks based on RoundRobin Rule
//the jobs are put in the order the arrived
//Make sure you print out the information like we put in the document
void RoundRobin::schedule_tasks() {
	/*
	Do this part.
	*/

	int waiting_time_sum = 0;
	int response_time_sum = 0;

	int t = 2;
	bool done = false;
	while(!done)
	{
	

		int i = 0;
		for(auto x : process_info)
		{
			Process p(get<0>(x), get<1>(x), get<2>(x));
			auto proc = make_shared<Process>(p);
			processVec.push_back(proc);
		}
		
		while(processVec.size() != 0)
		{
			if(i > processVec.size()-1)
			{
				i = 0;
			}

			auto p = processVec[i];
			while(p->get_arrival_time() > t)
			{
				auto p = processVec[i++];
			}

			for(int i = 0; i < time_quantum; i++)
			{
				p->Run(1);

				cout << "System["<< (t-2) << "]........." << "Process[PID=" << p->getPid() << "] is running" << endl;
				t++;
				if(p->is_Completed())
				{
					break;
				}
			}

			if(p->is_Completed())
			{
				response_time_sum += (t-2) - p->get_arrival_time();
				waiting_time_sum += (t-2) - p->get_arrival_time() - p->get_cpu_burst_time();
				cout << "System["<< (t-2) << "]........." << "Process[PID=" << p->getPid() << "] is finished its job!" << endl;
				processVec.erase(processVec.begin()+i);
			}
			else
				i++;

		}

		if(processVec.size() == 0)
		{
			done = true;
		}
	}

	printf("Average waiting time %.2f seconds\n", ((float) waiting_time_sum) / process_info.size());
	printf("Average response time %.2f seconds\n", ((float) response_time_sum) / process_info.size());
}

//Fill in the procesVec with shared_ptr
//And set time_quantum, or you can also use set_time_quantum function for setting quantum
RoundRobin::RoundRobin(vector<ProcessInfo> &process_info, int time_quant){

	for(auto x : process_info)
	{
		Process p(get<0>(x), get<1>(x), get<2>(x));
		auto proc = make_shared<Process>(p);
		processVec.push_back(proc);
	}

	time_quantum = time_quant;


}

void RoundRobin::print(){
	for (auto ele : processVec){
		cout << ele->getPid() << "; " << ele->get_arrival_time() << "; " << ele->get_cpu_burst_time() << endl;
	}
}