/* ------------------------------------------------------------------
 * Author: Nicholas Christensen					User ID: nwchrist
 * Due Date: 11/18/2017
 * Class: CS3331
 * Program Assignment #4
 * File Name: thread.h
 * File Purpose:
 *		This file contains the class definitions of my threads.
 * ------------------------------------------------------------------  */
 
 #include "ThreadClass.h"
 
 static Semaphore motherNeeded("MotherNeeded", 0);
 static Semaphore someoneEating("SomoneEating", 1);
 static Semaphore foodQueue("FoodQueue", 0);
 static Semaphore motherSleeping("MotherSleeping", 0);
 static Mutex safety;
 
 extern int numberMeals;
 extern int numberPots;
 extern int numberBabies;
 extern int eaglesWaiting;
 extern int foodPot;
 extern int eaglesEating;
 extern int motherSignaled;
 extern int timesRefilled;
 extern int wokeBabyEagleNumber;
 
 class thread : public Thread
{
	public:
		thread(int n);
	private:
		int number;
		void ThreadFunc();
};

int ready_to_eat(int eagleNumber);
void finish_eating(int eagleNumber);
void food_ready();
void goto_sleep();