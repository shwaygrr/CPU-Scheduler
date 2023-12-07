#include <iomanip>

#include "src.h"


//report functions
void contextSwitchReport(const Clock& p_clock, const Queue& ready, const Queue& io, const ProcessNode* running_process) {
    /*
        (5) Sample of dynamic execution (program output)
            **This information should be displayed for each context switch**
            1. Current Execution time: **displayed by clock**
            2. Running process: **displayed by head of ready**
            3. The Ready queue, with the CPU burst time for each process: **display queue function and the counter**
            4. The Processes in I/O with the remaining time for every process for its I/O burst completion: **display queue and counter**
            5. Indicate when a process has completed its total execution. **Final Burst statements**
    */

    std::cout << "--------------------------------" << std::endl << "***Context Switch Report***" << std::endl << std::endl;

    std::cout << "Current Execution time: " << p_clock.time << std::endl;
    std::cout << "Running Process: " << running_process->pid << std::endl;
    std::cout << std::endl << "Processes in Ready: " << std::endl;
    ready.display(); //process in ready
    std::cout << std::endl << "Processes in I/O: " << std::endl;
    io.display(); //processes in io

    std::cout << "--------------------------------" << std::endl;
}

void schedulerReport(const Clock& p_clock, std::vector<ProcessNode>& process_complete) {

    float avg_tw = 0.0, avg_ttr= 0.0, avg_tr = 0.0;
    /*
        (6) Results printed at the end of each simulation
            **This information should be displayed at the end of each simulation**
            1. Total time needed to complete all 8 processes.
            2. CPU utilization - [%] (U).
            3. Waiting times for each process and the average waiting time for all processes (Tw)
            4. Turnaround time for each process and the average turnaround time.(Ttr)
            5. Response time for each process and the average response time (Tr).
    */

   for(ProcessNode& completed : process_complete) {
        avg_tw += completed.tw;
        avg_ttr += completed.ttr;
        avg_tr += completed.tr;
   }

   avg_tw /= 8.0;
   avg_ttr /= 8.0;
   avg_tr /= 8.0;

    const int columnWidth = 10;
    std::cout << "---------------------------------" << std::endl;
    // Print the table header
    std::cout << std::setw(columnWidth) << std::left << "Process"
                << std::setw(columnWidth) << std::left << "Tw"
                << std::setw(columnWidth) << std::left << "Ttr" 
                << std::setw(columnWidth) << std::left << "Tr" << std::endl;

    std::cout << "---------------------------------" << std::endl;

    // Print the table rows
    for(ProcessNode& complete_process : process_complete) {
        std::cout << std::setw(columnWidth) << std::left << complete_process.pid
                    << std::setw(columnWidth) << std::left << complete_process.tw 
                    << std::setw(columnWidth) << std::left << complete_process.ttr 
                    << std::setw(columnWidth) << std::left << complete_process.tr << std::endl;
    }
   
   std::cout << "---------------------------------" << std::endl;
   std::cout << std::setw(columnWidth) << std::left << "Avg"
                    << std::setw(columnWidth-1) << std::fixed << std::setprecision(2) << std::left << avg_tw 
                    << std::setw(columnWidth-1) << std::fixed << std::setprecision(2) << std::left << avg_ttr 
                    << std::setw(columnWidth-1) << std::fixed << std::setprecision(2) << std::left << avg_tr << std::endl;

    std::cout << "---------------------------------" << std::endl;
    std::cout << "Total Time to Complete: " << p_clock.time;
}
