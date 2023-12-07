# CPU Scheduler Simulation Project

## Overview

This project involves the implementation and simulation of three CPU scheduling algorithms: FCFS non-preemptive, SJF non-preemptive, and Round Robin. The primary objective is to gain hands-on experience in OS scheduling through simulation programming.

## Project Details

### Implemented Scheduling Algorithms

1. **FCFS non-preemptive:**
2. **SJF non-preemptive:**
3. **Round Robin (non-preemptive):**

### Assumptions
1. Processes arrrive at time 0.
2. Processes dont wait I/O devices.
3. After /O event, a process moves back to the ready queue.


### Running the Simulation
To run the simulation, follow these steps:

1. Open the `main.cpp` file in the `src` directory.

2. Locate the `p_data` variable and input your burst sequence for the desired process. For example:

    ```cpp
    // main.cpp
    int p_data[] = {
                    {1, 2, 3, 5},
                    {2, 4, 5, 7},
                    {.2, 6, 7},
                    ...
                  };
    ```

3. Save the `main.cpp` file.

4. Open the `tasks.json` file in your Visual Studio Code workspace (`.vscode` directory).

5. Ensure that the `args` property within the `"tasks"` array looks like the following:
     ```json
          "args": [
              "-fdiagnostics-color=always",
              "-g",
              "${workspaceFolder}/*.cpp",
              "-o",
              "fileDirname{fileDirname}\\{fileBasenameNoExtension}.exe"
          ],
     ```

6. Save the `tasks.json` file.
7. Build and Run

### Evaluation and Results
![FCFS](https://github.com/shwaygrr/CPU-Scheduler/assets/122495108/ec38ea62-b193-4a0b-887e-661d621e47df) ![SJF](https://github.com/shwaygrr/CPU-Scheduler/assets/122495108/fa1d5f2f-b500-4013-ab5e-1d862a70ef21) ![RR](https://github.com/shwaygrr/CPU-Scheduler/assets/122495108/38c1d189-b870-4a5c-8de7-b7cb5d4792c6)
