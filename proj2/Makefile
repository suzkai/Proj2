
### check to make sure you're in Proj2/proj2 to run make

CXXFLAGS = -std=c++17 -I ./include/
PATHS = -isystem /user/src/googletest/googletest/include -pthread /usr/lib/libgtest.a /usr/lib/libgtest_main.a

SRC_DIR = src
TEST_DIR = testsrc
OBJ_DIR = obj
BIN_DIR = bin

# -lgtest -lgtest_main
# -I ./include/

OBJ = $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o \
	$(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o  \
	$(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o \
	$(OBJ_DIR)/StringDataSinkTest.o $(OBJ_DIR)/StringDataSourceTest.o \
	$(OBJ_DIR)/DSVTest.o

EXEC = $(BIN_DIR)/exec

all: dir $(EXEC) run

dir:
	mkdir -p obj
	mkdir -p bin

# compiling cpp to o
$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp
	g++ $(CXXFLAGS) -c $(SRC_DIR)/DSVReader.cpp -o $(OBJ_DIR)/DSVReader.o

$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp
	g++ $(CXXFLAGS) -c $(SRC_DIR)/DSVWriter.cpp -o $(OBJ_DIR)/DSVWriter.o

$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp
	g++ $(CXXFLAGS) -c $(SRC_DIR)/StringDataSink.cpp -o $(OBJ_DIR)/StringDataSink.o

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp
	g++ $(CXXFLAGS) -c $(SRC_DIR)/StringDataSource.cpp -o $(OBJ_DIR)/StringDataSource.o

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp
	g++ $(CXXFLAGS) -c $(SRC_DIR)/XMLReader.cpp -o $(OBJ_DIR)/XMLReader.o

$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp
	g++ $(CXXFLAGS) -c $(SRC_DIR)/XMLWriter.cpp -o $(OBJ_DIR)/XMLWriter.o

$(OBJ_DIR)/StringDataSinkTest.o: $(TEST_DIR)/StringDataSinkTest.cpp
	g++ $(CXXFLAGS) -c $(TEST_DIR)/StringDataSinkTest.cpp -o $(OBJ_DIR)/StringDataSinkTest.o

$(OBJ_DIR)/StringDataSourceTest.o: $(TEST_DIR)/StringDataSourceTest.cpp
	g++ $(CXXFLAGS) -c $(TEST_DIR)/StringDataSourceTest.cpp -o $(OBJ_DIR)/StringDataSourceTest.o

$(OBJ_DIR)/DSVTest.o: $(TEST_DIR)/DSVTest.cpp
	g++ $(CXXFLAGS) -c $(TEST_DIR)/DSVTest.cpp -o $(OBJ_DIR)/DSVTest.o

# magic
$(EXEC): bin $(OBJ)
	g++ -std=c++17 -lgtest -lgtest_main -lpthread $(PATHS) -o $(EXEC) $(OBJ)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf obj bin