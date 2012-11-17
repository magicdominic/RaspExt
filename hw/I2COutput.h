#ifndef I2COUTPUT_H
#define I2COUTPUT_H

#include "util/Debug.h"
#include <string>

class I2COutput
{
public:
    virtual void setI2C(int fd) = 0;
};

#endif // I2COUTPUT_H
