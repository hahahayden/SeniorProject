CC = g++
FLAGS = -mfpu=neon -mcpu=cortex-a8 -ftree-vectorize -std=c++11 -Ofast -Wall -g -I/usr/include/opencv 
MAIN = Camera
LIBS = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -lpthread
INCLUDES = 
OBJS = Camera.o

$(MAIN):$(OBJS)
	$(CC) $(FLAGS) -o $(MAIN) $(OBJS) $(LIBS)

%.o: %.cpp $(INCLUDES)
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(MAIN)