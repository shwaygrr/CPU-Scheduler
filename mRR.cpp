#include <iomanip>

#include "src.h"


int mRR(Queue& ready, Queue& io, unsigned int tq) {
    
    Clock p_clock;

    float cpu_util;

    std::vector<ProcessNode> process_complete;
    
    ProcessNode* running_process = ready.head;
    ProcessNode* io_process;

    unsigned int total_cpu_time = 0, idle_time = 0;

    unsigned int tq_tracker = 0;

    std::cout << std::endl << "Starting Round Robin queue with time quantum " << tq << std::endl;
    ready.display();

    while (!ready.isEmpty() || !io.isEmpty()) {
        p_clock.paused = true; //reset time

        if (ready.isEmpty() && !io.isEmpty()) {
            std::cout << "CPU idle all processes in IO" << std::endl;
            p_clock.time++;
            p_clock.paused = false;
        }

        if(!ready.isEmpty()) {
        
            std::cout << "Current execution time: " << p_clock.time << " Time Units" << std::endl;
            
            if(running_process == nullptr) { //if ready was empty and now not empty anymore
                running_process = ready.head;
            }

            //if time quantum expires and process still has more to run
            if(tq_tracker == tq && running_process->burst_seq[(running_process->p_counter)]) { //time quantum expired 
                std::cout << "Time quantum expired for process " << running_process->pid << std::endl;

                ready.enqueue(*running_process);
                ready.dequeue();
                if( ready.head != nullptr) {
                    running_process = ready.head; //context switch after process complete
                    tq_tracker = 0;
                    contextSwitchReport(p_clock, ready, io, running_process);
                } else {
                    running_process = nullptr;
                    tq_tracker = 0;
                    continue;
                }
            }
            //time splice not expire or process has no more to run 
            else { // time quantum not exprired and process still has more to run
                if (running_process->burst_seq[(running_process->p_counter)]) { //if process burst currently running ** != 0 **
                
                    ready.updateTimes(p_clock.time, running_process);

                    running_process->running_state = true; //update running state boolean
                    
                    std::cout << "Process " << running_process->pid << " running " << std::endl << "Burst " << running_process->p_counter+1 << ": " << running_process->burst_seq[(running_process->p_counter)] << " time units left" << std::endl << std::endl;

                    tq_tracker++;
                    p_clock.time++; //increment timer
                    total_cpu_time++;
                    p_clock.paused = false; //let I/O know time was changed
                    running_process->burst_seq[(running_process->p_counter)]--; //decrement sequence burst
                } else { //burst complete
                    
                    //final burst
                    if (running_process->p_counter == running_process->burst_seq.size()-1) { //final burst
                        std::cout << "Process " << running_process->pid << " Complete" << std::endl;
                    
                        running_process->ttr = p_clock.time; //update turnaround time
                    
                        //std::cout << "tw= " << running_process->tw << std::endl << "tr= " << running_process->tr << std::endl << "ttr= " << running_process->ttr << std::endl << "--------------------------------" << std::endl;
                    
                        process_complete.push_back(*running_process);
                        ready.dequeue(); ///remove from ready queue completely **returns next node, might have to overload**
                        if( ready.head != nullptr) {
                            running_process = ready.head; //context switch after process complete
                            tq_tracker = 0;
                            contextSwitchReport(p_clock, ready, io, running_process);
                        } else {
                            running_process = nullptr;
                            tq_tracker = 0;
                            continue;
                        }
                    } 
                    //middle burst
                    else {
                        std::cout << "--------------------------------" << std::endl << "Process " << running_process->pid << " Burst #" << running_process->p_counter+1 << " Complete" << std::endl << std::endl;

                        running_process->p_counter++;
                        io.enqueue(*running_process);
                        ready.dequeue();
                        if( ready.head != nullptr) {
                            running_process = ready.head; //context switch after process complete
                            tq_tracker = 0;
                            contextSwitchReport(p_clock, ready, io, running_process);
                        } else {
                            running_process = nullptr;
                            tq_tracker = 0;
                            continue;
                        }  
                    }
                }
            }
        }
        //handle io  
        if(!io.isEmpty() && !p_clock.paused) { //time changed and io queue no empty
            handleIO(io, ready, io_process, p_clock);
        }
    }
    cpu_util = static_cast<float>(total_cpu_time)/static_cast<float>(p_clock.time); //calculate cpu utilazation

    std::cout << std::endl << std::setw(5) << std::setfill(' ') << "RR (" << tq << ") CPU Utilization: " << std::fixed << std::setprecision(2) << cpu_util*100 << "%" << std::endl; 
    schedulerReport(p_clock, process_complete);
    return total_cpu_time;
}