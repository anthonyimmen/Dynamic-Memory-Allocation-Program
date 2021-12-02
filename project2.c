#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//TODO
// fix release memory
// fix memset
// maybe fix input?

struct memory {

  char memEnd[16]; // where this peice of memory ends
  char size[16]; // the size of the memory, which is not necessarily the same as the where the memory ends
  char pID[16]; // the id of the process so we can find it later

}memSlot;


void releaseMemory(struct memory *memoryBar, char currentPID[], int totalSizeMemBar) { //has issues, fix the else statement so i can increment with out skipping to end of process

  // release the memory held by the current process
  int i = 0;
  while (i < totalSizeMemBar) {
    if (strcmp(memoryBar[i].pID, currentPID) == 0) {
      strcpy(memoryBar[i].pID,"0");
      strcpy(memoryBar[atoi(memoryBar[i].memEnd)].pID, "0");
      printf("FREE %s %s %d", currentPID, memoryBar[i].size, i);
      return;
    }
    else {
      i = atoi(memoryBar[i].memEnd)+1;
    }
  }
  // we were not able to free the pID because it does not exist in the memory bar
      printf("FAIL RELEASE %s", currentPID);
}

void listAvaliable(struct memory *memoryBar, int totalSizeMemBar) {

  // print locations of all avaliable memory in space separated pairs (n1, x1) (n2, x2) ...
  int i = 0;
  int flag = 0;
  while (i < totalSizeMemBar) {
    int counter = 0; // this is used to keep track of how much memory is open in that slot
    if (strcmp(memoryBar[i].pID, "0") != 0) { //if the location contains memory then increment to the end of the memory
      i = atoi(memoryBar[i].memEnd)+1;
    }
    else {
      while (strcmp(memoryBar[i].pID, "0") == 0) {
        counter++;
      }
      printf("(%d,%d) ", counter, i);
      flag = 1;
    }
  }
  if (flag == 0) { //if no slots were open we output that the memory is FULL
    printf("FULL");
  }
}

void listAssigned(struct memory *memoryBar, int totalSizeMemBar) {

  // print a list of all the process labels in space separated triples (a1, n1, x1) (a2, n2, x2) ...
  int i = 0;
  int flag = 0;
  while (i < totalSizeMemBar) {
    if (strcmp(memoryBar[i].pID, "0") != 0) {
      printf("(%s,%s,%d) "), memoryBar[i].pID, memoryBar[i].size, i;
      i = atoi(memoryBar[i].memEnd)+1;
      flag = 1;
    }
    else {
      i++;
    }
  }
  if (flag == 0) { // if memory bar is completely empty we output NONE
    printf("NONE");
  }
}

void find(struct memory *memoryBar, int totalSizeMemBar, char currentPID[]) {

  // need to find the location of the current PID, return the name, size, and location, if unsuccessful print fault
  int i = 0;
  int flag = 0;
  while (i < totalSizeMemBar) {
    if (strcmp(memoryBar[i].pID, currentPID) == 0) { // we found the matching pID
       printf("(%s,%s,%d) "), memoryBar[i].pID, memoryBar[i].size, i;
       return;
    }
    else if (memoryBar[i].pID != '0') { // we found a pID but it does not match
       i = atoi(memoryBar[i].memEnd)+1;
    }
    else { // empty space, so we must increment linearly through
      i++;
    }
  }
    printf("FAULT"); // the specified pID was never found
  
}

void findOpenSlotFIRST(struct memory *memoryBar, int currentSize, char currentPID[], int totalSizeMemBar) {

  // start at the beginning of the memory bar, then recursively iterate through it, until we find an open slot, then check if its big enough
  int i = 0;
  while (i < totalSizeMemBar) { //using a sliding window approach with 'i' and 'counter' being my pointers
    int counter = 0;
    

    if (memoryBar[i].pID == '0') { // if the pIDs size == 0 that means we have any empty slot, even if we are at the last slot this will not return 0 because the last slot if full will still have an identifing pID

      // need to check if the slot is big enough to insert our new pID
      while (memoryBar[counter].pID == '0' || counter < currentSize) { //scans through until we find a slot that is occupied or a big enough slot for the new process
        ++counter;
      }
      if (counter == currentSize-1 && memoryBar[counter].pID[0] == '0') { // the slot is at least big enough and is empty
        sprintf(memoryBar[i].memEnd, "%d", i+currentSize-1);
        sprintf(memoryBar[i].size, "%d", currentSize);
        strcpy(memoryBar[i].pID, currentPID);
        strcpy(memoryBar[i+currentSize-1].pID, currentPID);
        printf("ALLOCATED %s %d", currentPID, i);
        return;
      } 

      else if (currentSize >= counter) { // if open slot was too small
        i = counter+i; // we can continue where we left off
      }
    }
      
    else { // if we haven't found an empty slot, then we increment to the slot aftwr the end of the current pID so we can start at an empty one
        i = atoi(memoryBar[i].memEnd)+1;
    }
  }
  // we ran through entire list and did not insert the process
  printf("FAIL REQUEST %s %d", currentPID, currentSize);
};

void firstFit(struct memory *memorybar, FILE *file, int totalMemSize) {
  
  // need to read in line by line on the file
  char *task = malloc(sizeof(task)*16); // used for request, release, find, and list 
  char *task2 = malloc(sizeof(task2)*16); // used for list or for holding the pIDs name 
  int currentSize = 0; // used for the size of the pID

  while (fscanf(file, "%s", task) == 1) { // we read in everything from the txt file and execute the instructions as we read it in
    if (strcmp(task, "REQUEST") == 0) {
      fscanf(file, "%s", task2);
      fscanf(file, "%d", &currentSize); 
      findOpenSlotFIRST(memorybar, currentSize, task2, totalMemSize);
    }

    else if (strcmp(task, "RELEASE") == 0) {
      fscanf(file, "%s", task2);
      releaseMemory(memorybar, task2, totalMemSize);
    }

    else if (strcmp(task, "LIST") == 0) {
      fscanf(file, "%s", task2);
      if (strcmp(task2, "ASSIGNED") == 0) {
        listAssigned(memorybar, totalMemSize);
      }
      else {
        listAvaliable(memorybar, totalMemSize);
      }
    }

    else { // task == "FIND"
      fscanf(file, "%s", task2);
      find(memorybar, totalMemSize, task2);
    }

  }

};


void nextFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};

void bestFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};

void worstFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};


int program(FILE *file, char *typeFit, char *totalMemSize) {

  int totalMemSizeINT = strtol(totalMemSize, NULL, 10)+1;

  struct memory *memBar = malloc(sizeof(memSlot)*totalMemSizeINT);
  memset(memBar, "0", sizeof(*memBar)); // initialize everything to 0
 
  if (strcmp(typeFit, "FIRSTFIT") == 0) {
    printf("at first fit ");
    //firstFit(memBar, file, totalMemSizeINT);
  }

  else if (strcmp(typeFit, "NEXTFIT") == 0) {
    nextFit(memBar, file, totalMemSizeINT);
  }

  else if (strcmp(typeFit, "BESTFIT") == 0) {
    bestFit(memBar, file, totalMemSizeINT);
  }

  else {
    worstFit(memBar, file, totalMemSizeINT);
  }

  free(memBar); //need to free the memory from malloc
  
  return 0;
}

int main(int argc, char** argv) {

	int result;
  char *typeFit = argv[1];
  char *totalMemSize = argv[2]; // this will be from 2^k, 4<=k<=30
  FILE *file = fopen(argv[3], "r"); // will get the file name after the previous 3 arguments
  printf("%s%d ", typeFit, totalMemSize);
  result = program(file, typeFit, totalMemSize);
  return result;

}