
#include "interfaceCmd.h"
#include "includes.h"
#include "stdbool.h"
#include "ESP3D_InterfaceCmd.h"
#include "ESP3D_Cmd_M20.h"
#include "ESP3D_Cmd_M23.h"
#include "ESP3D_Cmd_M24.h"
#include "ESP3D_Cmd_M30.h"

bool file_open = false;

void ESP3DSendQueueCmd(void)
{
    if (isPrinting())
    {
        return;
    }
    if (infoHost.wait == true)
    {
        return;
    }
    if (infoCmd.count == 0)
    {
        return;
    }

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
            Execution_ESP3D_M23(infoCmd.queue[infoCmd.index_r].gcode);
            if (infoCmd.queue[infoCmd.index_r].gcode[0] == 'M')
            {
                int cmd = strtol(&infoCmd.queue[infoCmd.index_r].gcode[1], NULL, 10);
                if (cmd == 24)
                {
                    ESP3DSendQueueCmd();
                    return;
                }
            }
            Clear_ESP3D_file_for_print();
            break;
        case 24:
            command_processed = true;
            Execution_ESP3D_M24();
            Clear_ESP3D_file_for_print();
            break;
        case 28:
            command_processed = true;
            Execution_ESP3D_M23(infoCmd.queue[infoCmd.index_r].gcode);
            file_open = Execution_ESP3D_M28(Get_ESP3D_file_for_print());
            Clear_ESP3D_file_for_print();
            break;
        case 29:
            command_processed = true;
            if (file_open)
            {
                Execution_ESP3D_M29(Get_New_File());
                file_open = false;
            }
            break;
        case 30:
            command_processed = true;
            Execution_ESP3D_M23(infoCmd.queue[infoCmd.index_r].gcode);
            Execution_ESP3D_M30();
            Clear_ESP3D_file_for_print();
            break;
        default:
            if (file_open)
            {
                bool res = Add_To_File_ESP3D_M28(infoCmd.queue[infoCmd.index_r].gcode);
                command_processed = true;
                if (!res)
                {
                    Execution_ESP3D_M29(Get_New_File());
                    file_open = false;
                }
            }
            break;
        }
    }

    if (command_processed)
    {
        infoCmd.count--;
        infoCmd.index_r = (infoCmd.index_r + 1) % CMD_MAX_LIST;
    }
}