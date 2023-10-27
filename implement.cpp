
#include "structs.h"
 
 //constructors
ProcessNode::ProcessNode() {
    ready_state = false;
    
    tw = 0, ttr = 0, tr = 0, p_counter = 0, pid = 0;
    
    prev = nullptr, next = nullptr;
}

Queue::Queue() {
    head = nullptr, tail = nullptr;
    count = 0, pidCounter = 0;
}




//privates functions
ProcessNode* Queue::getNode(const std::vector<int> node_data) {
    ProcessNode* new_node = new ProcessNode(); //allocate space for new node and point to it
    
    new_node->burst_seq = node_data; //input burst list

    new_node->next = nullptr; // node points to nothing
    
    return new_node; 
}

ProcessNode* Queue::allocateProcess(const ProcessNode& process) {
    ProcessNode* new_process = new ProcessNode(); //allocate space for new node and point to it
    
    *new_process = process; //input burst list

    new_process->next = nullptr; // node points to nothing

    return new_process;
}

void Queue::pDisplay(std::vector<int> burst_list) {
    for (int i = 0; i < burst_list.size(); i++) {
        std::cout << burst_list[i] << " ";
    }
    std::cout << std::endl;
}

void Queue::assignPid(ProcessNode* process) {
    if (!process->pid)
        process->pid = ++pidCounter;
}




//public functions
bool Queue::isEmpty() {
    return !head; //true if head == nullptr
}

void Queue::enqueue(const ProcessNode &process) {
    if(isEmpty()) { //if queue is empty
        head =  allocateProcess(process); //add to head
    } else {
        ProcessNode* temp = head; //traverser

        while(temp->next) {
            temp = temp->next;
        }
        temp->next = allocateProcess(process); //adds to end of queue
        tail = temp->next; //set tail
        temp->next->prev = temp;    
    }
    count++; //increment count
}

void Queue::enqueue(ProcessNode* process) {
    //used by createQueue
    assignPid(process); //assign process ID if theres not already one
    if (isEmpty()) { //if queue empty
        head = process;
    } else {
        ProcessNode* temp = head; //incrementer
        
        while(temp->next) { //while next process exists
            temp = temp->next; //move to next process in queue
        }
        temp->next = process; //add to end of queue
        tail = temp->next; //set tail
        temp->next->next = nullptr;
        process->prev = temp;
    }
    //display(); //display updated queue
    count++; //increment count
}

void Queue::dequeue() {
    if(isEmpty()) {//if queue empty
        std::cout <<std::endl << "Errot: Queue is empty - Cannot dequeue" << std::endl;
        return;
    } else {
        ProcessNode* temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp;
        count--; //decrement count
    }
    //display(); //display updated queue
}

void Queue::create(std::vector<std::vector<int>> p_data) {
    for(int i = 0; i < p_data.size(); i++) {
        ProcessNode* new_process = getNode(p_data[i]);
        enqueue(new_process);
    }
    std::cout << std::endl<< "Queue matrix:" << std::endl;
    //display();
}

void Queue::display() {
    ProcessNode* temp = head; //init traverser

    if (isEmpty()) { //if queue empty
        std::cout << "Empty queue";
    } else {
        while(temp) { //while Process exists
            std::cout << "Process "<< temp->pid << " : ";
            pDisplay(temp->burst_seq); //display row
            temp = temp->next;
        }
    }
    std::cout << std::endl << count << " Processes" << std::endl;
}