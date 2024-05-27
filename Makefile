CXX=g++
RM=rm -f
CPPFLAGS=-std=c++14 # Teensy supports c++14
LDFLAGS=
LDLIBS=

ms43CanHead=$(wildcard ./ms43-can/*.h)
ms43CanImpl=$(wildcard ./ms43-can/*.cpp)
ms43CanTestImpl=$(wildcard ./ms43-can-test/*.cpp)

nextionHead=$(wildcard ./nextion/*.h)
nextionImpl=$(wildcard ./nextion/*.cpp)
nextionTestImpl=$(wildcard ./nextion-test/*.cpp)

build: bin/ms43CanTest bin/nextionTest

bin/ms43CanTest: $(ms43CanTestImpl) $(ms43CanHead) $(ms43CanImpl)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o bin/ms43CanTest $(ms43CanTestImpl) $(ms43CanImpl) $(LDLIBS)

bin/nextionTest: $(nextionTestImpl) $(nextionHead) $(nextionImpl)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o bin/nextionTest $(nextionTestImpl) $(nextionImpl) $(LDLIBS)

test: build
	bin/ms43CanTest
	bin/nextionTest

clean:
	$(RM) bin/ms43CanTest bin/nextionTest
