#ifndef KERNLEV_H_
#define KERNLEV_H_

#include "datadef.h"

class PCB;
class Semaphore;
class KernelEv {
private:
	Semaphore *sem;
	PCB * pcb;
	IVTNo ivtNo;
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal()volatile;
	PCB* getPCB();

};

#endif
