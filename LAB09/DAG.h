#ifndef DAG_MODULE
#define DAG_MODULE

#include <stdio.h>
#include <stdlib.h>

#include "GRAPH.h"

typedef struct dag *DAG;

DAG DAGinit(int V);
void DAGfree(DAG dag);

DAG DAGload();

#endif