#ifndef I2CPOLL_H
#define I2CPOLL_H

class I2CPolling
{
public:
    virtual void poll(int fd) = 0;
};

#endif // I2CPOLL_H
