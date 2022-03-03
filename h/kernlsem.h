#ifndef _KERNELSEM_H_
#define _KERNELSEM_H_

#include "datadef.h"
//#define TIME_UNBLOCKED 0
//#define SIGNAL_UNBLOCKED 1




class PCB;
template <class T>
class List;
class KernelSem
{
private:
	struct elem
	{
		PCB* pcb;
		Time Time_to_wait;
		int unblock_status;
		elem(PCB* pcb, Time Time_to_wait)
		{
			this->pcb = pcb;
			this->Time_to_wait = Time_to_wait;
			this->unblock_status = 1;
		}
	};
	List<elem*> *WaitingPCBs;
	int value;
public:
	KernelSem(int value);
	~KernelSem();
	int block(Time maxTimeToWait);
	int unblock();
	int getValue();
	void setValue(int value);
	List<elem*> getWaitingPCBs();
	static List<KernelSem*>* AllKernelSemaphores;
	static void signalSemaphores();
	int getWaitingCount();
};

#endif
