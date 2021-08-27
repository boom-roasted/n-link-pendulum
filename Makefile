build:
	g++ pendulumsim.cpp -o pendulumsim -Wall

run:
	./pendulumsim

clean:
	rm -f pendulumsim
	rm -f data.bin

plot:
	python pendulumplot.py data.bin
