CC = g++
OUTPUTNAME = acra_experiment
INCLUDE = -I/usr/include/flycapture -I/usr/include/opencv2
LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_video
CFLAGS = -Wall -Wextra -Weffc++ -pedantic -std=c++0x -O

OUTDIR = ./bin/

OBJS = main.o FeatureExtractors.o

${OUTPUTNAME}: ${OBJS}
	${CC} -o ${OUTPUTNAME} ${OBJS} ${LIBS} ${COMMON_LIBS} 
	mv ${OUTPUTNAME} ${OUTDIR}

%.o: %.cpp
	${CC} ${CFLAGS} ${INCLUDE} -c $*.cpp
	
clean_obj:
	rm -f ${OBJS}

clean:
	rm -f ${OUTDIR}/${OUTPUTNAME} ${OBJS}
