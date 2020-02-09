
#ifndef __ESP3D_Cmd_M28_H__
#define __ESP3D_Cmd_M28_H__

#include "ff.h"
#include "vfs.h"

bool Execution_ESP3D_M28(char *fileName);
bool Add_To_File_ESP3D_M28(char *gcode, int index_check_sum, int index_cmd);
void Close_New_File();

#endif //__ESP3D_Cmd_M28_H__
