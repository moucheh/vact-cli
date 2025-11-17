all: vact

vact: main.cpp
	clang++ -std=c++20 main.cpp -o vact

install: vact
	./scripts/install.sh

uninstall:
	./scripts/uninstall.sh

clean:
	rm vact
