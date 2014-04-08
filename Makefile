CC = g++
OUTPUTNAME = LKTrack
INCLUDE = -I/usr/include/flycapture -I/usr/include/opencv2
LIBS = -lflycapture -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video
CFLAGS = -Wall -Wextra -std=c++0x

OUTDIR = ./bin/

OBJS = Display.o Firefly.o LKTrack.o Timer.o Tracker.o

${OUTPUTNAME}: ${OBJS}
	${CC} -o ${OUTPUTNAME} ${OBJS} ${LIBS} ${COMMON_LIBS} 
	mv ${OUTPUTNAME} ${OUTDIR}

%.o: %.cpp
	${CC} ${CFLAGS} ${INCLUDE} -c $*.cpp
	
clean_obj:
	rm -f ${OBJS}

clean:
	rm -f ${OUTDIR}/${OUTPUTNAME} ${OBJS}
