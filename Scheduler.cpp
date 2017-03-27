#include "Scheduler.h"
//#include <iostream>

//Read a process file to extract process information
//All content goes to proces_info vector
void Scheduler::extractProcessInfo(string file){
    ifstream task_file(file);
    int pid, arrival, execution;
    ProcessInfo proc;
    while (task_file >> pid >> arrival >> execution)
    {
        proc = make_tuple(pid, arrival, execution);
        process_info.push_back(proc);
    }
}