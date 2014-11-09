
#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stdio.h>
#include "core.h"

void * manage_data();
int analyze(Buffer *buffer);
int is_injection(char * request);

#endif