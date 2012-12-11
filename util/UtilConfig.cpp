// This file had to be renamed to UtilConfig.cpp from Config.cpp as this was leading a name conflict

#include "util/Config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief getRPiRevision checks the revision of the raspberry pi board. There are some minor differences between revisions, e.g. I2C pin assignment
 * @return
 */
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
        int rev;
        if( sscanf(line, "Revision : %x", &rev) != 0)
        {/*
        if(strncmp(line, "Revision", 8) == 0)
        {
            const char* strRevision = strchr(line, ':') + 2;

            int rev = atoi(strRevision);*/
            switch(rev)
            {
            case 2:
            case 3:
                revision = Revision1;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 0xf:
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

