CPP := g++
CPPFLAGS := -I include/ -L lib/
LIBFLAGS := -lglfw3 -lopengl32 -lgdi32
SRC_FILES := src/*.cpp src/glad.c

EXEC := main.exe

all:
	$(CPP) $(SRC_FILES) -o bin/$(EXEC) -O3 $(CPPFLAGS) $(LIBFLAGS)

debug:
	$(CPP) $(SRC_FILES) -o bin/$(EXEC) -O3 $(CPPFLAGS) $(LIBFLAGS) -g

run:
	cd bin && ./$(EXEC)
