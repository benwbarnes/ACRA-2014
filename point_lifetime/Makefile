CC = g++
OUTPUTNAME = acra_experiment
INCLUDE = -I/usr/include/flycapture -I/usr/include/opencv2
LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_video -lopencv_videoio -lopencv_imgcodecs
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

# All OpenCV libraries:
#libopencv_calib3d.so
#libopencv_core.so
#libopencv_features2d.so
#libopencv_flann.so
#libopencv_highgui.so
#libopencv_imgcodecs.so
#libopencv_imgproc.so
#libopencv_ml.so
#libopencv_nonfree.so
#libopencv_objdetect.so
#libopencv_optim.so
#libopencv_photo.so
#libopencv_shape.so
#libopencv_stitching.so
#libopencv_superres.so
#libopencv_videoio.so
#libopencv_video.so
#libopencv_videostab.so
