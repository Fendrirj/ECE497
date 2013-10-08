#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#include "beaglebone_gpio.h"

int keepgoing = 1;

void signal_handler(int sig);
void signal_handler(int sig){
	printf("Cleaning up...\n");
	keepgoing=0;
}

int main(int argc, char *argv[]){
	volatile void *gpio0_addr, *gpio1_addr;
	volatile unsigned int *gpio0_oe_addr, *gpio1_oe_addr;
	volatile unsigned int *gpio0_datain, *gpio1_datain;
	volatile unsigned int *gpio0_setdataout_addr, *gpio1_setdataout_addr;
	volatile unsigned int *gpio0_cleardataout_addr,*gpio1_cleardataout_addr;
	unsigned int reg;

	int fd = open("/dev/mem", O_RDWR);

	gpio0_addr = mmap(0, GPIO0_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO0_START_ADDR);	
	gpio1_addr = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_START_ADDR);

	gpio0_oe_addr = gpio0_addr + GPIO_OE;
	gpio0_setdataout_addr = gpio0_addr + GPIO_SETDATAOUT;
	gpio0_cleardataout_addr = gpio0_addr + GPIO_CLEARDATAOUT;
	gpio0_datain = gpio0_addr + GPIO_DATAIN;

	gpio1_oe_addr = gpio1_addr + GPIO_OE;
	gpio1_setdataout_addr = gpio1_addr + GPIO_SETDATAOUT;
	gpio1_cleardataout_addr = gpio1_addr + GPIO_CLEARDATAOUT;
	gpio1_datain = gpio1_addr + GPIO_DATAIN;

	if(gpio0_addr == MAP_FAILED){
		printf("Unable to map GPIO0\n");
		exit(1);
	}
	if(gpio1_addr == MAP_FAILED){
		printf("Unable to map GPIO1\n");
		exit(1);
	}

	signal(SIGINT, signal_handler);

	reg = *gpio1_oe_addr;
	reg &= ~USR3; //Set USR0 bit to 0;
	reg &= ~USR2; //Set USR1 bit to 0;

	*gpio1_oe_addr = reg;

	while(keepgoing){
		if((*gpio0_datain & GPIO_30) == GPIO_30){
			*gpio1_cleardataout_addr = USR2;
		} else{
			*gpio1_setdataout_addr = USR2;
		}
		if((*gpio1_datain & GPIO_60) == GPIO_60){
			*gpio1_cleardataout_addr = USR3;
		} else{
			*gpio1_setdataout_addr = USR3;
		}
		usleep(1000);
	}

	munmap((void *) gpio0_addr, GPIO0_SIZE);
	munmap((void *) gpio1_addr, GPIO1_SIZE);
	close(fd);

}
