SDL_LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
PLOT_SOURCES = pendulumplot.cpp plot/Texture.cpp plot/Dot.cpp

build:
	g++ pendulumsim.cpp -o pendulumsim -Wall
	g++ $(PLOT_SOURCES) $(SDL_LINKER_FLAGS) -o pendulumplot -Wall

sim:
	./pendulumsim

clean:
	rm -f pendulumsim
	rm -f data.bin

plot:
	python pendulumplot.py data.bin

plottest:
	./pendulumplot
