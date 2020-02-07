
#ifndef __ESP3D_Cmd_M28_H__
#define __ESP3D_Cmd_M28_H__

#include "ff.h"
#include "vfs.h"

bool Execution_ESP3D_M28();
bool Add_To_File_ESP3D_M28(const TCHAR *buff);
FIL *Get_New_File();

#endif //__ESP3D_Cmd_M28_H__
