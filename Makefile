SDL_LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
PLOT_SOURCES = pendulumplot.cpp plot/Texture.cpp plot/Dot.cpp plot/MainWindow.cpp

.PHONY: sim clean pyplot plot format check-format

sim:
	g++ pendulumsim.cpp -o pendulumsim -Wall
	./pendulumsim

clean:
	rm -f pendulumsim
	rm -f pendulumplot
	rm -f data.bin

pyplot:
	python pendulumplot.py data.bin

plot:
	g++ $(PLOT_SOURCES) $(SDL_LINKER_FLAGS) -o pendulumplot -Wall
	./pendulumplot

# To create format file:
# clang-format -style=Mozilla -dump-config > .clang-format
format:
	clang-format -i *.cpp *.h **/*.cpp **/*.h

check-format:
	clang-format -i -Werror --dry-run *.cpp *.h **/*.cpp **/*.h
