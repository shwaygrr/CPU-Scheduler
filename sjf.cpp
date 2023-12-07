#include <iomanip>

#include "src.h"

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
            handleIO(io, ready, io_process, p_clock);
        }
    }
    cpu_util = static_cast<float>(total_cpu_time)/static_cast<float>(p_clock.time); //calculate cpu utilazation

    std::cout << std::endl << std::setw(25) << std::setfill(' ') << "SJF CPU Utilization: " << std::fixed << std::setprecision(2) << cpu_util*100 << "%" << std::endl; 
    schedulerReport(p_clock, process_complete);
}