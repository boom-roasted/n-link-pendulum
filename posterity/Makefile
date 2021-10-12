.PHONY: build sim plot clean clean-data

build:
	g++ main.cpp -o main -Wall

sim:
	./main

plot:
	python plot.py data.bin

clean:
	rm -f main

clean-data:
	rm -f data.bin
