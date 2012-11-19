
#include "util/Config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

RPiRevision getRPiRevision()
{
#ifdef RASPBERRY_PI
    static RPiRevision revision = (RPiRevision)-1;
    if(revision != -1)
        return revision;

    // only update if it is needed
    FILE* file = fopen("/proc/cpuinfo", "r");
    if(file == NULL)
    {
        revision = Unknown;
        return revision;
    }

    char line[128];
    while( fgets(line, sizeof line, file) != NULL)
    {
        if(strcmp(line, "CPU revision") == 0)
        {
            const char* strRevision = strchr(line, ':') + start + 2;

            int rev = atoi(strRevision);
            switch(rev)
            {
            case 2:
            case 3:
                revision = Revision1;
                break;
            case 4:
            case 5:
            case 6:
                revision = Revision2;
                break;
            }

            break;
        }
    }

    fclose(file);

    return revision;
#else
    return Unknown;
#endif
}

/*
// TODO: rename and move this to a better place
// and think about enum instead of integer values
int getRevision()
{
#ifdef RASPBERRY_PI
    static int revision = -2;
    if(revision != -2)
        return revision;

    // only update if it is needed
    int fCpuInfo = open("/proc/cpuinfo", O_RDONLY);
    if(fCpuInfo < 0)
    {
        revision = -1;
        return revision;
    }


#else
    return -1;
#endif
}
*/
