#ifndef _IVTPEN_H_
#define _IVTPEN_H_
#include <iostream.h>
#include "datadef.h"

#define PREPAREENTRY(IvtNo,	Old)						\
void interrupt intrRoutine##IvtNo (...);				\
IVTPEntry iEntry##IvtNo (IvtNo, intrRoutine##IvtNo);	\
void interrupt intrRoutine##IvtNo (...){				\
	iEntry##IvtNo.signal();								\
	if(Old)												\
		iEntry##IvtNo.callOld();						\
}

class KernelEv;
template<class T>
class List;
class IVTPEntry {
public:
	IVTNo IvtNo;
	pInterrupt newRoutine;
	pInterrupt oldRoutine;
	volatile KernelEv* event;
	static List<IVTPEntry*> AllIVTPEntries;
public:

	IVTPEntry(IVTNo IvtNo, pInterrupt newRoutine);
	~IVTPEntry();

	void setEvent(KernelEv* event);
	void signal()volatile;
	void callOld()volatile;
	IVTNo getNo() const;
	//friend boolean compare(IVTNo IvtNo1, IVTNo IvtNo2);
	//friend boolean equals(IVTNo IvtNo1, IVTNo IvtNo2);
	//friend boolean equals(IVTPEntry* ie1, IVTPEntry* ie2);

	static IVTPEntry* getIVTPEntryByEntry(IVTNo IvtNo);

	static void interrupt timer(...);
	static IVTPEntry iEntry8;
};

#endif
