MASTER_BIN=KeyboardMaster
MASTER_SRC=KeyboardMaster.cpp

SLAVE_BIN=KeyboardSlave
SLAVE_SRC=KeyboardSlave.cpp

GLEW_INC=-I/opt/viz/include
GLEW_LIBS=-Wl,-rpath,/opt/viz/lib -L/opt/viz/lib -lGLEW

BOOST_INC=-I../boost_1_53_0/
VICON_LIBS=-L/research/emvasey/ilSoP-OSC/vicon-libs -Wl,-rpath,/research/emvasey/ilSoP-OSC/vicon-libs -lViconDataStreamSDK_CPP
BASIC_LIBS=-lGLEW -lglut -lX11 -lGL -lGLU -lstdc++ -lc -lm -pthread -lncurses
MAGICK_LIBS=`Magick-config --ldflags --libs --cflags`

ALL_LIBS=${GLEW_LIBS} ${MAGICK_LIBS} ${VICON_LIBS} ${BASIC_LIBS}
ALL_INC=${GLEW_INC} ${BOOST_INC}

all: $(SLAVE_BIN) $(MASTER_BIN)

${MASTER_BIN}: Client.h ${MASTER_SRC}
	g++ -fpermissive -g -Wall -o ${MASTER_BIN} ${ALL_INC} ${MASTER_SRC} ${ALL_LIBS}

${SLAVE_BIN}: Client.h ${SLAVE_SRC}
	g++ -fpermissive -g -Wall -o ${SLAVE_BIN} ${ALL_INC} ${SLAVE_SRC} ${ALL_LIBS}


clean:
	rm ${MASTER_BIN} ${SLAVE_BIN}
