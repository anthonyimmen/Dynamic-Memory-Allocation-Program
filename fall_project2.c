#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct memory {

  long unsigned size;  // the size of the process
  char pID[16];        // the id of the process 
  long unsigned head;
  long unsigned tail;
  int listLength;        // this acts as a length for memory, it is stored in slot memory[0]
  long unsigned totalSize;

}allMemory;

// declare all functions 
void firstFIT(struct memory *allMemory, struct memory process);
void fillNEXT(struct memory *memory, struct memory process);
void fillBEST(struct memory *memory, struct memory process);
void fillWORST(struct memory *memory, struct memory process);
void release(struct memory *memory, struct memory process);
void shiftLeft(struct memory *memory, int lastIdx);
void shiftRight(struct memory *memory, int startIdx);
void program(FILE *file, char *typeFit, long unsigned totalSize);

 void firstFIT(struct memory *allMemory, struct memory process) {

  int i = 0;
  int length = allMemory[0].listLength;
  
  // list is empty and we insert at front and shrink current empty slot
  if (length == 0 && process.size <= allMemory[0].totalSize) {
    allMemory[i].head = 0;
    allMemory[i].tail = process.size-1;
    allMemory[0].listLength++;
    allMemory[0].totalSize -= process.size;
    strcpy(allMemory[i].pID, process.pID);
    printf("ALLOCATED %s %lu\n", process.pID, allMemory[i].head);
    return;
  }

    while (i < length && process.size <= allMemory[0].totalSize) {

      if (allMemory[i+1].head - allMemory[i].tail >= process.size) {
        // shift everything down so it will be in order then insert element into the new slot
        shiftRight(allMemory, i);
        i++; //increment i so we insert into the new open slot
        allMemory[i].head = allMemory[i-1].tail+1;
        allMemory[i].tail = allMemory[i].head+process.size-1;
        allMemory[0].totalSize -= process.size;
        strcpy(allMemory[i].pID, process.pID);
        printf("ALLOCATED %s %lu\n", process.pID, allMemory[i].head);
        return;
      }

    i++;

  }

  printf("FAIL REQUEST %s %lu\n", process.pID, process.size);
  return;

};

void fillNEXT(struct memory *memory, struct memory process) {

};

void fillBEST(struct memory *memory, struct memory process) {
  
};

void fillWORST(struct memory *memory, struct memory process) {
  
};

void release(struct memory *memory, struct memory process) {
  
  // releases the memory held the the specified process
  
  int i = 0;
  while (i < memory[0].listLength) {
    if (strcmp(memory[i].pID, process.pID) == 0) {
        long unsigned head = memory[i].head;
        memory[0].totalSize += process.size;
        shiftLeft(memory, i);
        printf("FREE %s %lu %lu\n", process.pID, process.size, head);
        return;
    }
    i++;
  }
  printf("FAIL RELEASE %s\n", process.pID);
}

void listAssigned(struct memory *memory, struct memory process) {

}

void shiftLeft(struct memory *memory, int lastIdx) { //used in release

  int i = memory[0].listLength;
  struct memory temp = memory[i];
  struct memory temp2;
  while (i > lastIdx) {
    temp2 = memory[i-1];
    memory[i-1] = temp;
    temp = temp2;
    i--;
  }
  memory[0].listLength--;

};

void shiftRight(struct memory *memory, int startIdx) { // used in request
  
  int i = memory[0].listLength;
  while(i > startIdx) {
    memory[i+1] = memory[i];
    i--;
  }
  memory[0].listLength++;
  
};

void program(FILE *file, char *typeFit, long unsigned totalSize) {

  struct memory *allMemory = malloc(sizeof(allMemory)*totalSize+1);
  allMemory[0].listLength = 0; // we will use this to determine where we can insert into this array
  allMemory[0].totalSize = totalSize;

  // create temporary memSlot which will be used for current process
  char *task = malloc(sizeof(task)*16); // used for request, release, find, and list 
  char *task2 = malloc(sizeof(task2)*16); // used for list or for holding the pIDs name 

  // determine which fit
  if (strcmp(typeFit, "FIRSTFIT") == 0) {
     
    while (fscanf(file, "%s", task) == 1) { 

      struct memory process; 

      if (strcmp(task, "REQUEST") == 0) {
        fscanf(file, "%s", task2);
        fscanf(file, "%ld", &process.size); 
        strcpy(process.pID, task2);
        printf("%s %s %lu totalSize: %lu\n", task,task2, process.size, allMemory[0].totalSize);
        firstFIT(allMemory, process);
      }

      else if (strcmp(task, "RELEASE") == 0) {
        fscanf(file, "%s", task2);
        strcpy(process.pID, task2);
        printf("%s %s totalSize:%lu\n", task,task2, allMemory[0].totalSize);
        release(allMemory, process);
      }

      else if (strcmp(task, "LIST") == 0) {
        fscanf(file, "%s", task2);
        if (strcmp(task2, "ASSIGNED") == 0) {
           printf("%s %s totalSize:%lu\n", task,task2,allMemory[0].totalSize);
           //listAssigned(allMemory);
        }
        else {
          printf("%s %s totalSize:%lu\n", task,task2,allMemory[0].totalSize);
           //listAvaliable(allMemory);
        }
      } 

      else { // task == "FIND"
        fscanf(file, "%s", task2);
        strcpy(process.pID, task2);
        printf("%s %s totalSize:%lu\n", task,task2,allMemory[0].totalSize);
        //find(allMemory, process);
      }

    }
 
  }
  else if (strcmp(typeFit, "NEXTFIT") == 0) {
    //nextFIT(allMemory, file);
  }

  else if (strcmp(typeFit, "BESTFIT") == 0) {
    //bestFIT(allMemory, file);
  }

  else {
    //worstFIT(allMemory, file);
  }

};


int main(int argc, char** argv) {

  char *typeFit = argv[1];
  long unsigned totalMemSize = strtol(argv[2], NULL, 10); // size will be from 2^k, 4<=k<=30
  FILE *file = fopen(argv[3], "r");                       
  program(file, typeFit, totalMemSize);

  return 0;

}