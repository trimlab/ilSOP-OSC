SLVEXEC=GestureResponseSlave
MSTEXEC=GestureResponseMaster

HEADERS=

SLVSOURCE=GestureResponseSlave.cpp
MSTSOURCE=GestureResponseMaster.cpp
CC=g++

FLAGS=-O2

LIBS=-L/share/apps/glew/1.9.0/lib -lGLEW -lglut -lX11 -lGL -lGLU -lstdc++ -lc -lm -pthread -lncurses
VICONLIBS=-L../vicon-libs -Wl,-rpath,../vicon-libs -lViconDataStreamSDK_CPP
OSCLIBS=-L../oscpack/lib -Wl,-rpath,../oscpack/lib -loscpack

INCLUDES= -I../boost_1_53_0/ -I../oscpack/include/
all: $(SLVEXEC) $(MSTEXEC)

$(SLVEXEC): $(SLVSOURCE) $(HEADERS)
	$(CC) -fpermissive $(SLVSOURCE) -o $(SLVEXEC)  $(INCLUDES) $(LIBS)

$(MSTEXEC): $(MSTSOURCE) $(HEADERS)
	$(CC) -fpermissive $(MSTSOURCE) -o $(MSTEXEC)  $(INCLUDES) $(LIBS) $(VICONLIBS) $(OSCLIBS)

clean:
	rm -f $(SLVEXEC) $(MSTEXEC) *.o
