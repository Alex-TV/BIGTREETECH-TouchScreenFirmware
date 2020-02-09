
#include "ESP3D_Cmd_M28.h"
#include "Configuration.h"
#include "ESP3D_InterfaceCmd.h"
#include "ff.h"
#include "vfs.h"
#include "Serial.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "myfatfs.h"

FRESULT Write_Buff();

char *file_buff;
int buff_size_w;
FIL *fp;

bool Execution_ESP3D_M28(char *fileName)
{
    TCHAR title[MAX_PATH_LEN];
    memset(title, 0, MAX_PATH_LEN);
    strcat(title, "SD:");
    strcat(title, fileName);
    FRESULT res = FR_DISK_ERR;

    if (mountSDCard())
    {
        fp = malloc(sizeof(FIL));
        res = f_open(fp, fileName, FA_WRITE | FA_OPEN_ALWAYS);
        Serial_Puts(ESP3D_PORT, res == FR_OK ? ok_str : error_str);

        if (res == FR_OK)
        {
            file_buff = malloc(FF_MAX_SS);
            memset(file_buff, 0, FF_MAX_SS);
        }
    }
    return res == FR_OK;
}

bool Add_To_File_ESP3D_M28(char *gcode, int index_check_sum, int index_cmd)
{
    int ln_file = index_check_sum - index_cmd - 1;
    int next_buff_size_w = buff_size_w + ln_file + 2;
    FRESULT res = FR_OK;
    if (next_buff_size_w >= FF_MAX_SS)
    {
        res = Write_Buff();
    }

    if (res == FR_OK)
    {
        for (int i = 0; i < ln_file; i++)
        {
            file_buff[buff_size_w + i] = gcode[index_cmd + i];
        }
        buff_size_w += ln_file;
        file_buff[buff_size_w] = '\r';
        buff_size_w++;
        file_buff[buff_size_w] = '\n';
        buff_size_w++;
    }

    Serial_Puts(ESP3D_PORT, res == FR_OK ? ok_str : error_str);
    return res == FR_OK;
}

FRESULT Write_Buff()
{
    UINT write_byte = 0;
    FRESULT res = f_write(fp, (const void *)file_buff, (UINT)buff_size_w, &write_byte);
    Serial_Puts(ESP3D_PORT, res == FR_OK ? ok_str : error_str);
    memset(file_buff, 0, FF_MAX_SS);
    buff_size_w = 0;
    return res;
}

void Close_New_File()
{
    if (buff_size_w > 0)
    {
        Write_Buff();
    }

    FRESULT res = f_close(fp);
    Serial_Puts(ESP3D_PORT, res == FR_OK ? ok_str : error_str);

    free(file_buff);
    free(fp);
}
