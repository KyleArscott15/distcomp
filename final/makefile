
CC       =  g++
FLAGS    = -O3 -Wall -std=gnu++0x -fopenmp -g -pg #-E # -acc -fast -tesla,cc35 -Minfo=accel -Minform=inform 
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
LDFLAGS  = -lm -L.

ifeq ($(MAKECMDGOALS),)
else
	CFLAGS+=-DBULB
	CXXFLAGS+=-DBULB
endif

PROGRAM_NAME=mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)
	@mkdir -p images

mandelbulb: main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)
	@mkdir -p images

video: mandelbulb 
	@mkdir -p images
	@rm -f images/{*.bmp*,*.mp4}
	@./mandelbulb paramsBulb.dat
	@./video.sh

clean:
	rm -f *.o mandelbulb $(PROGRAM_NAME) $(EXEEXT) *~

