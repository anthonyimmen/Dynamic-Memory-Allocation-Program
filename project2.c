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

void findOpenSlotFIRST(struct memory *memoryBar, int currentSize, int currentPID, int totalSize) {

  // start at the beginning of the memory bar, then recursively iterate through it, until we find an open slot, then check if it big enough
  int i = 0;
  while (i < totalSize) {
    int counter = i;

    if (memoryBar[i].pID == 0) { // if the pIDs size == 0 that means we have any empty slot, even if we are at the last slot this will not return 0 because the last slot if full will still have an identifing pID

      // need to check if the slot is big enough to insert our new pID
      while (memoryBar[i].pID == 0 || counter < currentSize) { //scans through until we find a slot that is occupied or a big enough slot
        ++counter;
      }
      if (counter <= currentSize && memoryBar[i].size == 0) { //TODO: take a look at this to make sure it works
        memoryBar[i].memEnd = i+currentSize;
        memoryBar[i].size = currentSize;
        memoryBar[i].pID = currentPID;
        memoryBar[i+currentSize].isLastSlot = 1;
        memoryBar[i+currentSize].pID = currentPID;
      } 

    }
      
    else { // if we haven't found an empty slot, then we increment to the end of the current pID
        i = memoryBar[i].memEnd+1;
    }
  }
};

void firstFit(struct memory *memorybar, FILE *file, int totalMemSize) {
  
  // need to read in line by line on the file

  char *task = NULL; // used for request, release, find, and list 
  char *task2 = NULL; // used for list or for holding the pIDs name
  int currentSize; // used for the size of the pID

  while (fscanf(file, "%ms", task) == 1) {
    if (task == "REQUEST") {
      fscanf(file, "%ms", task2);
      fscanf(file, "%d", currentSize);
    }
    else if (task == "RELEASE") {
      fscanf(file, "%ms", task2);
    }
    else if (task == "LIST") {
      fscanf(file, "%ms", task2);
    }
    else {
      fscanf(file, "%ms", task2);
    }
  }

};


void nextFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};

void bestFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};

void worstFit(struct memory *memorybar, FILE *file, int totalMemSize) {

};


int program(FILE *file) {

  char *typeFit = NULL;
  int totalMemSize = 0; // this will be from 2^k, 4<=k<=30

  fscanf(stdin, "%ms", typeFit);
  fscanf(stdin, "%d", totalMemSize);

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

  
  return 0;
}

int main(int argc, char** argv) {

	int result;
  FILE *file = fopen(argv[4], "r"); // will get the file name after the previous 3 arguments
	result = program(file);
	return result;

}