#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct memory {

  long size;       // the size of the process
  char pID[16];    // the id of the process 
  long head;
  long tail;

}allMemory;

struct memoryInfo {
  
  int listLength;  // this acts as a length for amount of process, it is stored in slot memory[0]
  long totalSize;  // this is the dynamic total size in bytes of the memory array
  long fullLength; // this is the max size of the memory array

}allMemoryInfo;

int lastIdx = -1;

// declare all functions 
void firstFIT(struct memory *allMemory, struct memory process, struct memoryInfo *allMemoryInfo);
void bestFIT(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo);
void nextFIT(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo);
void worstFIT(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo);
void release(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo);
void listAssigned(struct memory *memory, struct memoryInfo *memoryInfo);
void listAvaliable(struct memory *memory, struct memoryInfo *memoryInfo);
void find(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo);
void shiftLeft(struct memory *memory, int lastIdx, struct memoryInfo *memoryInfo);
void shiftRight(struct memory *memory, int startIdx, struct memoryInfo *memoryInfo);
void program(FILE *file, char *typeFit, long totalSize);

 void firstFIT(struct memory *allMemory, struct memory process, struct memoryInfo *allMemoryInfo) {

  int i = 0;
  int length = allMemoryInfo->listLength;

  if (process.size <= 0) { // if process size == 0 dont allocate
    printf("FAIL REQUEST %s %ld\n", process.pID, process.size);
    return;
  }
  
  // list is empty and we insert at front and shrink current empty slot
  if (length == 0 && process.size <= allMemoryInfo->fullLength - allMemoryInfo->totalSize) {
    allMemory[i].head = 0;
    allMemory[i].tail = process.size-1;
    allMemory[i].size = process.size;
    allMemoryInfo->listLength++;
    allMemoryInfo->totalSize += process.size;
    strcpy(allMemory[i].pID, process.pID); 
    printf("ALLOCATED %s %ld\n", process.pID, allMemory[i].head);
    return;
  }

    while (i < length && process.size <= allMemoryInfo->fullLength - allMemoryInfo->totalSize) {


      if (i == allMemoryInfo->listLength-1 && allMemoryInfo->fullLength - allMemory[i].tail-1 >= process.size) { // we are inserting into last open space
        shiftRight(allMemory, i, allMemoryInfo);
        i++; //increment i so we insert into the new open slot
        allMemory[i].head = allMemory[i-1].tail+1;
        allMemory[i].tail = allMemory[i].head+process.size-1;
        allMemory[i].size = process.size;
        allMemoryInfo->totalSize += process.size;
        strcpy(allMemory[i].pID, process.pID);
        printf("ALLOCATED %s %ld\n", process.pID, allMemory[i].head);
        return;

      }

      if (allMemory[i+1].head - allMemory[i].tail-1 >= process.size) {
        // shift everything down so it will be in order then insert element into the new slot
        shiftRight(allMemory, i, allMemoryInfo);
        i++; //increment i so we insert into the new open slot
        allMemory[i].head = allMemory[i-1].tail+1;
        allMemory[i].tail = allMemory[i].head+process.size-1;
        allMemory[i].size = process.size;
        allMemoryInfo->totalSize += process.size;
        strcpy(allMemory[i].pID, process.pID);
        printf("ALLOCATED %s %ld\n", process.pID, allMemory[i].head);
        return;

      }

    i++;

  }

  printf("FAIL REQUEST %s %ld\n", process.pID, process.size);
  return;

};

void nextFIT(struct memory *allMemory, struct memory process, struct memoryInfo *allMemoryInfo) {

};

void bestFIT(struct memory *allMemory, struct memory process, struct memoryInfo *allMemoryInfo) {
 
  int i = 0;
  int length = allMemoryInfo->listLength;
  int flag = 0;

  if (process.size <= 0) { // if process size == 0 dont allocate
    printf("FAIL REQUEST %s %ld\n", process.pID, process.size);
    return;
  }
  
  // list is empty and we insert at front 
  if (length == 0 && process.size <= allMemoryInfo->fullLength - allMemoryInfo->totalSize) {
    allMemory[i].head = 0;
    allMemory[i].tail = process.size-1;
    allMemory[i].size = process.size;
    allMemoryInfo->listLength++;
    allMemoryInfo->totalSize += process.size;
    strcpy(allMemory[i].pID, process.pID);
    printf("ALLOCATED %s %ld\n", process.pID, allMemory[i].head);
    return;
  }

  struct memory temp;
  long smallest = allMemoryInfo->fullLength;
  temp.head = 0;
  temp.tail = allMemoryInfo->fullLength-1;
  strcpy(temp.pID, process.pID);
  temp.size = process.size;

  int j = i;

  while (i < length && process.size <= allMemoryInfo->fullLength - allMemoryInfo->totalSize) { // loop to find best slot

    if(allMemory[i].head != 0 && i == 0) { //if the empty space is first in the memory
      
      if (allMemory[i].head < smallest) {
        smallest = allMemory[i].head;
        temp.head = 0;
        temp.tail = temp.head+process.size-1;
        j = i;
        flag=1;
      }

    }

    if (i == allMemoryInfo->listLength-1  && allMemoryInfo->fullLength-allMemory[i].tail-1 < smallest) { //if empty space is last
      smallest = allMemoryInfo->fullLength-1 - allMemory[i].tail-1;
      temp.head = allMemory[i].tail+1;
      temp.tail = temp.head+process.size-1;
      j = i;
      flag=1;
    }

    if (allMemory[i+1].head - allMemory[i].tail-1 < smallest && allMemory[i+1].head - allMemory[i].tail-1 >= process.size) { //if empty is anything in between
      smallest = allMemory[i+1].head - allMemory[i].tail-1;
      temp.head = allMemory[i].tail+1;
      temp.tail = temp.head+process.size-1;
      j = i;
      flag=1;
    }

    i++;

  }
    if (flag == 1) {
      shiftRight(allMemory, j, allMemoryInfo);
      j++;
      allMemory[j] = temp;
      allMemoryInfo->totalSize += process.size;
      printf("ALLOCATED %s %ld\n", process.pID, allMemory[j].head);
      return;
    }
    else {
      printf("FAIL REQUEST %s %ld\n", process.pID, process.size);
      return;
    }

};

void worstFIT(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo) {
  
};

void release(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo) {
  
  // releases the memory held the the specified process
  
  int i = 0;

  while (i < memoryInfo->listLength) {
    if (strcmp(memory[i].pID, process.pID) == 0) {
        long headCPY = memory[i].head;
        long sizeCPY = memory[i].size;
        memoryInfo->totalSize -= memory[i].size;
        shiftLeft(memory, i, memoryInfo);
        printf("FREE %s %ld %ld\n", process.pID, sizeCPY, headCPY);
        return;
    }
    i++;
  }
  printf("FAIL RELEASE %s\n", process.pID);

}

void listAssigned(struct memory *memory, struct memoryInfo *memoryInfo) {

  int i = 0;
  if (memoryInfo->listLength == 0) { 
    printf("NONE\n");
    return;
  }

  while (i < memoryInfo->listLength) {
    printf("(%s, %ld, %ld) ", memory[i].pID,memory[i].size,memory[i].head);
    i++;
  }
  printf("\n");

}

void listAvaliable(struct memory *memory, struct memoryInfo *memoryInfo) {
  
  int i = 0;

  if (memoryInfo->listLength==0) { // if list is empty
        printf("(%ld, %d) \n", memoryInfo->fullLength, 0);
        return;
  }

  if (memoryInfo->fullLength == memoryInfo->totalSize) { // if list is full
    printf("FULL\n");
    return;
  }

  while (i < memoryInfo->listLength) {

    if(memory[i].head != 0 && i == 0) { //if the empty space is first in the memory
      printf("(%ld, %d) ", memory[i].head, 0);
    }
    if (i == memoryInfo->listLength-1) { // last peice of open in array
      if (memoryInfo->fullLength-memory[i].tail-1 == 0) {
        i++;
        continue;
      }
      printf("(%ld, %ld) ", memoryInfo->fullLength-memory[i].tail-1, memory[i].tail+1);
      
    }
    else if (memory[i+1].head - memory[i].tail > 1) { // this will not be equal to 1 if the gap between is greater than 1, for any normal gap
      
      printf("(%ld, %ld) ", memory[i+1].head-memory[i].tail-1, memory[i].tail+1);
     
    }
    i++;

  }
     printf("\n");
     return;
}

void find(struct memory *memory, struct memory process, struct memoryInfo *memoryInfo) {

  int i = 0;
  while (i < memoryInfo->listLength) {
    if (strcmp(memory[i].pID, process.pID) == 0) {
      printf("(%s, %ld, %ld)\n", memory[i].pID,memory[i].size,memory[i].head);
      return;
    }
    i++;
  }

  printf("FAULT\n");

}

void shiftLeft(struct memory *memory, int lastIdx, struct memoryInfo *memoryInfo) { //used in release

  int i = memoryInfo->listLength-1;
  struct memory temp = memory[i];
  struct memory temp2;
  while (i > lastIdx) {
    temp2 = memory[i-1];
    memory[i-1] = temp;
    temp = temp2;
    i--;
  }
  memoryInfo->listLength--;

};

void shiftRight(struct memory *memory, int startIdx, struct memoryInfo *memoryInfo) { // used in request
  
  int i = memoryInfo->listLength-1;
  while(i > startIdx) {
    memory[i+1] = memory[i];
    i--;
  }
  memoryInfo->listLength++;

};

void program(FILE *file, char *typeFit, long totalSize) {

  struct memory *allMemory = malloc(sizeof(allMemory)*totalSize+1);
  struct memoryInfo *allMemoryInfo = malloc(sizeof(allMemoryInfo));
  allMemoryInfo->listLength = 0; // we will use this to determine where we can insert into this array
  allMemoryInfo->totalSize = 0;
  allMemoryInfo->fullLength = totalSize;

  // create temporary memSlot which will be used for current process
  char *task = malloc(sizeof(task)*16); // used for request, release, find, and list 
  char *task2 = malloc(sizeof(task2)*16); // used for list or for holding the pIDs name 

    // determine which fit 
    while (fscanf(file, "%s", task) == 1) { 

      struct memory process; 

      if (strcmp(task, "REQUEST") == 0) {
        fscanf(file, "%s", task2);
        fscanf(file, "%ld", &process.size); 
        strcpy(process.pID, task2);
        if (strcmp(typeFit, "FIRSTFIT") == 0) {
          firstFIT(allMemory, process, allMemoryInfo);
        }
        if (strcmp(typeFit, "BESTFIT") == 0) {
          bestFIT(allMemory, process, allMemoryInfo);
        }
        if (strcmp(typeFit, "WORSTFIT") == 0) {
          worstFIT(allMemory, process, allMemoryInfo);
        }
        if (strcmp(typeFit, "NEXTFIT") == 0) {
          nextFIT(allMemory, process, allMemoryInfo);
        }
      }

      else if (strcmp(task, "RELEASE") == 0) {
        fscanf(file, "%s", task2);
        strcpy(process.pID, task2);
        release(allMemory, process, allMemoryInfo);
      }

      else if (strcmp(task, "LIST") == 0) {
        fscanf(file, "%s", task2);
        if (strcmp(task2, "ASSIGNED") == 0) {
           listAssigned(allMemory, allMemoryInfo); 
        }
        else {
           listAvaliable(allMemory, allMemoryInfo);
        }
      } 

      else if (strcmp(task, "FIND") == 0){ // task == "FIND"
        fscanf(file, "%s", task2);
        strcpy(process.pID, task2);
        find(allMemory, process, allMemoryInfo);
      }
    
      else {
        //do nothing
      }
   }
};


int main(int argc, char** argv) {

  char *typeFit = argv[1];
  long totalMemSize = strtol(argv[2], NULL, 10); // size will be from 2^k, 4<=k<=30
  FILE *file = fopen(argv[3], "r");                       
  program(file, typeFit, totalMemSize);

  return 0;

}