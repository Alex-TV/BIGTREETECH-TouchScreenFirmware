
#include "ESP3D_Cmd_M24.h"
#include "ESP3D_Cmd_M23.h"
#include "includes.h"

void Execution_ESP3D_M24()
{
    infoMenu.cur = 1;
    infoFile.source = TFT_SD;
    memset(infoFile.title, 0, 1024);
    strcat(infoFile.title, "SD:");
    strcat(infoFile.title, Get_ESP3D_file_for_print());
    menuBeforePrinting();
}