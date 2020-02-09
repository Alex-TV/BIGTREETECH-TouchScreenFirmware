#ifndef _ESP3D_InterfaceCmd_H_
#define _ESP3D_InterfaceCmd_H_

#include "stdbool.h"

static char error_str[] = "Error\n";
static char ok_str[] = "ok\n";
static char begin_str[] = "Begin file list\n";
static char end_str[] = "End file list\n";

void ESP3DSendQueueCmd(void);
#endif