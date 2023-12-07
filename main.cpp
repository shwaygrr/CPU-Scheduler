#include <iostream>
#include <vector>
#include <iomanip>

#include "src.h"



int main() {
    //****input of 2-D matrix here in the form ex. (cpu, io, cpu,...,cpu)***
    std::vector<std::vector<unsigned int>> p_data = {

    };

    Queue ready, io;

    ready.create(p_data);

    FCFS(ready, io);
    //SJF(ready, io);
    //mRR(ready, io, 5);

    return 0;
}