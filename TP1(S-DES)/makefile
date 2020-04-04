CC = g++
CCFLAGS = -Wall -std=c++11
EXTC = cpp
EXTH = h
EXECUTABLE = main

SRC = $(wildcard *.$(EXTC))
HDR = $(wildcard *.$(EXTH))
OBJ = $(SRC: $(EXETC) = .o)

all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJ)
	@$(CC) $(CCFLAGS) -o $@ $^

%.o: %.$(EXTC) %.$(EXTH)
	@$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -rf *.o *.gch
	rm -f $(EXECUTABLE)
	
run : $(EXECUTABLE)
	./$(EXECUTABLE)