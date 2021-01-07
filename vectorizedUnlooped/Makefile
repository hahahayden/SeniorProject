CC = g++
FLAGS = -mfpu=neon -mcpu=cortex-a8 -ftree-vectorize -std=c++11 -Ofast -Wall -g -I/usr/include/opencv -I/usr/local/include/perf -I/usr/local/lib/perf/include  -L/usr/local/lib -lperf
MAIN = vectorizedSobel
LIBS = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -lpthread
INCLUDES = sobel.hpp
OBJS = vectorizedSobel.o sobel.o

$(MAIN):$(OBJS)
	$(CC) $(FLAGS) -o $(MAIN) $(OBJS) $(LIBS)

%.o: %.cpp $(INCLUDES)
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(MAIN)