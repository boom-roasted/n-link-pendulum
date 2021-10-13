main: main.cpp
	g++ main.cpp -o main -Wall

.PHONY: sim
sim:
data.bin: main
	./main

.PHONY: plot
plot: data.bin
	python plot.py data.bin

.PHONY: clean
clean:
	rm -f main

.PHONY: clean-data
clean-data:
	rm -f data.bin
