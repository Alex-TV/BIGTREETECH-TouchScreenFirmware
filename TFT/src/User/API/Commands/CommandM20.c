#include "CommandM20.h"
#include "includes.h"
#include "ff.h"
#include "myfatfs.h"
#include "diskio.h"

bool begin_extcute = false;
bool complete = false;
char *buffer;
int index_w;
int curFileIndex;

char begin_str[] = "Begin file list\n";
char end_str[] = "End file list\n";
char error_str[] = "Error\n";

FATFS fatfs[FF_VOLUMES];

void StartExtcute()
{
    buffer = malloc(2048);
    while (!buffer)
        ; // malloc failed
    memset(buffer, 0, 2048);
    //  if (!begin_extcute)
    //  {
    //      strcat(buffer, (const char *)begin_str);
    //      index_w += strlen(begin_str);
    // }
    // FillBuffer();

    begin_extcute = true;
    complete = false;
    //FATFS fs;
    FRESULT res;
    char buff[256];
    res = f_mount(&fatfs[VOLUMES_SD_CARD], "SD:", 1);
    //res = f_mount(&fs, "", 1);
    if (res == FR_OK)
    {
        strcpy(buff, "");
        res = scan_files(buff);
    }
    begin_extcute = false;
    complete = true;
}

void FillBuffer()
{
    int LongfileLen = sizeof(infoFile.file);
    infoFile.source = TFT_SD;
    resetInfoFile();
    powerFailedSetDriverSource(getCurFileSource());
    if (mountFS() == true && scanPrintFiles() == true)
    {
        if (curFileIndex < LongfileLen)
        {
            for (int i = curFileIndex; i < LongfileLen; i++)
            {
                begin_extcute = true;
                complete = false;
                char *file = infoFile.file[i];
                if (file == 0)
                {
                    break;
                }
                int fileNameLength = strlen(file);

                if (index_w + fileNameLength + 1 > 1024)
                {
                    curFileIndex = i;
                    return;
                }
                char b_file[fileNameLength + 1];
                sprintf(b_file, "%s\n", file);
                strcat(buffer, (const char *)b_file);
                index_w = index_w + fileNameLength + 1;
            }
        }
    }

    //---------------
    int fileNameLength = strlen(end_str);

    if (index_w + fileNameLength + 1 > 1024)
    {
        curFileIndex = sizeof(infoFile.Longfile);
        return;
    }

    strcat(buffer, (const char *)end_str);
    index_w += strlen(end_str);
    //-----------

    begin_extcute = false;
    complete = true;
    curFileIndex = 0;
    index_w = 0;
}

FRESULT scan_files(char *path) /* Start node to be scanned (***also used as work area***) */
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path); /* Open the directory */
    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno); /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0)
                break; /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)
            { /* It is a directory */
                i = strlen(path);

                //strcat(buffer, &path[i]);
                // strcat(buffer, fno.fname);

                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path); /* Enter the directory */
                if (res != FR_OK)
                    break;
                path[i] = 0;
            }
            else
            { /* It is a file. */

                //strcat(buffer, path);

                int fileNameLength = strlen(fno.fname);
                char b_file[fileNameLength + 1];
                sprintf(b_file, "%s\n", fno.fname);
                strcat(buffer, (const char *)b_file);

                //printf("%s/%s\n", path, fno.fname);
            }
        }
        //f_closedir(&dir);
    }

    return res;
}