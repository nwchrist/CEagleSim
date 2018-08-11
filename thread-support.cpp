/*
 * Author: Nicholas Christensen
 * Date: 11/18/2017
 * File Purpose:
 *		This file contains the supporting functions for the program.
 * ------------------------------------------------------------------  */
 
 #include "thread.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 /* -----------------------------------------------------------------
  * FUNCTION: ready_to_eat
  * 	This function is called when a baby eagle wants to eat.
  * PARAMETER USAGE:
  * 	This function currently has no parameters.
  * FUNCTION CALLS:
  * 	This function currently calls no other functions.
  * ----------------------------------------------------------------- */
  
 int ready_to_eat(int eagleNumber)
 {
	 safety.Lock();		//First we lock the safety mutex
	 if (foodPot > 0)	//Check if there is a unoccupied pot with food in it
	 {
		 safety.Unlock();
		 foodQueue.Wait();		//We enter the foodQueue, though we will not have to wait in it as the current counter will be > 0
		 safety.Lock();
		 if (eaglesEating == 0)		//If there is an unoccupied pot, then we need to check if we are the first eagle to start eating
		 {
			 //eaglesEating++;
			 someoneEating.Wait();		//If we are, we indicate so by Waiting on the someoneEating semaphore
		 }
		 int resultPot = foodPot;		//We automatically eat out of the highest number full unoccupied pot
		 foodPot--;						//Then we decrease the number of full unoccupied pots
		 eaglesEating++;				//And increase the number of eagles that are currently eating
		 safety.Unlock();				//Then we can unlock the safety mutex
		 return resultPot;				//And return the result of what pot we are eating out of
		 
	 } else {						//If there is not a pot with food in it, the eagle must check if it should signal the mother
		 if (motherSignaled == 0)	//We need to check if the mother has already been signaled
		 {
			 motherSignaled = 1;	//If she has not, then we signal that we are going to do so, so that no other eagles try to also signal her
			 
			 char output [10000];
			 char * outputPointer = output;
			 char buffer [100];
			 char * bufferPointer = buffer;
			 char space = ' ';
			 char * spaces = &space;
			 int i;
			 for (i = 0; i < eagleNumber; i++)
			 {
				 strcat(outputPointer, spaces);
			 }
			 
			 motherSleeping.Wait();		//We need to make sure that the mother is sleeping before we can wake her up again
			
			 sprintf(buffer, "Baby eagle %d sees all feeding pots are empty and wakes up the mother.\n", eagleNumber);
			 strcat(outputPointer, buffer);
			 write(1, outputPointer, strlen(output));
			 memset(outputPointer, '\0', sizeof(output));
			 memset(bufferPointer, '\0', sizeof(buffer));
			 
			 wokeBabyEagleNumber = eagleNumber;		//We record the number of which baby eagle woke up the mother
			 motherNeeded.Signal();					//And then signal for the mother to wake up
		 }
		 
		 eaglesWaiting++;		//If we cannot eat yet, and the mother has already been signaled, then we must wait until the mother refills the pots
		 
		 safety.Unlock();
		 foodQueue.Wait();		//We wait in the foodQueue semaphore
		 safety.Lock();
		 
		 if (eaglesEating == 0)		//Once we are released to eat, we first must check if we are the first eagle to start eating
		 {
			 someoneEating.Wait();	//If we are then we indicate that someone is eating with the someoneEating semaphore
		 }
		 
		 int resultPot = foodPot;	//We record what pot we are eating from
		 foodPot--;					//We decrease the number of unoccupied pots with food
		 eaglesWaiting--;			//And decrease the number of eagles waiting to eat
		 eaglesEating++;			//Then increase the number of eagles currently eating
		 safety.Unlock();
		 return resultPot;			//And return the number pot that we are eating from
	 }
 }
 
  /* -----------------------------------------------------------------
  * FUNCTION: finish_eating
  * 	This function is called when a baby eagle finishes eating.
  * PARAMETER USAGE:
  * 	This function currently has no parameters.
  * FUNCTION CALLS:
  * 	This function currently calls no other functions.
  * ----------------------------------------------------------------- */
  void finish_eating(int eagleNumber)
  {
	 safety.Lock();
	 eaglesEating--;	//First we decrease the number of eagles that are currently eating
	 if (eaglesEating == 0)		//And check if there are any more eagles eating
	 {
		 someoneEating.Signal();	//If there are not, we signal such by Signaling the someoneEating semaphore
	 }
	 
 	 char space = ' ';
	 char * spaces = &space;
	 int i;
 	 char output [10000];
	 char * outputPointer = output;
	 char buffer [100];
	 char * bufferPointer = buffer;
	 for (i = 0; i < eagleNumber; i++)
	 {
		 strcat(outputPointer, spaces);
	 }
	 sprintf(buffer, "Baby eagle %d finishes eating.\n", eagleNumber);
	 strcat(outputPointer, buffer);
	 write(1, outputPointer, strlen(output));
	 memset(outputPointer, '\0', sizeof(output));
	 memset(bufferPointer, '\0', sizeof(buffer));
	 safety.Unlock();
  }
 
  /* -----------------------------------------------------------------
  * FUNCTION: goto_sleep
  * 	This function is called by the mother when she wants to take a nap.
  * PARAMETER USAGE:
  * 	This function currently has no parameters.
  * FUNCTION CALLS:
  * 	This function currently calls no other functions.
  * ----------------------------------------------------------------- */
 void goto_sleep()
 {
	 motherSleeping.Signal();	//Here we signal that the mother is now sleeping
	 motherNeeded.Wait();		//The mother sleeps in this semaphore and waits for a baby eagle to wake it up
 }
 
  /* -----------------------------------------------------------------
  * FUNCTION: food_ready
  * 	This function is called by the mother after she refills the food pots.
  * PARAMETER USAGE:
  * 	This function currently has no parameters.
  * FUNCTION CALLS:
  * 	This function currently calls no other functions.
  * ----------------------------------------------------------------- */
  void food_ready()
  {
	 someoneEating.Wait(); //Once awoken, the mother must first wait until all the current eagles are done eating
	 safety.Lock();
	 motherSignaled = 0;		//Then we reset the int that signals we have been called
	 char output [10000];
	 char * outputPointer = output;
	 char buffer [100];
	 
	 if (timesRefilled == numberMeals)	//Then we check if we are done serving all the meals
	 {
		 //Once no more eagles are eating we can terminate
		 sprintf(output, "Mother retires after serving %d feedings. Game ends!!!\n", timesRefilled);
		 write(1, outputPointer, strlen(output));
		 Exit();
	 }
	 
	 //If there are more meals to serve, then we continue
	 timesRefilled++;	//We increase the meal counter
	 
	 sprintf(output, "Mother eagle says \"Feeding (%d)\"\n",  timesRefilled);
	 write(1, outputPointer, strlen(output));
	 
	 foodPot = numberPots;			//And set the number of unoccupied pots with food in them to the total number of pots
	 //Then we signal that as many baby eagles as there are pots can get through
	 int i;
	 for (i = 0; i < numberPots; i++)
	 {
		 foodQueue.Signal();
	 }
	 someoneEating.Signal();	//And allow the first eagle to get in
	 safety.Unlock();
  }
  
  
