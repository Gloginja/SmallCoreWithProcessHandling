/*
 * pcb.cpp
 *
 *  Created on: Aug 3, 2021
 *      Author: OS1
 */
#include "pcb.h"
#include <dos.h>
#include "thread.h"
#include "SCHEDULE.h"
#include <iostream.h>


volatile  PCB* PCB::runningPCB = new PCB(0, 1, nullptr, nullptr , RUNNING);
volatile List<PCB*>*PCB::PCBS = new List<PCB*>();
ID PCB::nextId = 0;
volatile  PCB* PCB::idle = new PCB(1024, 1, nullptr, PCB::idleThread, IDLE );
volatile unsigned PCB::lck = 0;
volatile boolean PCB::ecplicitContextChange = false;
volatile Time PCB::timeLeft = 0;
void PCB::wrapper(){
	PCB::runningPCB->myThread->run();
	lock;
	PCB::terminate();
	unlock;
}

void PCB::idleThread(){
	while(true){
		//syncPrintf("IDLE");
	}
}

void PCB::terminate(){
	lock;
	if(PCB::runningPCB->stateStatus != FINISHED){
		while(!PCB::runningPCB->waiting->isEmpty()){
			PCB* tmp = PCB::runningPCB->waiting->popFront();
			tmp->stateStatus = READY;
			Scheduler::put(tmp);
		}
		PCB::runningPCB->stateStatus = FINISHED;
		unlock;
		dispatch();
		lock;
	}
	unlock;
}

PCB::PCB(StackSize stackSize,Time _timeSlice, Thread* thread, void (*fun)(),State _stateStatus)
{
	this->timeSlice = _timeSlice;
	this->myThread = thread;
	this->stateStatus = _stateStatus;
	this->id = nextId++;
	this->oldFlag = 0;
	if (stackSize<minStackSize) stackSize = minStackSize;
	if (stackSize>maxStackSize) stackSize = maxStackSize;
	unsigned long stackCount = stackSize/sizeof(unsigned);

	lock;
		stack = new unsigned[stackCount];
		PCB::timeLeft = this->timeSlice;
	unlock;

#ifndef BCC_BLOCK_IGNORE
	stack[stackCount-1] = 0x200; // PSWI=1
	stack[stackCount-2] = FP_SEG(fun);
	stack[stackCount-3] = FP_OFF(fun);
	stackPointer = FP_OFF(stack+stackCount-12);
	stackSegment = FP_SEG(stack+stackCount-12);
	basePointer = stackPointer;
#endif
	lock;
		waiting= new List<PCB*>();
		((List<PCB*>*)PCBS)->pushBack(this);
	unlock;
};

PCB::~PCB()
{
	lock;
		delete stack;
		stack= nullptr;
		while(!waiting->isEmpty())
		{
			PCB* tmp = waiting->popFront();
			tmp->stateStatus=READY;
			Scheduler::put(tmp);
		}
		waiting = nullptr;
		remove();
	unlock;
}

void PCB::remove()
{
	((List<PCB*>*)PCBS)->resetCurrent();
		while(!((List<PCB*>*)PCBS)->isLast())
		{
			if(((List<PCB*>*)PCBS)->getCurrent()->data->id==id)
			{
				((List<PCB*>*)PCBS)->erase();
			}
			else
			{
				((List<PCB*>*)PCBS)->nextCurrent();
			}
		}
}

ID PCB::getId()
{
	lock;
		int _id = id;
	unlock;
	return _id;
}

ID PCB::getRunningId()
{
	lock;
		ID _id = PCB::runningPCB->id;
	unlock;
	return _id;
}

Thread* PCB::getThreadById(ID id)
{
	lock;
	Thread* t = nullptr;
	((List<PCB*>*)PCBS)->resetCurrent();
	while(!((List<PCB*>*)PCBS)->isLast())
	{
		if(((List<PCB*>*)PCBS)->getCurrent()->data->id==id)
		{
			t = ((List<PCB*>*)PCBS)->getCurrent()->data->myThread;
			break;
		}
		else
		{
			((List<PCB*>*)PCBS)->nextCurrent();
		}
	}
	unlock;
	return t;
}



void PCB::waitToComplete()
{
	lock;
		if (PCB::runningPCB != this && stateStatus != FINISHED && stateStatus != CREATED && stateStatus != IDLE) {
			PCB::runningPCB->stateStatus=BLOCKED;
			waiting->pushFront((PCB*)PCB::runningPCB);
			dispatch();
		}
	unlock;
}

void PCB::waitForAll()
{
	lock;
	((List<PCB*>*)PCBS)->resetCurrent();
	List<PCB*>*temp = new List<PCB*>();

	while(!((List<PCB*>*)PCBS)->isLast())
	{
		if(((List<PCB*>*)PCBS)->getCurrent()->data->stateStatus==READY &&
				((List<PCB*>*)PCBS)->getCurrent()->data->id != 1 && ((List<PCB*>*)PCBS)->getCurrent()->data->id != 0)
		{
			temp->pushBack(((List<PCB*>*)PCBS)->getCurrent()->data);
		}
		((List<PCB*>*)PCBS)->nextCurrent();
	}
	unlock;

	temp->resetCurrent();
	while(!temp->isLast())
	{
		temp->getCurrent()->data->waitToComplete();
		temp->nextCurrent();
	}
}

void PCB::start()
{
	lock;
		if(stateStatus==CREATED)
		{
			stateStatus=READY;
			Scheduler::put(this);
		}
	unlock;
}

void PCB::setS(unsigned* s)
{
	stack = s;
}

unsigned* PCB::getS()
{
	return stack;
}

void PCB::setSS(unsigned ss)
{
	stackSegment = ss;
}

unsigned PCB::getSS()
{
	return stackSegment;
}

void PCB::setSP(unsigned sp)
{
	stackPointer = sp;
}

unsigned PCB::getSP()
{
	return stackPointer;
}

void PCB::setBP(unsigned bp)
{
	basePointer = bp;
}

unsigned PCB::getBP()
{
	return basePointer;
}

void PCB::setTime(unsigned time)
{
	timeSlice = time;
}

unsigned PCB::getTime()
{
	return timeSlice;
}

void PCB::setStatus(State st) volatile
{
	stateStatus = st;
}

State PCB::getStatus() volatile
{
	return stateStatus;
}




