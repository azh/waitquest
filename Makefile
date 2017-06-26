SRC= $(wildcard *.cpp)
LDFLAGS=-lncurses
CXXFLAGS+=-MP -MD
# ^ is a delimited list of pre-requisites, @ is the current target name

.PHONY: wq wqtest clean

wq:
	g++ $(SRC) -o $@ $^$(LDFLAGS) -pthread -std=c++1y

wqtest:
	g++ $(SRC) -o $@ $^$(LDFLAGS) -pthread -std=c++1y

clean:
	rm main

-include $(SRC:%.cpp=%.d)

