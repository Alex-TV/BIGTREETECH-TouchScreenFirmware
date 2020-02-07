
#include "ESP3D_Cmd_M29.h"
#include "Configuration.h"
#include "ff.h"
#include "vfs.h"

void Execution_ESP3D_M29(FIL *fp)
{
    if (f_close(fp) != FR_OK)
    {
        Serial_Puts(ESP3D_PORT, "Error\n");
    }
}
