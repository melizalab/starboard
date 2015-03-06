
CXX= g++
CXXFLAGS= -Wall -D__DEBUG -O2 -mtune=cortex-a8 -march=armv7-a

LD= g++
LD_LIBRARIES= -lprussdrv -lpthread -lm

ASM= pasm
ASMFLAGS= -V3 -cdl

CXX_SRCS= pruss_pwm.cc pulse_pwm.cc
OBJS= $(CXX_SRCS:.cc=.o)

BINS= pwm.bin

all: pulse_pwm $(BINS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.bin : %.p
	$(ASM) $(ASMFLAGS) -b $<

pulse_pwm: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o pulse_pwm $(LD_LIBRARIES)

clean:
	rm -f pulse_pwm *.o *.bin *.dbg *.lst
