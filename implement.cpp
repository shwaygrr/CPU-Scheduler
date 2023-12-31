
#include "src.h"
 
 //constructors
ProcessNode::ProcessNode() {
    running_state = false;
    
    tw = 0, ttr = 0, tr = 0, p_counter = 0, pid = 0;
    
    prev = nullptr, next = nullptr;
}

Queue::Queue() {
    head = nullptr, tail = nullptr;
    count = 0, pidCounter = 0;
}

Clock::Clock(): paused(true), time(0) {}

//privates functions
ProcessNode* Queue::getNode(const std::vector<unsigned int> node_data) {
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

void Queue::pDisplay(std::vector<unsigned int> burst_list)const {
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
bool Queue::isEmpty()const {
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
    } else if(head->next == nullptr) {
        delete head;
        head = nullptr;
        tail = nullptr;
        count = 0; // Reset count to 0 since the queue is now empty
    } else {
        ProcessNode* temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp;
        count--; //decrement count
    }
    //display(); //display updated queue
}

void Queue::create(std::vector<std::vector<unsigned int>> p_data) {
    for(int i = 0; i < p_data.size(); i++) {
        ProcessNode* new_process = getNode(p_data[i]);
        enqueue(new_process);
    }
    //std::cout << std::endl<< "Queue matrix:" << std::endl;
    //display();
}

void Queue::display()const {
    ProcessNode* temp = head; //init traverser

    if (isEmpty()) { //if queue empty
        std::cout << "Empty queue";
    } else {
        while(temp) { //while Process exists
            //std::cout << "Process "<< temp->pid << " : ";
            //pDisplay(temp->burst_seq); //display row
            std::cout << "Process " << temp->pid << ": " << temp->burst_seq[temp->p_counter] << " time units left" << std::endl;
            temp = temp->next;
        }
    }
    std::cout << std::endl <<"**" << count << " Processes**" << std::endl;
}

ProcessNode* Queue::remove(unsigned int pid) {
    ProcessNode* temp = head; // initialize temp for traversal

    while (temp && temp->pid != pid) {
        temp = temp->next;
    }

    // Check if temp is the head
    if (temp == head) {
        head = temp->next;
        if (head) {
            head->prev = nullptr;
        }
    } else {
        temp->prev->next = temp->next; // node before temp points to node after temp, ignoring temp
        if (temp->next) {
            temp->next->prev = temp->prev;
        }
    }

    ProcessNode* nextNode = temp->next; // store the next node before deletion
    delete temp; // deallocate temp for possible new use
    count--;

    return nextNode;
}

void Queue::updateTimes(unsigned int time, ProcessNode* running_process) {
    if (running_process->p_counter == 0 && !running_process->running_state)  //if first time process running
        running_process->tr = time; //set response time

    ProcessNode* temp = head;

    while (temp) {
        if (temp != running_process) {
            temp->tw++;
        }
        temp = temp->next;
    }
}

ProcessNode* Queue::getSJ() {
    //**Handle if processes are equal
    if (isEmpty()) {
        return nullptr;
    } else if (!head->next) { //if only one item in Queue
        return head;
    } else {

        ProcessNode* shortest_job = head;
        ProcessNode* temp = head;
        
        while (temp && temp->next) {
            if (temp->next->burst_seq[temp->next->p_counter] < shortest_job->burst_seq[shortest_job->p_counter]) { //next job less than current shortest job
                shortest_job = temp->next; //set shortest job
            } else if (temp->next->burst_seq[temp->next->p_counter] < shortest_job->burst_seq[shortest_job->p_counter]) { //else the bursts equal
                if (temp->next->pid ) {

                } 
            }
            temp = temp->next; //traverse
        }

        return shortest_job;
    }
}

//handle I/O

void handleIO(Queue& io, Queue& ready, ProcessNode* io_process, Clock& p_clock) {
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