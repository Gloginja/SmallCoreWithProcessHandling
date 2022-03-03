/*
 * kernlsem.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include "kernlsem.h"
#include "list.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include <iostream.h>
void dispatch();

List<KernelSem*>* KernelSem::AllKernelSemaphores = new List<KernelSem*>();

KernelSem::KernelSem(int value)
{
	this->value = value;
	WaitingPCBs = new List<elem*>();
	lock;
		KernelSem::AllKernelSemaphores->pushBack(this);
	unlock;
}
KernelSem::~KernelSem(){
	lock;
		while(!WaitingPCBs->isEmpty())
		{
			PCB* tmp = WaitingPCBs->popFront()->pcb;
			tmp->setStatus(READY);
			Scheduler::put(tmp);
		}
		delete WaitingPCBs;
		KernelSem::AllKernelSemaphores->positionCurrent(this);
		KernelSem::AllKernelSemaphores->erase();
		KernelSem::AllKernelSemaphores->resetCurrent();
	unlock;
}
int KernelSem::block(Time maxTimeToWait){
	lock;
		int ret;
		PCB::runningPCB->setStatus(BLOCKED);
		elem *e = new elem((PCB*)PCB::runningPCB, maxTimeToWait);
		WaitingPCBs->pushBack(e);
		dispatch();
		ret = e->unblock_status;
		delete e;
	unlock;
	return ret;
}
int KernelSem::unblock(){
	lock;
	int ret = 0;
	/*elem* noTime = nullptr;
	WaitingPCBs->resetCurrent();
	while(!WaitingPCBs->isLast())
	{
		if(WaitingPCBs->getCurrent()->data->Time_to_wait<=0)
		{
			noTime = WaitingPCBs->getCurrent()->data;
			break;
		}
		else
			WaitingPCBs->nextCurrent();
	}*/
	if(!WaitingPCBs->isEmpty()){ //&& noTime==nullptr){
		elem* e = WaitingPCBs->popFront();
		e->unblock_status = 1;
		e->pcb->setStatus(READY);
		Scheduler::put(e->pcb);
		ret = 1;
	}
	/*else
	{
		noTime->unblock_status = 1;
		noTime->pcb->setStatus(READY);
		Scheduler::put(noTime->pcb);
		WaitingPCBs->erase();
		ret = 1;
	}*/
	unlock;
	return ret;
}

int KernelSem::getValue(){
	lock;
	int ret = value;
	unlock;
	return ret;
}
void KernelSem::setValue(int value){
	lock;
	this->value = value;
	unlock;
}

int KernelSem::getWaitingCount()
{
		return this->WaitingPCBs->getCount();
}


void KernelSem::signalSemaphores(){
	KernelSem::AllKernelSemaphores->resetCurrent();
	while(!KernelSem::AllKernelSemaphores->isLast()){
		KernelSem* ks = KernelSem::AllKernelSemaphores->getCurrent()->data;
		List<elem*>*tmpWaitingPCBs = ks->WaitingPCBs;
		tmpWaitingPCBs->resetCurrent();
		while(!tmpWaitingPCBs->isLast()){
			elem *e = tmpWaitingPCBs->getCurrent()->data;

			if(e->Time_to_wait != 0){

				e->Time_to_wait--;
				if(e->Time_to_wait == 0){
					e->unblock_status = 0;
					e->pcb->setStatus(READY);
					Scheduler::put(e->pcb);
					tmpWaitingPCBs->erase();
				}
				else
					tmpWaitingPCBs->nextCurrent();
			}
			else
				tmpWaitingPCBs->nextCurrent();
		}
		KernelSem::AllKernelSemaphores->nextCurrent();
	}

}





