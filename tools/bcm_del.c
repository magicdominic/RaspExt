// based on http://www.open.com.au/mikem/bcm2835/
 
 
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

/* BSC register offsets */
#define BSC_C			0x00
#define BSC_S			0x04
#define BSC_DLEN		0x08
#define BSC_A			0x0c
#define BSC_FIFO		0x10
#define BSC_DIV			0x14
#define BSC_DEL			0x18
#define BSC_CLKT		0x1c
 
static volatile uint32_t *i2c0 = MAP_FAILED;
 
// Physical addresses for various peripheral regiser sets
/// Base Physical Address of the BCM 2835 peripheral registers
#define BCM2835_PERI_BASE               0x20000000
// i2c base
#define BCM2835_I2C0_BASE                (BCM2835_PERI_BASE + 0x205000)
#define BCM2835_I2C1_BASE		 (BCM2835_PERI_BASE + 0x804000)
 
/// Size of memory page on RPi
#define BCM2835_PAGE_SIZE               (4*1024)
/// Size of memory block on RPi
#define BCM2835_BLOCK_SIZE              (4*1024)
 
 
static  int     fd = -1;
static  uint8_t *i2c0Mem = NULL;
static  debug   = 0;
 

// safe write to peripheral
void bcm2835_peri_write(volatile uint32_t* paddr, uint32_t value)
{
    if (debug)
    {
        printf("bcm2835_peri_write paddr %08X, value %08X\n", paddr, value);
    }
    else
    {
        // Make sure we dont rely on the firs write, which may get
        // list if the previous access was to a different peripheral.
        *paddr = value;
        *paddr = value;
    }
}

// *****************************************************************************
// I2C write register using register offset
// *****************************************************************************
void bcm2835_write_i2c(uint8_t offset,uint32_t value)
{
    volatile uint32_t* paddr;
    if(offset)
        paddr = i2c0 + offset/4;
    else    
        paddr = i2c0;
    bcm2835_peri_write(paddr, value);
}


// Just sets delay I2C, returns 0 on sucess
int bcm2835_i2c_setdel(uint32_t delay )
{
    // memory / register access
    uint8_t *mapaddr;
    if ((fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0)
    {
        fprintf(stderr, "bcm2835_init: Unable to open /dev/mem: %s\n", strerror(errno)) ;
        return -1;
    }
   
    if ((i2c0Mem = malloc(BCM2835_BLOCK_SIZE + (BCM2835_PAGE_SIZE - 1))) == NULL)
    {
        fprintf(stderr, "bcm2835_init: i2c0mem malloc failed: %s\n", strerror(errno)) ;
        return -1;
    }
   
    mapaddr = i2c0Mem;
    if (((uint32_t)mapaddr % BCM2835_PAGE_SIZE) != 0)
        mapaddr += BCM2835_PAGE_SIZE - ((uint32_t)mapaddr % BCM2835_PAGE_SIZE) ;

    // we want to use this program on both i2c0 and i2c1, so we have to repeat the steps below for the other port

    i2c0 = (uint32_t *)mmap(mapaddr, BCM2835_BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, BCM2835_I2C0_BASE) ;

    if ((int32_t)i2c0 < 0)
    {
        fprintf(stderr, "bcm2835_init: mmap failed (i2c0): %s\n", strerror(errno)) ;
        return -1;
    }
    
    // Set bus requirements
    bcm2835_write_i2c(BSC_DEL, delay);


    // now comes i2c1
    i2c0 = (uint32_t *)mmap(mapaddr, BCM2835_BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, BCM2835_I2C1_BASE) ;

    if ((int32_t)i2c0 < 0)
    {
        fprintf(stderr, "bcm2835_init: mmap failed (i2c1): %s\n", strerror(errno)) ;
        return -1;
    }
    
    // Set bus requirements
    bcm2835_write_i2c(BSC_DEL, delay);


    return 0;
}
 
 
int main(int argc, char **argv)
{
    if(bcm2835_i2c_setdel(0x06300630)) {
        fprintf(stderr, "bcm2835: failed (i2c): %s\n", strerror(errno));
        exit(1);
    }
    exit(0);
}
