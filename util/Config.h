#ifndef RPI_H
#define RPI_H

// if we compile on a raspberry, we should include extra functionality
// maybe __arm__ is not a very good test, but I did not yet found a better one
#ifdef __arm__

#define RASPBERRY_PI
#define USE_GPIO
#define USE_I2C

#else

#undef RASPBERRY_PI
#undef USE_GPIO
#undef USE_I2C

#endif

//#define USE_BLUETOOTH

enum RPiRevision
{
    Revision1,
    Revision2,
    Unknown
};

RPiRevision getRPiRevision();

#endif // RPI_H
