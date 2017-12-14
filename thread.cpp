/* ------------------------------------------------------------------
 * Author: Nicholas Christensen					User ID: nwchrist
 * Due Date: 11/18/2017
 * Class: CS3331
 * Program Assignment #4
 * File Name: thread.cpp
 * File Purpose:
 *		This file contains the class implementations for my threads.
 * ------------------------------------------------------------------  */
 
#include "thread.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
 
thread::thread(int n)
{
	number = n;
	ThreadName.seekp(0, ios::beg);
	ThreadName << "Thread " << number;
}

void thread::ThreadFunc()
{
	Thread::ThreadFunc();
	char output [10000];
	char * outputPointer = output;
	char buffer [100];
	char * bufferPointer = buffer;
	char space = ' ';
	char * spaces = &space;
	int i;
	for (i = 0; i < number; i++)
	{
		strcat(outputPointer, spaces);
	}
	sprintf(buffer, "Baby eagle %d started.\n", number);
	strcat(outputPointer, buffer);
	write(1, outputPointer, strlen(output));
	memset(outputPointer, '\0', sizeof(output));
	memset(bufferPointer, '\0', sizeof(buffer));
	
	//The following code is the baby eagle's pattern
	while (1)
	{
		//sleep(rand() / 699000000 + 1);			//Delay to get hungry
		
		for (i = 0; i < number; i++)
		{
			strcat(outputPointer, spaces);
		}
		sprintf(buffer, "Baby eagle %d is ready to eat.\n", number);
		strcat(outputPointer, buffer);
		write(1, outputPointer, strlen(output));
		memset(outputPointer, '\0', sizeof(output));
		memset(bufferPointer, '\0', sizeof(buffer));
		
		int resultPot = ready_to_eat(number);		//Try to eat
		//sleep(rand() / 699000000 + 1);			//Eating delay
		
		for (i = 0; i < number; i++)
		{
			strcat(outputPointer, spaces);
		}
		sprintf(buffer, "Baby eagle %d is eating using feeding pot %d.\n", number, resultPot);
		strcat(outputPointer, buffer);
		write(1, outputPointer, strlen(output));
		memset(outputPointer, '\0', sizeof(output));
		memset(bufferPointer, '\0', sizeof(buffer));
		
		finish_eating(number);	//Done Eating
	}
}