BIN=ViconRelay
SRC=ViconRelay.cpp imageio.cpp


GLEW_INC=-I/opt/viz/include
GLEW_LIBS=-Wl,-rpath,/opt/viz/lib -L/opt/viz/lib -lGLEW
BOOST_INC=-I../boost_1_53_0/

VICON_LIBS=-L/research/emvasey/ilSoP-OSC/vicon-libs -Wl,-rpath,/research/emvasey/ilSoP-OSC/vicon-libs -lViconDataStreamSDK_CPP

BASIC_LIBS=-L/share/apps/glew/1.9.0/lib -lGLEW -lglut -lX11 -lGL -lGLU -lstdc++ -lc -lm -pthread -lncurses
MAGICK_LIBS=`Magick-config --ldflags --libs --cflags`

ALL_LIBS = ${VICON_LIBS} ${GLEW_LIBS} ${BASIC_LIBS} ${MAGICK_LIBS} 
ALL_INC  = ${GLEW_INC} ${BOOST_INC}

${BIN}: Client.h ${SRC}
	g++ -fpermissive -g -Wall -v ${SRC} -o ${BIN} ${ALL_INC} ${ALL_LIBS}

clean:
	rm ${BIN}
