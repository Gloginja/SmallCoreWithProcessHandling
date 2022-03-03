#include "thread.h"
#include "pcb.h"
#include "ivtpen.h"
Thread::Thread(StackSize stackSize, Time timeSlice)
{
	lock;
	this->myPCB = new PCB(stackSize, timeSlice, this, PCB::wrapper, CREATED);
	unlock;
}

Thread::~Thread() {
	lock;
		waitToComplete();
		delete myPCB;
		myPCB = nullptr;
	unlock;
}

ID Thread::getId()
{
	lock;
		ID _id = this->myPCB->getId();
	unlock;
	return _id;
}

Thread* Thread::getThreadById(ID id)
{
	lock;
		Thread* t = PCB::getThreadById(id);
	unlock;
	return t;
}

ID Thread::getRunningId()
{
	lock;
		ID _id = PCB::getRunningId();
	unlock;
	return _id;
}

void Thread::start()
{
	lock;
		myPCB->start();
	unlock;
}

void Thread::waitToComplete()
{
	lock;
		myPCB->waitToComplete();
	unlock;
}

PCB* Thread::getPCB()
{
	lock;
		PCB* pcb = this->myPCB;
	unlock;
	return pcb;
}

void Thread::waitForAll()
{
	PCB::waitForAll();
}


void dispatch()
{
	#ifndef BCC_BLOCK_IGNORE
	intd;
	#endif
	PCB::ecplicitContextChange=true;
	IVTPEntry::timer();
	#ifndef BCC_BLOCK_IGNORE
	inte;
	#endif
}

