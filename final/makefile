
CC       =  g++
FLAGS    = -O3 -Wall -std=gnu++0x -fopenmp -g -pg #-E
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
LDFLAGS  = -lm -L.

ifeq ($(MAKECMDGOALS),mandelbulb)
	CFLAGS+=-DBULB
	CXXFLAGS+=-DBULB
else
endif

PROGRAM_NAME=mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)

mandelbulb: main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o mandelbulb $(PROGRAM_NAME) $(EXEEXT) *~

