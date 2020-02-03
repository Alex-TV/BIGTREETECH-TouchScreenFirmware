#include "CommandM20.h"
#include "includes.h"

bool begin_extcute = false;
bool complete = false;
char *buffer;
int index_w;
int curFileIndex;

char begin_str[] = "Begin file list\n";
char end_str[] = "End file list\n";
char error_str[] = "Error\n";

void StartExtcute()
{
    buffer = malloc(1024);
    while (!buffer)
        ; // malloc failed
    memset(buffer, 0, 1024);
    if (!begin_extcute)
    {
        strcat(buffer, (const char *)begin_str);
        index_w += strlen(begin_str);
    }
    FillBuffer();
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
                //buffer[index_w] = '\n';
            }
        }
    }
    int fileNameLength = strlen(end_str);

    if (index_w + fileNameLength + 1 > 1024)
    {
        curFileIndex = sizeof(infoFile.Longfile);
        return;
    }

    strcat(buffer, (const char *)end_str);
    index_w += strlen(end_str);
    begin_extcute = false;
    complete = true;
    curFileIndex = 0;
    index_w = 0;
}