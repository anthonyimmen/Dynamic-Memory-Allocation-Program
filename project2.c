#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct memory {

  int memEnd; // where this peice of memory ends
  int size; // the size of the memory, which is not necessarily the same as the where the memory ends
  int pID; // the id of the process so we can find it later
  int isLastSlot; //checks if the slot is an ending peice of memory, 0 if false, 1 is true

}memSlot;

char* releaseMemory(struct memory *memoryBar, int currentPID, int totalSizeMemBar) {

  // release the memory held by the current process
  int i = 0;
  while (i < totalSizeMemBar) {
    if (memoryBar[i].pID == currentPID) {
      memoryBar[i].pID = 0;
      memoryBar[memoryBar[i].memEnd].pID = 0;
      return "FREE P%d %d %d", currentPID, memoryBar[i].size, i;
    }
    else {
      i = memoryBar[i].memEnd+1;
    }
  }
  // we were not able to free the pID because it does not exist in the memory bar
  return "FAIL RELEASE P%d", currentPID;
}

void listAvaliable(struct memory *memoryBar, int totalSizeMemBar) {

  // print locations of all avaliable memory in space separated pairs (n1, x1) (n2, x2) ...
  int i = 0;
  int flag = 0;
  while (i < totalSizeMemBar) {
    int counter = 0; // this is used to keep track of how much memory is open in that slot
    if (memoryBar[i].pID != 0) { //if the location contains memory then increment to the end of the memory
      i = memoryBar[i].memEnd+1;
    }
    else {
      while (memoryBar[i].pID == 0) {
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
    if (memoryBar[i].pID == 1) {
      printf("(%d,%d,%d) "), memoryBar[i].pID, memoryBar[i].size, i;
      i = memoryBar[i].memEnd+1;
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

void find(struct memory *memoryBar, int totalSizeMemBar, int currentPID) {

  // need to find the location of the current PID, return the name, size, and location, if unsuccessful print fault
  int i = 0;
  int flag = 0;
  while (i < totalSizeMemBar) {
    if (memoryBar[i].pID == currentPID) { // we found the matching pID
       printf("(%d,%d,%d) "), memoryBar[i].pID, memoryBar[i].size, i;
       i = memoryBar[i].memEnd+1;
       flag = 1;
       break;
    }
    else if (memoryBar[i].pID != 0) { // we found a pID but it does not match
       i = memoryBar[i].memEnd+1;
    }
    else { // empty space, so we must increment linearly through
      i++;
    }
  }
  if (flag == 0) { // the specified pID was never found
    printf("FAULT");
  }
}

char* findOpenSlotFIRST(struct memory *memoryBar, int currentSize, int currentPID, int totalSizeMemBar) {

  // start at the beginning of the memory bar, then recursively iterate through it, until we find an open slot, then check if its big enough
  int i = 0;
  while (i < totalSizeMemBar) { //using a sliding window approach with 'i' and 'counter' being my pointers
    int counter = 0;

    if (memoryBar[i].pID == 0) { // if the pIDs size == 0 that means we have any empty slot, even if we are at the last slot this will not return 0 because the last slot if full will still have an identifing pID

      // need to check if the slot is big enough to insert our new pID
      while (memoryBar[counter].pID == 0 || counter < currentSize) { //scans through until we find a slot that is occupied or a big enough slot for the new process
        ++counter;
      }
      if (counter == currentSize-1 && memoryBar[counter].pID == 0) { // the slot is at least big enough and is empty
        memoryBar[i].memEnd = i+currentSize-1;
        memoryBar[i].size = currentSize;
        memoryBar[i].pID = currentPID;
        memoryBar[i+currentSize-1].isLastSlot = 1;
        memoryBar[i+currentSize-1].pID = currentPID;
        return "ALLOCATED P%d %d", currentPID, i;
      } 

      else if (currentSize >= counter) { // if open slot was too small
        i = counter+i; // we can continue where we left off
      }
    }
      
    else { // if we haven't found an empty slot, then we increment to the slot aftwr the end of the current pID so we can start at an empty one
        i = memoryBar[i].memEnd+1;
    }
  }

  // we ran through entire list and did not insert the process
  return "FAIL REQUEST %d %d", currentPID, currentSize;
};

void firstFit(struct memory *memorybar, FILE *file, int totalMemSize) {
  
  // need to read in line by line on the file
  char *task = NULL; // used for request, release, find, and list 
  char *task2 = NULL; // used for list or for holding the pIDs name
  int pIDNumber = 0;
  int currentSize; // used for the size of the pID

  while (fscanf(file, "%ms", task) == 1) { // we read in everything from the txt file and execute the instructions as we read it in
    if (task == "REQUEST") {
      fscanf(file, "%ms", task2);
      fscanf(file, "%d", currentSize); 
      pIDNumber = task2[1];
      char *returned = findOpenSlotFIRST(memorybar, currentSize, pIDNumber, totalMemSize);
      printf("%s", returned);
    }
    else if (task == "RELEASE") {
      fscanf(file, "%ms", task2);
      pIDNumber = task2[1];
      char *returned = releaseMemory(memorybar, pIDNumber, totalMemSize);
      printf("%s", returned);
    }
    else if (task == "LIST") {
      fscanf(file, "%ms", task2);
      pIDNumber = task2[1];
      if (task2 == "ASSIGNED") {
        listAssigned(memorybar, totalMemSize);
      }
      else {
        listAvaliable(memorybar, totalMemSize);
      }
    }
    else { // task == "FIND"
      fscanf(file, "%ms", task2);
      pIDNumber = task2[1];
      find(memorybar, totalMemSize, pIDNumber);
    }
  }

};


void nextFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};

void bestFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};

void worstFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};


int program(FILE *file, char *typeFit, int totalMemSize) {

  struct memory *memBar = malloc(sizeof(memSlot)*totalMemSize+1);
  memset(memBar, 0, sizeof(memBar)); // initialize everything to 0

  if (typeFit == "FIRSTFIT") {
    firstFit(memBar, file, totalMemSize);
  }

  else if (typeFit == "NEXTFIT") {
    nextFit(memBar, file, totalMemSize);
  }

  else if (typeFit == "BESTFIT") {
    bestFit(memBar, file, totalMemSize);
  }

  else {
    worstFit(memBar, file, totalMemSize);
  }

  free(memBar); //need to free the memory from malloc
  return 0;
}

int main(int argc, char** argv) {

	int result;
  char *typeFit = argv[1];
  int totalMemSize = strtol(argv[2], NULL, 10); // this will be from 2^k, 4<=k<=30
  FILE *file = fopen(argv[3], "r"); // will get the file name after the previous 3 arguments
  result = program(file, typeFit, totalMemSize);
  return result;

}