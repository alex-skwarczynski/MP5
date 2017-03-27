#include "SRTF.h"

void SRTF::printByArrivalTime(){
	cout << "pid   " << "  arrival time     " << "     burst time:\n";
	for(auto ele : process_info){
		cout << get<0>(ele) << "\t" << get<1>(ele) << "\t" << get<2>(ele) << endl;
	}
}

SRTF::SRTF(string file)
{
	extractProcessInfo(file);
	std::sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
		return get<1>(t1) < get<1>(t2);
	});
	
}
//scheduling implementation
void SRTF::schedule_tasks(){
	/*
	Fill in this part
	*/
	//cout << "Scheduling task" << endl;
	bool done = false;
	std::vector<Process> processes;
	for(auto x : process_info)
	{
		Process p(get<0>(x), get<1>(x), get<2>(x));
		processes.push_back(p); 
	}

	int waiting_time_sum = 0;
	int response_time_sum = 0;

	int t = 2;
	int doneCounter = 0;
	while(!done)
	{
		/*if(t == 0)
		{
			auto proc = std::make_shared<Process>(processes[0]);
			//cout << proc->getPid() << endl;
			SRTF_queue.push(proc);
		}*/

		for(int i = 0; i < processes.size(); i++)
		{
			if(t == processes[i].get_arrival_time())
			{
				auto proc = std::make_shared<Process>(processes[i]);
				//cout << proc->getPid() << endl;
				SRTF_queue.push(proc);
			}
		}

		auto p = SRTF_queue.top();

		p->Run(1);

		cout << "System["<< (t-2) << "]........." << "Process[PID=" << p->getPid() << "] is running" << endl;

		if(p->is_Completed())
		{
			doneCounter++;
			response_time_sum += (t-2) - p->get_arrival_time();
			waiting_time_sum += (t-2) - p->get_arrival_time() - p->get_cpu_burst_time();
			cout << "System["<< (t-2) << "]........." << "Process[PID=" << p->getPid() << "] is finished its job!" << endl;
			SRTF_queue.pop();
		}

		if(doneCounter == processes.size())
		{
			done = true;
		}
		t++;

	}

	printf("Average waiting time %.2f seconds\n", ((float) waiting_time_sum) / process_info.size());
	printf("Average response time %.2f seconds\n", ((float) response_time_sum) / process_info.size());

}