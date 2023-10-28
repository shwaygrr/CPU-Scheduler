#include <iostream>
#include <vector>
#include <iomanip>

#include "structs.h"

void contextSwitchReport(const Clock& p_timer, const Queue& ready, const Queue& io, const ProcessNode* running_process);
void schedulerReport(const Clock& p_clock, std::vector<ProcessNode>& process_complete);
void FCFS(Queue& ready, Queue& io);
void SJF(Queue& ready, Queue& io);

int main() {

    std::vector<std::vector<unsigned int>> p_data = {
        {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4},
        {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8},
        {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6},
        {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3},
        {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4},
        {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8},
        {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10},
        {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6}
    };

    Queue ready, io;

    ready.create(p_data);

    //FCFS(ready, io);
    SJF(ready, io);
//   std::cout << ready.getSJ()->pid << std::endl;
//     ready.remove(4);
//     std::cout << ready.getSJ()->pid << std::endl;
//     ready.remove(2);
//     std::cout << ready.getSJ()->pid << std::endl;  
    return 0;
}

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

/***SJF and FCFS very similiar, will try to make them seperate functions if no issues arise with SJF*/

void FCFS(Queue& ready, Queue& io) {
    Clock p_clock;

    float cpu_util;

    std::vector<ProcessNode> process_complete;

    ProcessNode* running_process = ready.head;
    ProcessNode* io_process  = io.head;

    unsigned int total_cpu_time = 0, idle_time;
  
    while(!ready.isEmpty() || !io.isEmpty()) { //while either queue running operations
        
        //handle ready operations **head of ready queue is the process running on the CPU**
        
        //std::cout << "Current execution time: " << p_clock.time << " Time Units" << std::endl;
        p_clock.paused = true; //reset time


        if(ready.isEmpty() && !io.isEmpty()) { //if ready empty but io not
            std::cout << "CPU idle, All processes in I/O" << std::endl; 
            p_clock.time++;
            idle_time++;
            p_clock.paused = false;
        }
        
        if(!ready.isEmpty()) {
            
            if(running_process == nullptr) { //if ready was empty and now not empty anymore
                running_process = ready.head;
            }

            if (running_process->burst_seq[(running_process->p_counter)]) { //if process burst currently running ** != 0 **
                
                ready.updateTimes(p_clock.time, running_process);

                running_process->running_state = true; //update running state boolean
                
                //std::cout << "Process " << running_process->pid << " running " << std::endl << "Burst " << running_process->p_counter+1 << ": " << running_process->burst_seq[(running_process->p_counter)] << " time units left" << std::endl << std::endl;

                p_clock.time++; //increment timer
                total_cpu_time++;
                p_clock.paused = false; //let I/O know time was changed
                running_process->burst_seq[(running_process->p_counter)]--; //decrement sequence burst

            } else { //process finish with current cpu burst **time paused, IO will not do anything**

                std::cout << "--------------------------------" << std::endl << "Process " << running_process->pid << " Burst #" << running_process->p_counter+1 << " Complete" << std::endl << std::endl;

                if (running_process->p_counter == running_process->burst_seq.size()-1) { //if that was final cpu burst
                    std::cout << "Process " << running_process->pid <<" Complete" << std::endl;
                    
                    running_process->ttr = p_clock.time; //update turnaround time
                    

                    //sstd::cout << "tw= " << running_process->tw << std::endl << "tr= " << running_process->tr << std::endl << "ttr= " << running_process->ttr << std::endl << "--------------------------------" << std::endl;
                    
                    process_complete.push_back(*running_process);
                    ready.dequeue(); ///remove from ready queue completely
                    running_process = ready.head; //context switch after process complete
                
                } else { //just normal cpu burst

                    running_process->p_counter++; //move to next burst in process
                    running_process->running_state = false;
                    io.enqueue(*running_process); //move head to io queue
                    ready.dequeue(); //remove from ready queue
                    if(ready.head != nullptr) {
                        running_process = ready.head; //context swtich to next process in ready
                        contextSwitchReport(p_clock, ready, io, running_process);
                    } else {
                        running_process = nullptr;
                        continue;
                    } 
                }
            }
        }

        //handle io if time unit moved
        if(!io.isEmpty() && !p_clock.paused) { //time changed and io queue no empty
            io_process = io.head; //set up travesal
            
            while(io_process) { //while still process left
                io_process->burst_seq[io_process->p_counter]--; //decrement burst

                if(io_process->burst_seq[io_process->p_counter] == 0) { //finished I/O
                    //std::cout << "Process " << io_process->pid << " complete I/O #" << io_process->p_counter << " at time: " << p_clock.time << std::endl;

                    io_process->p_counter++; //move to next CPU burst
                    ready.enqueue(*io_process); //move copy to ready
                    io_process = io.remove(io_process->pid); //remove from I/O and get next process in queue
                } else {
                    io_process = io_process->next;
                }
            }
            io_process = io.head;
        }
    }
    cpu_util = static_cast<float>(total_cpu_time)/static_cast<float>(p_clock.time); //calculate cpu utilazation

    std::cout << std::endl << std::setw(25) << std::setfill(' ') << "FCFS CPU Utilization: " << std::fixed << std::setprecision(2) << cpu_util*100 << "%" << std::endl; 
    schedulerReport(p_clock, process_complete);
}

void SJF(Queue& ready, Queue& io) {
    Clock p_clock;

    float cpu_util;

    std::vector<ProcessNode> process_complete;

    ProcessNode* running_process = ready.getSJ();
    ProcessNode* io_process  = nullptr;

    unsigned int total_cpu_time = 0, idle_time = 0;

    while(!ready.isEmpty() || !io.isEmpty()) { //while either queue running operations
        
        //std::cout << "Current execution time: " << p_clock.time << " Time Units" << std::endl;
        p_clock.paused = true; //reset time


        if(ready.isEmpty() && !io.isEmpty()) { //if ready empty but io not
            std::cout << "CPU idle - all processes in I/O" << std::endl; 
            p_clock.time++;
            idle_time++;
            p_clock.paused = false;
        }
        
        if(!ready.isEmpty()) {
            
            if(running_process == nullptr) { //if ready was empty and now not empty anymore
                running_process = ready.getSJ();
            }

            if (running_process->burst_seq[(running_process->p_counter)]) { //if process burst currently running ** != 0 **
                
                ready.updateTimes(p_clock.time, running_process);

                running_process->running_state = true; //update running state boolean

                //std::cout << "Process " << running_process->pid << " running " << std::endl << "Burst " << running_process->p_counter+1 << ": " << running_process->burst_seq[(running_process->p_counter)] << " time units left" << std::endl << std::endl;

                p_clock.time++; //increment timer
                total_cpu_time++;
                p_clock.paused = false; //let I/O know time was changed
                running_process->burst_seq[(running_process->p_counter)]--; //decrement sequence burst

            } else { //process finish with current cpu burst **time paused, IO will not do anything**

                std::cout << "--------------------------------" << std::endl << "Process " << running_process->pid << " Burst #" << running_process->p_counter+1 << " Complete" << std::endl << std::endl;

                if (running_process->p_counter == running_process->burst_seq.size()-1) { //if that was final cpu burst
                    std::cout << "Process " << running_process->pid <<" Complete" << std::endl;
                    
                    running_process->ttr = p_clock.time; //update turnaround time
                    
                    //std::cout << "tw= " << running_process->tw << std::endl << "tr= " << running_process->tr << std::endl << "ttr= " << running_process->ttr << std::endl << "--------------------------------" << std::endl;
                    
                    process_complete.push_back(*running_process);
                    ready.remove(running_process->pid); ///remove from ready queue completely **returns next node, might have to overload**
                    running_process = ready.getSJ(); //context switch after process complete
                
                } else { //just normal cpu burst

                    running_process->p_counter++; //move to next burst in process
                    running_process->running_state = false;
                    io.enqueue(*running_process); //move head to io queue
                    ready.remove(running_process->pid); //remove from ready queue
                    if(ready.head != nullptr) {
                        running_process = ready.getSJ(); //context swtich to next process in ready
                        contextSwitchReport(p_clock, ready, io, running_process);
                    } else {
                        running_process = nullptr;
                        continue;
                    } 
                }
            }
        }

        //handle io if time unit moved
        if(!io.isEmpty() && !p_clock.paused) { //time changed and io queue no empty
            io_process = io.head; //set up travesal
            
            while(io_process) { //while still process left
                io_process->burst_seq[io_process->p_counter]--; //decrement burst

                if(io_process->burst_seq[io_process->p_counter] == 0) { //finished I/O
                    std::cout << "Process " << io_process->pid << " complete I/O #" << io_process->p_counter << " at time: " << p_clock.time << std::endl;

                    io_process->p_counter++; //move to next CPU burst
                    ready.enqueue(*io_process); //move copy to ready
                    io_process = io.remove(io_process->pid); //remove from I/O and get next process in queue
                } else {
                    io_process = io_process->next;
                }
            }
            io_process = io.head;
        }
    }
    cpu_util = static_cast<float>(total_cpu_time)/static_cast<float>(p_clock.time); //calculate cpu utilazation

    std::cout << std::endl << std::setw(25) << std::setfill(' ') << "SJF CPU Utilization: " << std::fixed << std::setprecision(2) << cpu_util*100 << "%" << std::endl; 
    schedulerReport(p_clock, process_complete);
}
