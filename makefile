# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -w -I/usr/local/include -L/usr/local/lib -std=c++11

# Libraries
LIBS = -lssl -lcrypto

# Source Files
SRCS = main.cpp storage.cpp login.cpp AES.cpp

# Object Files
OBJS = $(SRCS:.cpp=.o)

# Executable Name
TARGET = main.exe

# Build Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
	rm -f $(OBJS) $(TARGET)
