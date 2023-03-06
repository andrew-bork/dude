.PHONY: all clean sdl

all: bin/game bin/SDL3.dll
	./bin/game

bin/SDL3.dll: bin
	powershell cp lib/SDL3.dll bin/SDL3.dll

test:
	echo ${patsubst src/imgui/backend/%.cpp,build/%.o,${wildcard src/imgui/backend/*.cpp}}

bin:
	mkdir bin
build:
	mkdir build

IMGUI_OBJS:=${patsubst src/imgui/%.cpp,build/%.o,${wildcard src/imgui/*.cpp}}

${patsubst src/imgui/backend/%.cpp,build/%.o,${wildcard src/imgui/backend/*.cpp}}:build/%.o:src/imgui/backend/%.cpp build
	g++ -c -o $@ -Isrc -Iinclude ${filter %.cpp,$^}
${IMGUI_OBJS}:build/%.o:src/imgui/%.cpp build
	g++ -c -o $@ -Isrc -Iinclude ${filter %.cpp,$^}

bin/game: ${IMGUI_OBJS} ${wildcard src/*.cpp} build/imgui_impl_opengl3.o build/imgui_impl_sdl3.o bin
	g++ -o $@ -Iinclude ${filter %.cpp,$^} ${filter %.o,$^} -Llib -lSDL3 -Isrc -lOpengl32

sdl:
	git clone https://github.com/libsdl-org/SDL.git tmp/sdl
	cmake -G "MinGW Makefiles" -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -S tmp/sdl -B tmp/sdl/build
	echo "cd tmp/sdl/build;make;cd ../../.."
	echo "cp tmp/sdl/build/SDL3.dll lib/SDL3.dll"

clean:
	-powershell rm -r bin
	-powershell rm -r build
	-powershell rm -r tmp