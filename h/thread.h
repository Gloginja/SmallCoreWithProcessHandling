/*
 * thread.h
 *
 *  Created on: Aug 3, 2021
 *      Author: OS1
 */

#ifndef _thread_h_
#define _thread_h_
#include "datadef.h"


class PCB; // Kernel's implementation of a user's thread

class Thread{
public:
	void start();
	void waitToComplete();
	virtual~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);
	static void waitForAll();
	PCB* getPCB();
protected:
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run(){};
private:
	PCB* myPCB;

};

void dispatch();

#endif /* THREAD_H_ */
