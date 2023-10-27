#include <iostream>
#include <vector>

#include "structs.h"


int main() {

    std::vector<std::vector<int>> p_data = {
        {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4},
        {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8},
        {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6},
        {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3},
        {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4},
        {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8},
        {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10},
        {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6}
    };

    Clock p_timer;

    unsigned int time = 31;

    while(p_timer.time != time) {
        p_timer.time++;
    }

    std::cout << p_timer.time << std::endl << time; 

    return 0;
}