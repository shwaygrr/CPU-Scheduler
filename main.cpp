#include <iostream>
#include <vector>

#include "structs.h"

void contextSwitchReport(const Clock& p_timer, const Queue& ready, const Queue& io);
void FCFS(Queue& ready, Queue& io);

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

    FCFS(ready, io);

    return 0;
}

//report functions
void contextSwitchReport(const Clock& p_clock, const Queue& ready, const Queue& io) {
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
    std::cout << "Running Process: " << ready.head->pid << std::endl;
    std::cout << std::endl << "Processes in Ready: " << std::endl;
    ready.display(); //process in ready
    std::cout << std::endl << "Processes in I/O: " << std::endl;
    io.display(); //processes in io

    std::cout << "--------------------------------" << std::endl;
}


void FCFS(Queue& ready, Queue& io) {
    Clock p_clock;

    unsigned int avg_tw, avg_ttr, avg_tr;

    float cpu_util;

    std::vector<int> process_complete;

    ProcessNode* ready_process = ready.head;
    ProcessNode* io_process  = io.head;

  
    while(!ready.isEmpty() || !io.isEmpty()) { //while either queue running operations
        
        //handle ready operations **head of ready queue is the process running on the CPU**
        
        //std::cout << "Current execution time: " << p_clock.time << " Time Units" << std::endl;
        p_clock.paused = true; //reset time


        if(ready.isEmpty() && !io.isEmpty()) { //if ready empty but io not
            //std::cout << "Ready Queue empty, All remaining processes in I/O" << std::endl; 
            p_clock.time++;
            p_clock.paused = false;
        }
        
        if(!ready.isEmpty()) {
            
            if(ready_process == nullptr) { //if ready was empty and now not empty anymore
                ready_process = ready.head;
            }

            if (ready_process->burst_seq[(ready_process->p_counter)]) { //if process burst currently running ** != 0 **
                
                ready.updateTimes(p_clock.time);
                
                ready_process->running_state = true; //update running state boolean
                
                //std::cout << "Process " << ready_process->pid << " running " << std::endl << "Burst " << ready_process->p_counter+1 << ": " << ready_process->burst_seq[(ready_process->p_counter)] << " time units left" << std::endl << std::endl;

                p_clock.time++; //increment timer
                p_clock.paused = false; //let I/O know time was changed
                ready_process->burst_seq[(ready_process->p_counter)]--; //decrement sequence burst

            } else { //process finish with current cpu burst **time paused, IO will not do anything**

                std::cout << "--------------------------------" << std::endl << "Process " << ready_process->pid << " Burst #" << ready_process->p_counter+1 << " Complete" << std::endl << std::endl;

                if (ready_process->p_counter == ready_process->burst_seq.size()-1) { //if that was final cpu burst
                    std::cout << "Process " << ready_process->pid <<" Final Burst #" << ready_process->p_counter+1 << std::endl;
                    
                    ready_process->ttr = p_clock.time; //update turnaround time
                    

                    std::cout << "tw= " << ready_process->tw << std::endl << "tr= " << ready_process->tr << std::endl << "ttr: " << ready_process->ttr << std::endl << "--------------------------------" << std::endl;
                    
                    process_complete.push_back(ready_process->pid);
                    ready.dequeue(); ///remove from ready queue completely
                    ready_process = ready.head; //context switch after process complete
                
                } else { //just normal cpu burst

                    ready_process->p_counter++; //move to next burst in process
                    ready_process->running_state = false;
                    io.enqueue(*ready_process); //move head to io queue
                    ready.dequeue(); //remove from ready queue
                    if(ready.head != nullptr) {
                        ready_process = ready.head; //context swtich to next process in ready
                        contextSwitchReport(p_clock, ready, io);
                    } else {
                        ready_process = nullptr;
                        continue;
                    } 
                }
            }
        }

        //handle io if time unit moved
        if(!io.isEmpty() && !p_clock.paused) { //time changed and io queue no empty
            io_process = io.head; //set up travesal
            
            while(io_process) { //while still process left
                io_process->burst_seq[io_process->p_counter]--;

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

    for(int& pid : process_complete) {
        std::cout << pid << " ";
    }
}

