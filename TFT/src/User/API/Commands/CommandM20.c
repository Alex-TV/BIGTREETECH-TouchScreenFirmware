#include "CommandM20.h"
#include "includes.h"
#include "ff.h"
#include "myfatfs.h"
#include "diskio.h"
#include "limits.h"

void command_result_send();
FRESULT scan_files(char *path);

const int buffer_size = 1024;

char *buffer;
int index_w;

char begin_str[] = "Begin file list\n";
char end_str[] = "End file list\n";
char error_str[] = "Error\n";

void StartExtcute()
{
    FATFS fs;

    FRESULT res = f_mount(&fs, "SD:", 1);
    if (res != FR_OK)
    {
        Serial_Puts(ESP3D_PORT, error_str);
        return;
    }

    Serial_Puts(ESP3D_PORT, begin_str);

    index_w = 0;
    buffer = malloc(buffer_size);
    memset(buffer, 0, buffer_size);

    char buff[FF_LFN_BUF + 1];
    strcpy(buff, "");

    res = scan_files(buff);

    if (res == FR_OK)
    {
        Serial_Puts(ESP3D_PORT, end_str);
    }
    else
    {
        Serial_Puts(ESP3D_PORT, error_str);
    }
    free(buffer);
}

FRESULT scan_files(char *path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int path_ln = 0;

    res = f_opendir(&dir, path);
    if (res != FR_OK)
    {
        return res;
    }
    path_ln = strlen(path);
    for (;;)
    {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0)
        {
            break;
        }
        if ((fno.fattrib & AM_HID) != 0)
        {
            continue;
        }
        if (fno.fname[0] == '.')
        {
            continue;
        }
        if ((fno.fattrib & AM_DIR) == AM_DIR)
        {
            sprintf(&path[path_ln], "/%s", fno.fname);
            res = scan_files(path);
            if (res != FR_OK)
            {
                break;
            }
            path[path_ln] = 0;
        }
        else
        {
            char full_file[(FF_LFN_BUF + 1) * 3];
            sprintf(full_file, "%s/%s %d\n", path, fno.fname, (int)fno.fsize);
            int full_file_ln = strlen(full_file);
            if (full_file_ln + index_w >= buffer_size)
            {
                command_result_send();
            }

            index_w = index_w + full_file_ln;
            strcat(buffer, full_file);
        }
    }
    command_result_send();
    f_closedir(&dir);
    return res;
}

void command_result_send()
{
    if (index_w == 0)
    {
        return;
    }
    Serial_Puts(ESP3D_PORT, buffer);
    memset(buffer, 0, buffer_size);
    index_w = 0;
}