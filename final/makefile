#-std=gnu++0x
CC       =  pgCC
CXX      =  pgCC
FLAGS    = -lm -fast -Mautoinline:maxsize:1000 -acc -mp -Minfo -O2 -ta=tesla,cc30 -pg  #-DVERSION_ACC #-DTEST -O3 -Wall -std=gnu++0x -fopenmp -g -pg #-E
ACCFLAGS = -DBULB -E -std=gnu++0x -I.  -fast -acc -Minfo -O2 -ta=tesla,cc30 #-DVERSION_ACC -g -pg
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
	@mkdir -p images

mandelbulb: main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)
	@mkdir -p images

acc: main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(ACCFLAGS) $(LDFLAGS)

video: $(PROGRAM_NAME) 
	@mkdir -p images
	@rm -f images/{*.bmp*,*.jpg*,*.mp4}
	@./mandelbox params.dat
	@./video.sh

clean:
	rm -f *.o mandelbulb $(PROGRAM_NAME) $(EXEEXT) *~

