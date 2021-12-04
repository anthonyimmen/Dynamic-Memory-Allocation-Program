#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct memory {

  long size;       // the size of the process
  char pID[16];    // the id of the process 
  long head;
  long tail;
  int listLength;  // this acts as a length for memory, it is stored in slot memory[0]
  long totalSize;  // this is the dynamic total size in bytes of the memory array
  long fullLength;

}allMemory;

// declare all functions 
void firstFIT(struct memory *allMemory, struct memory process);
void bestFIT(struct memory *memory, struct memory process);
void nextFIT(struct memory *memory, struct memory process);
void worstFIT(struct memory *memory, struct memory process);
void release(struct memory *memory, struct memory process);
void listAssigned(struct memory *memory);
void listAvaliable(struct memory *memory);
void find(struct memory *memory, struct memory process);
void shiftLeft(struct memory *memory, int lastIdx);
void shiftRight(struct memory *memory, int startIdx);
void program(FILE *file, char *typeFit, long totalSize);

 void firstFIT(struct memory *allMemory, struct memory process) {

  int i = 0;
  int length = allMemory[0].listLength;

  if (process.size <= 0) { // if process size == 0 dont allocate
    printf("FAIL REQUEST %s %ld\n", process.pID, process.size);
    return;
  }
  
  // list is empty and we insert at front and shrink current empty slot
  if (length == 0 && process.size <= allMemory[0].totalSize) {
    allMemory[i].head = 0;
    allMemory[i].tail = process.size-1;
    allMemory[i].size = process.size;
    allMemory[0].listLength++;
    allMemory[0].totalSize += process.size;
    strcpy(allMemory[i].pID, process.pID); 
    printf("ALLOCATED %s %ld\n", process.pID, allMemory[i].head);
    return;
  }

    while (i < length && process.size <= allMemory[0].totalSize) {

      if (allMemory[i+1].head - allMemory[i].tail >= process.size) {
        // shift everything down so it will be in order then insert element into the new slot
        shiftRight(allMemory, i);
        i++; //increment i so we insert into the new open slot
        allMemory[i].head = allMemory[i-1].tail+1;
        allMemory[i].tail = allMemory[i].head+process.size-1;
        allMemory[i].size = process.size;
        allMemory[0].totalSize += process.size;
        strcpy(allMemory[i].pID, process.pID);
        printf("ALLOCATED %s %ld\n", process.pID, allMemory[i].head);
        return;
      }

    i++;

  }

  printf("FAIL REQUEST %s %ld\n", process.pID, process.size);
  return;

};

void nextFIT(struct memory *memory, struct memory process) {

};

void bestFIT(struct memory *allMemory, struct memory process) {
 
  int i = 0;
  int length = allMemory[0].listLength;

  if (process.size <= 0) { // if process size == 0 dont allocate
    printf("FAIL REQUEST %s %ld\n", process.pID, process.size);
    return;
  }
  
  // list is empty and we insert at front and shrink current empty slot
  if (length == 0 && process.size <= allMemory[0].totalSize) {
    allMemory[i].head = 0;
    allMemory[i].tail = process.size-1;
    allMemory[i].size = process.size;
    allMemory[0].listLength++;
    allMemory[0].totalSize -= process.size;
    strcpy(allMemory[i].pID, process.pID);
    printf("ALLOCATED %s %ld\n", process.pID, allMemory[i].head);
    return;
  }

  struct memory temp;
  long smallest = allMemory[0].fullLength;
  temp.head = 0;
  temp.tail = allMemory[0].fullLength-1;
  strcpy(temp.pID, process.pID);
  temp.size = process.size;

  int j = i;

  while (i < length && process.size <= allMemory[0].totalSize) { // loop to find best slot

    if (i == allMemory[0].listLength-1  && allMemory[0].fullLength-allMemory[i].tail < smallest) {
      smallest = allMemory[i+1].head - allMemory[i].tail;
      temp.head = allMemory[i].tail+1;
      temp.tail = allMemory[i+1].head-1;
      j = i;
      break;
    }

    else if (allMemory[i+1].head - allMemory[i].tail < smallest && allMemory[i+1].head - allMemory[i].tail >= process.size) {
      smallest = allMemory[i+1].head - allMemory[i].tail;
      temp.head = allMemory[i].tail+1;
      temp.tail = allMemory[i+1].head-1;
      j = i;
    }

    i++;

  }
    shiftRight(allMemory, j);
    j++;
    allMemory[j] = temp;
    allMemory[0].listLength++;
    allMemory[0].totalSize -= process.size;
    printf("ALLOCATED %s %ld\n", process.pID, allMemory[j].head);
    return;

};

void worstFIT(struct memory *memory, struct memory process) {
  
};

void release(struct memory *memory, struct memory process) {
  
  // releases the memory held the the specified process
  
  int i = 0;
  while (i < memory[0].listLength) {
    if (strcmp(memory[i].pID, process.pID) == 0) {
        long headCPY = memory[i].head;
        long sizeCPY = memory[i].size;
        memory[0].totalSize -= memory[i].size;
        shiftLeft(memory, i);
        printf("FREE %s %ld %ld\n", process.pID, sizeCPY, headCPY);
        return;
    }
    i++;
  }
  printf("FAIL RELEASE %s\n", process.pID);
}

void listAssigned(struct memory *memory) {

  int i = 0;
  if (memory[0].listLength == 0) { 
    printf("NONE\n");
    return;
  }

  while (i < memory[0].listLength) {
    printf("(%s, %ld, %ld) ", memory[i].pID,memory[i].size,memory[i].head);
    i++;
  }
  printf("\n");

}

void listAvaliable(struct memory *memory) {
  
  int i = 0;
  int flag = 0; // is set to 1 if we have open space
  if (memory[0].listLength==0 && memory[0].totalSize == memory[0].fullLength) { // if list is empty
        printf("(%ld, %ld) \n", memory[0].fullLength-memory[i].tail, memory[i].tail);
        return;
  }
  while (i < memory[0].listLength) {
    if (i == memory[0].listLength-1 && memory[0].totalSize != 0) { // last peice of open in array
      printf("(%ld, %ld) ", memory[0].fullLength-memory[i].tail-1, memory[i].tail+1);
      flag = 1;
    }
    else if (memory[i+1].head - memory[i].tail != 1 && memory[0].totalSize != 0) { // this will not be equal to 1 if the gap between is greater than 1, for any normal gap
      printf("(%ld, %ld) ", memory[i+1].head-memory[i].tail-1, memory[i].tail+1);
      flag = 1;
    }
    i++;
  }
  if (flag == 1) {
     printf("\n");
     return;
  }
  else {
    printf("FULL\n");
  }

}

void find(struct memory *memory, struct memory process) {

  int i = 0;
  while (i < memory[0].listLength) {
    if (strcmp(memory[i].pID, process.pID) == 0) {
      printf("(%s, %ld, %ld)\n", memory[i].pID,memory[i].size,memory[i].head);
      return;
    }
    i++;
  }

  printf("FAULT\n");

}

void shiftLeft(struct memory *memory, int lastIdx) { //used in release

  int i = memory[0].listLength-1;
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
  
  int i = memory[0].listLength-1;
  while(i >= startIdx) {
    memory[i+1] = memory[i];
    i--;
  }
  memory[0].listLength++;

};

void program(FILE *file, char *typeFit, long totalSize) {

  struct memory *allMemory = malloc(sizeof(allMemory)*totalSize+1);
  allMemory[0].listLength = 0; // we will use this to determine where we can insert into this array
  allMemory[0].totalSize = totalSize;
  allMemory[0].fullLength = totalSize;

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
          firstFIT(allMemory, process);
        }
        if (strcmp(typeFit, "BESTFIT") == 0) {
          bestFIT(allMemory, process);
        }
        if (strcmp(typeFit, "WORSTFIT") == 0) {
          worstFIT(allMemory, process);
        }
        if (strcmp(typeFit, "NEXTFIT") == 0) {
          nextFIT(allMemory, process);
        }
      }

      else if (strcmp(task, "RELEASE") == 0) {
        fscanf(file, "%s", task2);
        strcpy(process.pID, task2);
        release(allMemory, process);
      }

      else if (strcmp(task, "LIST") == 0) {
        fscanf(file, "%s", task2);
        if (strcmp(task2, "ASSIGNED") == 0) {
           listAssigned(allMemory); 
        }
        else {
           listAvaliable(allMemory);
        }
      } 

      else if (strcmp(task, "FIND") == 0){ // task == "FIND"
        fscanf(file, "%s", task2);
        strcpy(process.pID, task2);
        find(allMemory, process);
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