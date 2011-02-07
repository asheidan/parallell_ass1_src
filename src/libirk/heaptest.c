#include <stdlib.h>
#include <stdio.h>

#include <time.h>

#include "ILHeap.h"

#define SIZE2		(ILHEAP_INITIAL_SIZE + ILHEAP_GROWTH * 2)
#define SIZE		300

int main(int argc, char **argv) {
	ILHeap *ih;
	int
		i, *n;
	srandom(time(NULL));

	ih = ILHeapCreate();

	for(i = 0; i < SIZE; i++) {
		n = malloc(sizeof(int));
		*n = (int)random();

		ILHeapInsert(ih, *n, (void *)n);
	}
	for(i = 0; i < SIZE; i++) {
		n = (int *)ILHeapRemove(ih);
		/*fprintf(stderr, "%p\n", (void*)n);*/
		if(n != NULL) {
			fprintf(stdout, "\t\t%12d\t%d\n", *n, ih->size);
			free(n);
		}
	}

	ILHeapFree(ih);
}
