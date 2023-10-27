#include <iostream>
#include <vector>

#ifndef STRUCTS_H
#define STRUCTS_H

//Proccess Control Block double linked list implementation

struct ProcessNode {
    ProcessNode();
    
    unsigned int pid; // process ID

    unsigned int p_counter; //program counter

    bool ready_state; //either ready or in I/O
    
    std::vector<int> burst_seq; 

    unsigned int tw, ttr, tr; //stats: wait time, turnaround, and response time

    ProcessNode* prev; //previous node/process
    ProcessNode* next; //next node/process
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
        void remove(); //remove from any
        void moveToHead(); //move process to head of queue

        bool isEmpty(); //true if queue is empty

        void display();

        void create(const std::vector<std::vector<int>>); //create queue given vector of processes
    
    private:
        ProcessNode* getNode(const std::vector<int> node_data); //create new node
        
        ProcessNode* allocateProcess(const ProcessNode& new_process); //allocate space for already formed process
        
        void pDisplay(const std::vector<int> burst); //display a given process' bursts
        
        int pidCounter; //counts amount element in queue
        
        void assignPid(ProcessNode* process); //assign process ID based on counter


};
#endif