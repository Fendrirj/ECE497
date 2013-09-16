#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include "gpio-utils.h"


#define POLL_TIMEOUT (3*1000)
/*******************************
* Global Variables
********************************/
int keepgoing = 1; //Set to 0 when ctrl-c is pressed

void signal_handler(int sig){
	printf("\nCleaning up and exiting...\n");
	keepgoing = 0;
}

int main(int argc, char** argv){
	//Create an array to store GPIO FDs the switches are connected to
	int switchGPIOFDs[4];
	
	int switchPorts[4] = {50, 51, 3, 2};
	int ledPorts[4] = {30,31,48,60};
	
	int nfds = 5;
	int rc, timeout, value, i;
	struct pollfd fdset[5];

	timeout = POLL_TIMEOUT;

	for(i = 0; i<4 ; i++){
		gpio_export(switchPorts[i]);
		gpio_export(ledPorts[i]);

		gpio_set_dir(switchPorts[i], "in");
		gpio_set_dir(ledPorts[i], "out");

		gpio_set_edge(switchPorts[i], "both");
		
		switchGPIOFDs[i] = gpio_fd_open(switchPorts[i], O_RDONLY);
	}

	//Set signal callback for Ctrl-C
	signal(SIGINT, signal_handler);
	while(keepgoing){
		memset((void*)fdset, 0, sizeof(fdset));
		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;
		
		//Set the FDs for the switch GPIOs
		for(i = 0; i<4; i++){
			fdset[i+1].fd = switchGPIOFDs[i];
			fdset[i+1].events = POLLPRI;
		}

		rc = poll(fdset, nfds, timeout);
		
		if(rc < 0) {
			printf("\npoll() failed\n");
			return -1;
		}

		if(rc == 0){
			printf(".");
		}

		for(i=0; i<4; i++){
			if(fdset[i+1].revents & POLLPRI){
				gpio_get_value(switchPorts[i], &value);
				gpio_set_value(ledPorts[i], value);
			}
		}
		fflush(stdout);
	}
	for(i = 0; i<4; i++){
		gpio_fd_close(switchGPIOFDs[i]);
	}	
	return 0;
}
