#include <iostream>
#include <vector>

#ifndef SRC_H
#define SRC_H

//Proccess Control Block double linked list implementation

struct ProcessNode {
    ProcessNode();
    
    unsigned int pid; // process ID

    unsigned int p_counter; //program counter

    bool running_state; //either ready or in I/O
    
    std::vector<unsigned int> burst_seq; 

    unsigned int tw, ttr, tr; //stats: wait time, turnaround, and response time

    ProcessNode* prev; //previous node/process
    ProcessNode* next; //next node/process
};

//Scheduler clock
struct Clock {
    Clock();
    unsigned int time;
    bool paused; 
};

//double linked list Queue

class Queue {
    public:
        Queue();
        ProcessNode* head; //tail of queue
        ProcessNode* tail; //head of queue
        unsigned int count; //number of items in queue

        void enqueue(ProcessNode*); //add new queue to end
        void enqueue(const ProcessNode&); //add existing queue to end

        void dequeue(); //remove from head

        ProcessNode* remove(unsigned int pid); //remove from any pid and return the next item in queue **Only for I/O to use**

        bool isEmpty()const; //true if queue is empty

        void display()const;

        void create(const std::vector<std::vector<unsigned int>>); //create queue given vector of processes

        void updateTimes(unsigned int time, ProcessNode* running_process); //update wait time and response time

        ProcessNode* getSJ(); //get shortest job
    private:
        ProcessNode* getNode(const std::vector<unsigned int> node_data); //create new node
        
        ProcessNode* allocateProcess(const ProcessNode& new_process); //allocate space for already formed process
        
        void pDisplay(const std::vector<unsigned int> burst)const; //display a given process' bursts
        
        int pidCounter; //counts amount element in queue
        
        void assignPid(ProcessNode* process); //assign process ID based on counter
};

void handleIO(Queue& io, Queue& ready, ProcessNode* io_process, Clock& p_clock);

void contextSwitchReport(const Clock& p_timer, const Queue& ready, const Queue& io, const ProcessNode* running_process);
void schedulerReport(const Clock& p_clock, std::vector<ProcessNode>& process_complete);
void FCFS(Queue& ready, Queue& io);
void SJF(Queue& ready, Queue& io);
int mRR(Queue& ready, Queue& io, unsigned int tq);

#endif