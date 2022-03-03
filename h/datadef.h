/*
 * datadef.h
 *
 *  Created on: Aug 4, 2021
 *      Author: OS1
 */

#ifndef H_DATADEF_H_
#define H_DATADEF_H_

typedef unsigned boolean;
#define true 1
#define false 0
#define nullptr 0

#define intd asm { pushf; cli; }
#define inte asm popf;
#define lock PCB::lck++;
#define unlock PCB::lck--;		\
if (PCB::ecplicitContextChange) {		\
	dispatch();							\
}
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time x 55ms
const Time defaultTimeSlice = 2; // 2x55ms
const StackSize minStackSize = 256;
const StackSize maxStackSize = 65536;
typedef int ID;
typedef void interrupt (*pInterrupt) (...);
typedef unsigned char IVTNo;
enum State{CREATED,READY,RUNNING,BLOCKED,IDLE,FINISHED};
#endif /* H_DATADEF_H_ */
