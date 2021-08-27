build:
	g++ main.cpp -o main -Wall

run:
	./main

clean:
	rm -f main
	rm -f data.bin

plot:
	python pendulumplot.py data.bin
