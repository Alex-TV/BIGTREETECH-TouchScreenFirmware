
#include "ESP3D_Cmd_M30.h"
#include "ESP3D_Cmd_M23.h"
#include "Configuration.h"
#include "ff.h"
#include "vfs.h"
#include "Serial.h"
#include "string.h"

void Execution_ESP3D_M30()
{
    TCHAR title[MAX_PATH_LEN];
    memset(title, 0, MAX_PATH_LEN);
    strcat(title, "SD:");
    strcat(title, Get_ESP3D_file_for_print());
    if (f_unlink(title) != FR_OK)
    {
        Serial_Puts(ESP3D_PORT, "Error\n");
    }
}