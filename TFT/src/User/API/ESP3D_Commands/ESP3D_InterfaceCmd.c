#include "ESP3D_InterfaceCmd.h"
#include "interfaceCmd.h"
#include "includes.h"
#include "stdbool.h"
#include "ESP3D_Cmd_M20.h"

void ESP3DSendQueueCmd(void)
{
    char *data_file;
    if (infoHost.wait == true)
        return;
    if (infoCmd.count == 0)
        return;
    bool command_processed = false;
    u16 cmd = 0;
    switch (infoCmd.queue[infoCmd.index_r].gcode[0])
    {
    case 'M':
        cmd = strtol(&infoCmd.queue[infoCmd.index_r].gcode[1], NULL, 10);
        switch (cmd)
        {
        case 20:
            command_processed = true;
            Execution_ESP3D_M20();
            break;
        case 23:
            strcpy(infoCmd.queue[infoCmd.index_r].gcode, &infoCmd.queue[infoCmd.index_r].gcode[4]);

            int ln_file = strchr(infoCmd.queue[infoCmd.index_r].gcode, '\n') - infoCmd.queue[infoCmd.index_r].gcode + 1;

            data_file = malloc(ln_file);
            memset(data_file, 0, ln_file);

            strncpy(data_file, infoCmd.queue[infoCmd.index_r].gcode, ln_file - 1);
            strcpy(infoCmd.queue[infoCmd.index_r].gcode, &infoCmd.queue[infoCmd.index_r].gcode[ln_file]);

            if (infoCmd.queue[infoCmd.index_r].gcode[0] == 0)
            {
                cmd = strtol(&infoCmd.queue[infoCmd.index_r].gcode[1], NULL, 10);
                if (cmd == 24)
                {
                    ESP3DSendQueueCmd();
                    return;
                }
            }

            free(data_file);

        case 24:
            command_processed = true;
            infoMenu.cur = 1;
            infoFile.source = TFT_SD;
            memset(infoFile.title, 0, 1024);
            strcat(infoFile.title, "SD:");
            strcat(infoFile.title, data_file);
            free(data_file);
            menuBeforePrinting();
            break;
        }
    }
    if (command_processed)
    {
        infoCmd.count--;
        infoCmd.index_r = (infoCmd.index_r + 1) % CMD_MAX_LIST;
    }
}