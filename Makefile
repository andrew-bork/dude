
# LINKER_FLAGS:=-lSDL2

.PHONY: all

all: bin/game bin/SDL3.dll
	./bin/game

bin/lSDL3.dll:
	cp ./lib/SDL3.dll ./bin/SDL3.dll

IMGUI_SRCS:=${wildcard src/imgui/imgui_*.cpp}
IMGUI_OBJS:=${patsubst src/imgui/%.cpp,build/%.o,${IMGUI_SRCS}}
IMGUI_BACKEND_SRCS:=${wildcard src/imgui/backend/*.cpp}
IMGUI_BACKEND_OBJS:=${patsubst src/imgui/backend/%.cpp,build/%.o,${IMGUI_SRCS}}
test:
	echo ${IMGUI_OBJS}

${IMGUI_OBJS}: build/%.o: src/imgui/%.cpp
	g++ $^ -o $@ -Isrc 
${IMGUI_BACKEND_OBJS}: build/%.o: src/imgui/backend/%.cpp
	g++ $^ -o $@ -Isrc 

# %.o: build/%.o

bin/game: ${wildcard src/*/*.cpp} ${wildcard src/*.cpp} src/imgui/backend/imgui_impl_opengl3.cpp src/imgui/backend/imgui_impl_SDL3.cpp
	cp ./lib/SDL3.dll ./bin/SDL3.dll
	g++ $^ -o $@ -L"C:\Users\Andrew\Documents\VSCode\dude\lib" -lSDL3 -Isrc -lopengl32 -lassimp-5 -Iinclude

