
#include "ESP3D_Cmd_M28.h"
#include "ESP3D_Cmd_M23.h"
#include "Configuration.h"
#include "ff.h"
#include "vfs.h"
#include "Serial.h"
#include "string.h"

FIL *fp;

bool Execution_ESP3D_M28(char *fileName)
{
    TCHAR title[MAX_PATH_LEN];
    memset(title, 0, MAX_PATH_LEN);
    strcat(title, "SD:");
    strcat(title, fileName);
    if (f_open(fp, title, FA_CREATE_NEW) != FR_OK)
    {
        Serial_Puts(ESP3D_PORT, "Error\n");
        return false;
    }
    return true;
}

bool Add_To_File_ESP3D_M28(const TCHAR *buff)
{
    if (f_puts(buff, fp) == -1)
    {
        Serial_Puts(ESP3D_PORT, "Error\n");
        return false;
    }
    return true;
}

FIL *Get_New_File()
{
    return fp;
}