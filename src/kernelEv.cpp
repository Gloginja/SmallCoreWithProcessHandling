/*
 * kernelEv.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: OS1
 */

#include "kernelEv.h"
#include "pcb.h"
#include "semaphor.h"
#include "ivtpen.h"
#include "thread.h"
KernelEv::KernelEv(IVTNo ivtNo){
	this->ivtNo = ivtNo;
	this->pcb = (PCB*)PCB::runningPCB;
	lock;
	this->sem = new Semaphore(0);
	IVTPEntry::getIVTPEntryByEntry(ivtNo)->setEvent(this);
	unlock;
}
KernelEv::~KernelEv(){
	lock;
	delete sem;
	sem = nullptr;
	unlock;
}
void KernelEv::wait(){
	lock;
	sem->wait(0);
	unlock;
}
void KernelEv::signal() volatile{
	lock;
	if(sem->val()<0)
	{
		sem->signal();
	}
	unlock;
}
PCB* KernelEv::getPCB(){
	return pcb;
}



