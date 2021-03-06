# Dynamic Memory Allocation Program

In order to run the code you need to download all of the files into a folder. Open the folder in terminal, enter make clean, make all, then './project2 <fit-type> <total-memory> test.txt'. Replace <fit-type> with FIRSTFIT, BESTFIT, NEXTFIT, or WORSTFIT.

Libraries used: <stdio.h> <stdlib.h> <string.h> <stdbool.h>

## Code Summary:

This project's goal is to simulate four memory allocation algorithms. Based off of the input given, the program will allocate, release, list, and find the process ids specifed. The way I designed this project to work was using a structure array to hold the process ids name, head, tail, and size. I also had an additional structure that would hold certain information about our structure array that was necessary for looking at certain information. To determine the places where to insert and remove processes in our allocated memory, I would perform small calculations on the head's and tail's of each process which would give me access to the empty slots spaced between the processes. Using these techniques together, I was able to get FIRSTFIT working within my own personal testing, but not within Gradescope's testing. I believed I had covered all the possible cases for FIRSTFIT, but I must have overlooked a few. BESTFIT is fully working within my own testing, but I was able to get it working almost completely in the end. NEXTFIT and WORSTFIT have been created within the program and both will perform as intended.

## Brief Description of Functions: 

firstFIT() - This function is what places our memory within the first avaliable slot. I check if the process size is <= 0 then we do not insert it into our array. If the list is empty, we insert it into the first avaliable spot, being at index 0 of the array. We then iterated over the rest of the slots within the memory, then insert the memory into the first avaliable spot that will fit the process.

bestFIT() - Has similar implementation to firstFIT, but I had an intital while loop which iterated through the entire memory array to find the smallest empty slot which still fit the process.

nextFIT() - Works by having two while loops, the first one runs through where the lastIdxNEXT counter leaves off to up until the end of the memory. If our flag is never checked during the first loop meaning there was not any empty space, that means that we need to run through the beginning of the memory up until where our lastIdxNEXT counter was at.

worstFIT() - Very similar to best fit, it is essentially the same except that we have greatest instead of smallest and the <'s are changed to >'s

release() - Uses a while loop to iterate over all of the process IDs in the memory until we find the one that we are looking for. Once we find it, we use shiftLeft in order to pop out the specifed process. If not found we print that we failed the release.

listAssigned() - Uses a while loop to iterate over the memory array and outputs each processes ID, size, and its location. If the 

listAvaliable() - Checks if the list is full, if it is we output FULL. Checks if the list is empty, then outputs the size of the array. Then, it uses a while loop to iterate over all the process in the memory. There are three specified checks within the while loop being, if the empty space is first in the memory, if its the last peice open, and then if its anywhere in between. 

find() - Uses a while loop to iterate over all of the process IDs in the memory until we find the one that we are looking for. If found we print its location, if not we print FAULT.

shiftLeft() - Helper function to release. This function would shift all our processes down to essentially cover up the process that we are wanting to pop out. I used two temp variables in order to accomplish this. 

shiftRight() - Helper function to all of the fitting algorithms. This would shift all our process down so we could have an empty slot open to insert in the new process. This was easier to implement than shiftLeft because I did not need any temp variables.

program() - This function is where we initalize the all memory and allocated its space. This is also where we call the specifed fitting algorithms, release, list, and find in order to perfrom the specifed decisions from the test.txt file. 

main() - this function is used to get the file name, the fit type, and the total memory from the command line arguments, main() is also used for calling the program function.



