TOOLS_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes
CFLAGS  := -O3 -g
#
# Programs
#
all:	ledswitch gpioThru	

ledswitch: ledswitch.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
gpiothru: gpioThru.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm ledswitch gpioThru

print:
	@echo LDFLAGS = $(LDFLAGS)
