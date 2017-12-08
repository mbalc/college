punkty: punkty.cc
	g++ -Wall -Wextra -O2 -std=c++17 punkty.cc -o punkty

punkty.g: punkty.cc
	g++ -Wall -Wextra -pedantic -g -std=c++17 punkty.cc -o punkty.g

alt: punktyAlt.cpp
	g++ -Wall -Wextra -O2 -std=c++17 $< -o punkty

debug: punkty.g
	valgrind --leak-check=full --show-leak-kinds=all ./punkty.g private/punkty0.file < private/punkty0.in


test: punkty
	./punkty private/punkty0.file < private/punkty0.in

clean clear:
	rm -rf punkty punkty.g
