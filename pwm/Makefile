
CXX= g++
CXXFLAGS= -Wall -D__DEBUG -O2 -mtune=cortex-a8 -march=armv7-a

LD= g++
LD_LIBRARIES= -lprussdrv -lpthread -lm

ASM= pasm
ASMFLAGS= -V3 -cdl

DTC= dtc
DTCFLAGS= -@ -O dtb

CXX_SRCS= pruss_pwm.cc pulse_pwm.cc
OBJS= $(CXX_SRCS:.cc=.o)

BINS= pwm.bin
DTBOS= BB-BONE-PRU-00A0.dtbo

all: pulse_pwm $(BINS) $(DTBOS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.bin : %.p
	$(ASM) $(ASMFLAGS) -b $<

%.dtbo : %.dts
	$(DTC) $(DTCFLAGS) -o $@ $<

pulse_pwm: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o pulse_pwm $(LD_LIBRARIES)

clean:
	rm -f pulse_pwm *.o *.bin *.dbg *.lst
