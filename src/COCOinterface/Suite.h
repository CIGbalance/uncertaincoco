/** @file test */ 

#ifndef SUITE_H
#define SUITE_H

#include "../algos/CocoOptimiser.h"
#include "coco.h"
#include "COCOfunc.h"

/// Brief desc test
class Suite{
public:
    Suite(char* suite_name, int budget_multiplier,
        int batches, int cbatch, const char* dimension_string,
        char* observer_name, int independent_restarts = 1e5, int batchOffset=0);
    void run(CocoOptimiser * algo, int timeWindow, Transform trans, bool lido);

private:
    coco_suite_t * suite;
    char * suite_name;
    char * observer_name;
    int independent_restarts;
    int budget_multiplier;
    int cbatch;
    int batches;
    int batchOffset;
};

#endif /* SUITE_H */

