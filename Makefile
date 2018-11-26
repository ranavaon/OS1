# Makefile for the smash program
CCC = g++
CXXFLAGS = -g -Wall -std=c++0x
CXXLINK = $(CCC)
OBJS = smash.o commands.o signals.o data.o
TARGET=smash
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CXXLINK) -o smash $(OBJS)
# Creating the object files
data.o: data.cpp data.h
signals.o: signals.cpp signals.h data.h
commands.o: commands.cpp commands.h data.h signals.h
smash.o: smash.cpp signals.h commands.h data.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

