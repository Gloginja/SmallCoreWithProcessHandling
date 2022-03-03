/*
 * event.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: OS1
 */
#include "event.h"
#include "kernelEv.h"
#include "pcb.h"
#include "thread.h"
#include "ivtpen.h"
Event::Event (IVTNo ivtNo){
	lock;
	myImpl = new KernelEv(ivtNo);
	unlock;
}
Event::~Event (){
	lock;
	delete myImpl;
	unlock;
}

void Event::wait (){
	if(PCB::runningPCB == myImpl->getPCB())
	{
		myImpl->wait();
	}
}
void Event::signal(){
	myImpl->signal();
}




