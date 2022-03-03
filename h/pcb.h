/*
 * pcb.h
 *
 *  Created on: Aug 3, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "list.h"
class Thread;

class PCB
{
friend class IVTPEntry;
private:
	unsigned* stack;
	unsigned stackPointer;
	unsigned stackSegment;
	unsigned basePointer;
	Time timeSlice;
	static ID nextId;
	ID id;
	State stateStatus;
	Thread* myThread;
	List<PCB*>* waiting;
	unsigned oldFlag;
public:


	volatile static PCB* runningPCB;
	volatile static List<PCB*>*PCBS;
	volatile static boolean ecplicitContextChange;
	volatile static boolean changeWaiting;
	volatile static unsigned lck;
	volatile static PCB* idle;
	volatile static Time timeLeft;
	static void runningMethod();

	static void idleThread();
	static void terminate();
	static void wrapper();
	void waitToComplete();
	void start();
	ID getId();
	Thread* getMyThread();
	static ID getRunningId();
	static Thread* getThreadById(ID id);
	static void waitForAll();
	void remove();
	void setCall(boolean) volatile;
	boolean getCall();

	void setS(unsigned*);
	unsigned* getS();

	void setSP(unsigned);
	unsigned getSP();

	void setSS(unsigned);
	unsigned getSS();

	void setBP(unsigned);
	unsigned getBP();

	void setTime(unsigned);
	unsigned getTime();

	void setStatus(State) volatile;
	State getStatus() volatile;

	PCB(StackSize _stackSize,Time _timeSlice, Thread* thread, void (*fun)(),State _stateStatus);
	~PCB();




};



#endif /* PCB_H_ */
