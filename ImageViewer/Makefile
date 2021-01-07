CC = g++
OBJS = DisplayImage.o
FLAGS = -Wall -g
MAIN = DisplayImage
LIBS = -I/usr/include/opencv -lopencv_core -lopencv_highgui -lopencv_imgcodecs
SRC = DisplayImage.cpp

$(MAIN):$(OBJS)
	$(CC) $(FLAGS) -o $(MAIN) $(OBJS) $(LIBS)

$(OBJS): $(SRC)
	$(CC) $(FLAGS) -c $(SRC)

clean:
	rm $(OBJS) $(MAIN)