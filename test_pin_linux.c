/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAP_SIZE 		4096UL
#define MAP_MASK 		(MAP_SIZE - 1)
#define GPIO_DATA_OFFSET		0x00000000
#define GPIO_BASE_ADDR	0x43c00000
#define GPIO_HIGH_ADDR	0x43c0ffff
#define GPIO_ADDR_RANGE	0xffff
#define GPIO_DIRECTION_OFFSET 0
#define LED_1			0x00000001	// 335 nm
#define LED_2			0x00000002	// 335 nm
#define LED_3			0x00000003	// 375 nm
#define LED_4			0x00000004	// 410 nm
#define LED_5			0x00000005	// 410 nm
#define LED_6			0x00000006	// 410 nm
#define LED_7			0x00000007	// 470 nm
#define LED_8			0x00000008	// 470 nm
#define LED_9			0x00000009	// 525 nm
#define LED_10			0x0000000A	// 620 nm
#define REED			0x00000001	//REEDKontakschalter


int main()
{
    int memfd;
    void *mapped_base, *mapped_dev_base;
    off_t dev_base = GPIO_BASE_ADDR;

    memfd = open("/dev/mem", O_RDWR | O_SYNC);
        if (memfd == -1) {
        printf("Can't open /dev/mem.\n");
        exit(0);
    }
    printf("/dev/mem opened.\n");

    // Map one page of memory into user space such that the device is in that page, but it may not
    // be at the start of the page.

    mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
        if (mapped_base == (void *) -1) {
        printf("Can't map the memory to user space.\n");
        exit(0);
    }
     printf("Memory mapped at address %p.\n", mapped_base);

    // get the address of the device in user space which will be an offset from the base
    // that was mapped as memory is mapped at the start of a page

    mapped_dev_base = mapped_base + (dev_base & MAP_MASK);

    // write to the direction register so all the GPIOs are on output to drive LEDs

    *((volatile unsigned long *) (mapped_dev_base + GPIO_DIRECTION_OFFSET)) = 0;

    // toggle the output as fast as possible just to see how fast it works

    while (1) {
                   // If writes to multiple addresses were done:
                   //     may need memory barriers i.e. need a driver
                   //     caution with data being cached
        *((volatile unsigned long *) (mapped_dev_base + GPIO_DATA_OFFSET)) = 0;
        *((volatile unsigned long *) (mapped_dev_base + GPIO_DATA_OFFSET)) = 1;
    }

    // unmap the memory before exiting

    if (munmap(mapped_base, MAP_SIZE) == -1) {
        printf("Can't unmap memory from user space.\n");
        exit(0);
    }

    close(memfd);
    return 0;
}
