/*
 * Author: Nicholas Christensen
 * Date: 11/18/2017
 * File Purpose:
 * 		This file contains the main program. The main program contains
 * 		the actions of the mother eagle.
 * ------------------------------------------------------------------  */
 
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstdlib>

	  int numberMeals;
	  int numberPots;
	  int numberBabies;
	  int eaglesWaiting;
	  int foodPot;
	  int eaglesEating;
	  int motherSignaled;
	  int timesRefilled;
	  int wokeBabyEagleNumber;

 int main (int argc, char *argv[])
 {
	 eaglesWaiting = 0;
	 foodPot = 0;
	 eaglesEating = 0;
	 motherSignaled = 0;
	 timesRefilled = 0;
	  
	 
	 //Read in the arguments
	 numberPots = atoi(argv[1]);
	 numberBabies = atoi(argv[2]);
	 numberMeals = atoi(argv[3]);
	 
	 cout << "MAIN: There are " << numberBabies << " baby eagles, " << numberPots << " feeding pots, and " << numberMeals << " meals" << endl << "MAIN: Game Starts!!!!!" << endl;
	 
	 //Now we initiate the baby eagles
	 thread * run[numberBabies];
	 int i;
	 for (i = 0; i < numberBabies; i++)
	 {
		 run[i] = new thread(i + 1);
		 run[i] -> Begin();
	 }
	 
	 char output [10000];
	 char * outputPointer = output;
	 char buffer [100];
	 
	 sprintf(output, "Mother eagle started.\n");
	 write(1, outputPointer, strlen(outputPointer));
	 memset(outputPointer, '\0', sizeof(output));
	
	 //The following code is the mother eagle's pattern
	 while (1)
	 {
		 sprintf(output, "Mother eagle takes a nap.\n");
		 write(1, outputPointer, strlen(output));
		 memset(outputPointer, '\0', sizeof(output));
		 goto_sleep();		//The mother sleeps until a baby eagle wakes it up
		 sprintf(output, "Mother eagle is awoke by baby eagle %d and starts preparing food.\n", wokeBabyEagleNumber);
		 write(1, outputPointer, strlen(output));
		 memset(outputPointer, '\0', sizeof(output));
		 
		 //sleep(rand() / 699000000 + 1);			//Once woken up, the mother hunts for food, symbolized by this delay
		 food_ready();		//Then the refills the pots and signals that there is food again
		 //sleep(rand() / 699000000 + 1);			//Then the mother takes some time to fall back asleep, symbolized by this delay
	 }
 }
