/*
 * ivtpen.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: OS1
 */
#include "ivtpen.h"
#include "kernelEv.h"
#include "pcb.h"
#include <dos.h>
#include "kernlsem.h"
#include "SCHEDULE.H"
#include "datadef.h"
#include "thread.h"
#include <iostream.h>
List<IVTPEntry*> IVTPEntry::AllIVTPEntries;

IVTPEntry::IVTPEntry(IVTNo IvtNo, pInterrupt newRoutine){
	lock;
	this->IvtNo = IvtNo;
	this->oldRoutine = nullptr;
	this->newRoutine = nullptr;
	this->event = nullptr;
	#ifndef BCC_BLOCK_IGNORE
	this->oldRoutine = getvect(IvtNo);
	this->newRoutine = newRoutine;
	setvect(IvtNo, newRoutine);
	#endif
	AllIVTPEntries.pushFront(this);
	unlock;
}
IVTPEntry::~IVTPEntry(){
	lock;
	#ifndef BCC_BLOCK_IGNORE
	oldRoutine();
	setvect(IvtNo, oldRoutine);
	#endif
	AllIVTPEntries.positionCurrent(this);
	AllIVTPEntries.erase();
	AllIVTPEntries.resetCurrent();
	unlock;
}

IVTPEntry* IVTPEntry::getIVTPEntryByEntry(IVTNo IvtNo){
	lock;
	IVTPEntry* ret;
	AllIVTPEntries.resetCurrent();
	while(!AllIVTPEntries.isLast())
	{
		if(AllIVTPEntries.getCurrent()->data->IvtNo==IvtNo)
		{
			ret = AllIVTPEntries.getCurrent()->data;
			break;
		}
		AllIVTPEntries.nextCurrent();
	}
	unlock;
	return ret;
}

void IVTPEntry::setEvent(KernelEv* event){
	lock;
	this->event = event;
	unlock;
}
void IVTPEntry::signal() volatile{
	lock;
	event->signal();
	unlock;
}
void IVTPEntry::callOld()volatile{
	oldRoutine();
}

IVTNo IVTPEntry::getNo() const{
	return IvtNo;
}


void interrupt IVTPEntry::timer(...){
	static unsigned tsp;
	static unsigned tbp;
	static unsigned tss;

	if (!PCB::ecplicitContextChange) {
		IVTPEntry::iEntry8.callOld();
		KernelSem::signalSemaphores();
		extern void tick();
		tick();

	}

	if(!PCB::ecplicitContextChange && PCB::timeLeft>0)
		PCB::timeLeft--;

	if ((PCB::runningPCB->timeSlice!=0 && PCB::timeLeft == 0 ) || PCB::ecplicitContextChange) {
		if(PCB::lck == 0 || PCB::ecplicitContextChange){
			PCB::ecplicitContextChange = 0;
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp ,bp
			}
			#endif
			PCB::runningPCB->stackPointer = tsp;
			PCB::runningPCB->stackSegment = tss;
			PCB::runningPCB->basePointer = tbp;

			PCB::runningPCB->oldFlag = PCB::lck;

			if (PCB::runningPCB->stateStatus == RUNNING)
			{
				PCB::runningPCB->stateStatus = READY;
				Scheduler::put((PCB *) PCB::runningPCB);
			}
			PCB::runningPCB=Scheduler::get();
			if(PCB::runningPCB == nullptr){
				PCB::runningPCB = PCB::idle;
			} else PCB::runningPCB->stateStatus = RUNNING;

			tsp = PCB::runningPCB->stackPointer;
			tss = PCB::runningPCB->stackSegment;
			tbp = PCB::runningPCB->basePointer;

			PCB::lck = PCB::runningPCB->oldFlag;

			PCB::timeLeft = PCB::runningPCB->timeSlice;
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			#endif


		}

	}




}

IVTPEntry IVTPEntry::iEntry8(0x8, IVTPEntry::timer);





