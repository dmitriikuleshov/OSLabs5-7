files: calc server

calc: src/calculation_node.cpp
	mkdir -p build
	g++ -fsanitize=address src/calculation_node.cpp -lzmq -o build/calc -w

server: src/server.cpp
	mkdir -p build
	g++ -fsanitize=address src/server.cpp -lzmq -o build/server -w

clean:
	rm -rf build
