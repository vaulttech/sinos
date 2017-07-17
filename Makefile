CC = g++

FLAGS = -Wall

OBJS =  ObjectModel.o Object.o ObjectStick.o ObjectBall.o Material.o LightInfo.o Level.o Camera.o Game.o utils.o
OBJLIB = lib/glm.o lib/imageloader.o lib/Texture.o

LIBS = -lGL -lglut -lGLU

BIN = sinos

%.o: %.cpp %.h
	$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) -g


all: opengl.cpp $(OBJS) $(OBJLIB)
	$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) -g


clean:
	rm -f sinos
	rm -f *.o
	rm -f lib/*.o
