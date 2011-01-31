#ifndef __ILHEAP_H__
#define __ILHEAP_H__

#define ILHEAP_INITIAL_SIZE	100
#define ILHEAP_GROWTH			100

typedef struct {
	int
		prio;
	void
		*data;
} ILHeapEntry;

typedef struct {
	unsigned int
		size;
	unsigned int
		allocated_size;
	ILHeapEntry
		*entries;
} ILHeap;

ILHeap *ILHeapCreate();

void ILHeapFree(ILHeap *ih);

void *ILHeapRemove(ILHeap *ih);

void ILHeapInsert(ILHeap *ih, int priority, void *data);

#endif
