
#include "interfaceCmd.h"
#include "includes.h"
#include "stdbool.h"
#include "ESP3D_InterfaceCmd.h"
#include "ESP3D_Cmd_M20.h"
#include "ESP3D_Cmd_M23.h"
#include "ESP3D_Cmd_M24.h"
#include "ESP3D_Cmd_M28.h"
#include "ESP3D_Cmd_M29.h"
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
        case 30:
            command_processed = true;
            Execution_ESP3D_M23(infoCmd.queue[infoCmd.index_r].gcode);
            Execution_ESP3D_M30();
            Clear_ESP3D_file_for_print();
            break;
        case 110:
            command_processed = true;
            Serial_Puts(ESP3D_PORT, ok_str);
            break;
        case 21:
            command_processed = true;
            Serial_Puts(ESP3D_PORT, "echo:SD card ok\n");
            break;
        }
        break;
    case 'N':
        command_processed = true;
        //u16 line_num = strtol(&infoCmd.queue[infoCmd.index_r].gcode[1], NULL, 10);
        int index_check_sum = strchr(infoCmd.queue[infoCmd.index_r].gcode, '*') - infoCmd.queue[infoCmd.index_r].gcode + 1;
        //u16 check_sum = strtol(&infoCmd.queue[infoCmd.index_r].gcode[index_check_sum], NULL, 10);
        int index_cmd = strchr(infoCmd.queue[infoCmd.index_r].gcode, ' ') - infoCmd.queue[infoCmd.index_r].gcode + 1;
        int index_f_name = strchr(&infoCmd.queue[infoCmd.index_r].gcode[index_cmd], ' ') - infoCmd.queue[infoCmd.index_r].gcode + 1;

        if (infoCmd.queue[infoCmd.index_r].gcode[index_cmd] == 'M')
        {
            cmd = strtol(&infoCmd.queue[infoCmd.index_r].gcode[index_cmd + 1], NULL, 10);
            if (cmd == 29)
            {
                if (file_open)
                {
                    Close_New_File();
                    file_open = false;
                }
                else
                {
                    Serial_Puts(ESP3D_PORT, ok_str);
                }

                break;
            }

            if (cmd == 28 && !file_open)
            {
                int ln_file = index_check_sum - index_f_name;
                char *data = malloc(ln_file + 2);
                memset(data, 0, ln_file + 2);
                strncpy(data, &infoCmd.queue[infoCmd.index_r].gcode[index_f_name], ln_file - 1);
                strcat(data, "\r\n");
                file_open = Execution_ESP3D_M28(data);
                free(data);
                break;
            }
        }

        if (file_open)
        {
            //char line_num_str[22];
            // sprintf(line_num_str, "Line num: S%d", line_num);
            //  statusScreen_setMsg((u8 *)"Uploading...", (u8 *)line_num_str);
            file_open = Add_To_File_ESP3D_M28(infoCmd.queue[infoCmd.index_r].gcode, index_check_sum, index_cmd);
            if (!file_open)
            {
                Close_New_File();
            }
        }
        break;
    }

    if (command_processed)
    {
        infoCmd.count--;
        infoCmd.index_r = (infoCmd.index_r + 1) % CMD_MAX_LIST;
    }
}
