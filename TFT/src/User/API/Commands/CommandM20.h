
#ifndef _COMMANDM20_H_
#define _COMMANDM20_H_

#include "stdbool.h"
#include "ff.h"

bool begin_extcute;
bool complete;
char *buffer;
void StartExtcute();
void FillBuffer();
FRESULT scan_files(char *path); /* Start node to be scanned (***also used as work area***) */

#endif