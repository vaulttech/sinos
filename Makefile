CC = g++
INCLUDE = -I.

FLAGS = -Wall

OBJS =  src/ObjectModel.o src/Object.o src/ObjectStick.o src/ObjectBall.o src/Material.o src/LightInfo.o src/Level.o src/Camera.o src/Game.o src/utils.o
OBJLIB = lib/glm.o lib/imageloader.o lib/Texture.o

LIBS = -lGL -lglut -lGLU

BIN = sinos

src/%.o: src/%.cpp src/%.h
	$(CC) $(FLAGS) -c $< -o $@ $(INCLUDE) $(LIBS) -g


all: src/opengl.cpp $(OBJS) $(OBJLIB)
	$(CC) $(FLAGS) $^ -o $(BIN) $(INCLUDE) $(LIBS) -g


clean:
	rm -f sinos
	rm -f *.o
	rm -f lib/*.o
