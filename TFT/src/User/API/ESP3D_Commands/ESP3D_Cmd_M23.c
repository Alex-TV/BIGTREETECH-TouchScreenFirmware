#include "stdbool.h"
#include "ESP3D_Cmd_M23.h"
#include "includes.h"

char *ESP3D_file_for_print;

void Execution_ESP3D_M23(char *gcode)
{
    strcpy(gcode, &gcode[4]);
    int ln_file = 0;
    int ln_file_1 = strchr(gcode, '\n') - gcode + 1;
    int ln_file_2 = strchr(gcode, '\r') - gcode + 1;
    if (ln_file_1 < ln_file_2)
    {
        ln_file = ln_file_1;
    }
    else
    {
        ln_file = ln_file_2;
    }

    ESP3D_file_for_print = malloc(ln_file);

    memset(ESP3D_file_for_print, 0, ln_file);

    strncpy(ESP3D_file_for_print, gcode, ln_file - 1);
    strcpy(gcode, &gcode[ln_file]);
}

void Clear_ESP3D_file_for_print()
{
    free(ESP3D_file_for_print);
}

char *Get_ESP3D_file_for_print()
{
    return ESP3D_file_for_print;
}
