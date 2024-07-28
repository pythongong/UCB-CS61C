#include <mpi.h>
#include <time.h>

#include "coordinator.h"

#define READY 0
#define NEW_TASK 1
#define TERMINATE -1

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error: not enough arguments\n");
    printf("Usage: %s [path_to_task_list]\n", argv[0]);
    return -1;
  }

  // initialize
  MPI_Init(&argc, &argv); 

  // Read and parse task list file
  int num_tasks;
  task_t **tasks;
  
  if (read_tasks(argv[1], &num_tasks, &tasks)) {
    printf("Error reading task list from %s\n", argv[1]);
    return -1;
  }
  
  // get process ID of this process and total number of processes
  int procID, totalProcs;
  MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &procID);
  if (procID == 0) {
    // manager node code 
    int nextTask = 0;
    MPI_Status status;
    int32_t message;
    while (nextTask < num_tasks)
    {
      // wait for a message from any worker
      MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      // assign next task
      int sourceProc = status.MPI_SOURCE;
      message = nextTask;
      MPI_Send(&message, 1, MPI_INT32_T, sourceProc, 0, MPI_COMM_WORLD);
      nextTask++;
    }

    // wait for all processes to finish
    for (int i = 0; i < totalProcs - 1; i++) {
      MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      int sourceProc = status.MPI_SOURCE;
      message = TERMINATE;
      MPI_Send(&message, 1, MPI_INT32_T, sourceProc, 0, MPI_COMM_WORLD);
    }
    
  } else {
    // worker node code 
    int32_t message;
    while (true) {
      message = READY;
      // request more work
      MPI_Send(&message, 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD);
      // receive message from manager
      MPI_Recv(&message, 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (message == TERMINATE)
      {
        break;
      }
 
      if (execute_task(tasks[message])) {
        printf("Task %d failed\n", message);
        return -1;
      }
      free(tasks[message]->path);
      free(tasks[message]);
    }
  }

  free(tasks);

  // clean up
  MPI_Finalize();
}
