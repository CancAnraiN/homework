SRCS = client.c 
EXT  = .c
OBJS = $(SRCS:$(EXT)=.o)
EXEC = client
CXX  = gcc
#CXXFLAGS = -std=c99 -Wall -g -D _DEBUG 

DEP  = dep

all:$(EXEC) clean

$(EXEC):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

%.o:%$(EXT)
	$(CXX) $(CXXFLAGS) -c $<

dep:
	$(CXX) $(CXXFLAGS) -M $(SRCS) > $(DEP)

clean:
	rm -rf $(OBJS) $(DEP)
