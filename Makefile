CXX = g++
CXXLIB = -lssl -lcrypto
EXE = dictionary

default: $(EXE)

$(EXE): main.cpp json11/json11.cpp
	$(CXX) -o $@ $^ $(CXXLIB)
