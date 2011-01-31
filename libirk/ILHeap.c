#include "ILHeap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ILHeap *ILHeapCreate() {
	ILHeap
		*ih;

	ih = malloc(sizeof(ILHeap));
	if(ih == NULL) {
		perror("ILHeap");
		exit(-1);
	}

	ih->entries = malloc(sizeof(ILHeapEntry) * ILHEAP_INITIAL_SIZE);
	if(ih->entries == NULL) {
		perror("ILHeap");
		exit(-1);
	}

	ih->size = 0;
	ih->allocated_size = ILHEAP_INITIAL_SIZE;
	return ih;
}

void ILHeapFree(ILHeap *ih) {
	free(ih->entries);
	free(ih);
}

void ILHeapEntrySwap(ILHeapEntry *a, ILHeapEntry *b) {
	ILHeapEntry tmp;
	memcpy(&tmp,a,sizeof(ILHeapEntry));
	memcpy(a,b,sizeof(ILHeapEntry));
	memcpy(b,&tmp,sizeof(ILHeapEntry));
}

void *ILHeapRemove(ILHeap *ih) {
	unsigned int
		parent, child;
	void
		*data;
	ILHeapEntry
		*entries;

	if(ih->size == 0) {
		return (void*)NULL;
	}
	entries = ih->entries;

	data = entries[0].data;

	ILHeapEntrySwap( &entries[0], &entries[ih->size--] );

	parent = 0;
	child = 1;

	while( child < (ih->size - 1) ) {
		if(entries[child].prio < entries[child+1].prio) {
			child++;
		}
		if(entries[child].prio > entries[parent].prio) {
			ILHeapEntrySwap(&entries[child],&entries[parent]);
			parent = child;
			child *= 2;
		}
		else {
			return data;
		}
	}

	return data;
}

void ILHeapInsert(ILHeap *ih, int priority, void *data) {
	ILHeapEntry
		*entries;
	unsigned int
		child, parent;

	if(ih->size >= ih->allocated_size) {
		entries = realloc(ih->entries,sizeof(ILHeapEntry) * (ih->allocated_size + ILHEAP_GROWTH));
		if(entries == NULL) {
			/* Allocation failed */
			fprintf(stderr, "Insertion failed!!!\n");
			return;
		}
		else if(entries != ih->entries) {
			ih->entries = entries;
		}
		ih->allocated_size += ILHEAP_GROWTH;
	}
	else {
		entries = ih->entries;
	}

	ih->size++;

	child = ih->size;
	parent = child / 2;

	entries[child].prio = priority;
	entries[child].data = data;

	while(parent < child) {
		if(entries[parent].prio < entries[child].prio) {
			ILHeapEntrySwap(&entries[parent],&entries[child]);
			child = parent;
			parent /= 2;
		}
		else {
			return;
		}
	}
}
