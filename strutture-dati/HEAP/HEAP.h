#ifndef HEAP_MODULE
#define HEAP_MODULE

#include "Item.h"

#include <stdlib.h>

typedef struct heap_s *Heap;

Heap HEAPinit(int maxN);
void HEAPfree(Heap h);

void HEAPfill(Item x, Heap h);
void HEAPify(Heap h, int n);
void HEAPbuild(Heap h);
void HEAPsort(Heap h);

#endif