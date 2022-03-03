/*
 * semaphor.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include "pcb.h"
#include "thread.h"
#include "semaphor.h"
#include "kernlsem.h"
#include <iostream.h>
Semaphore::Semaphore (int init){
	lock;
		myImpl = new KernelSem(init);
	unlock;
}
Semaphore::~Semaphore (){
	lock;
		delete myImpl;
		myImpl = nullptr;
	unlock;
}

int Semaphore::wait (Time maxTimeToWait){
	lock;
		int ret = 1;
		int old_val = myImpl->getValue();
		int new_val = old_val - 1;
		myImpl->setValue(new_val);
		if(new_val < 0)
			ret = myImpl->block(maxTimeToWait);
	unlock;
	return ret;
}
int Semaphore::signal()
{
	lock;
		int ret = 0;
		int old_val = myImpl->getValue();
		int new_val = old_val + 1;
		myImpl->setValue(new_val);
		if (old_val < 0)
		{
			ret = myImpl->unblock();
		}

	unlock;
	return ret;
}

int Semaphore::val () const{
	lock;
		int ret = myImpl->getValue();
	unlock;
	return ret;
}



