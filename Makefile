EXE = nlinkpendulum
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
SOURCES = main.cpp src/Texture.cpp src/Dot.cpp src/MainWindow.cpp src/RenderableChain.cpp

.PHONY: build run clean format check-format

build:
	g++ $(SOURCES) $(LFLAGS) -o $(EXE) -Wall

run:
	./$(EXE)

clean:
	rm -f $(EXE)

# To create format file:
# clang-format -style=Mozilla -dump-config > .clang-format
format:
	clang-format -i *.cpp *.h **/*.cpp **/*.h

check-format:
	clang-format -i -Werror --dry-run *.cpp *.h **/*.cpp **/*.h
