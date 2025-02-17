
### check to make sure you're in Proj2/proj2 to run make

CXXFLAGS = -std=c++17 -I ./include/
# CXXFLAGS = -std=c++17 -I ./include/ -mmacosx-version-min=13.6

# updated from:
# PATHS = -isystem /user/src/googletest/googletest/include -pthread /usr/lib/libgtest.a /usr/lib/libgtest_main.a
PATHS = -isystem /usr/local/include -pthread /usr/local/lib/libgtest.a /usr/local/lib/libgtest_main.a

# define and use LDFLAGS
LDFLAGS = -lgtest -lgtest_main -pthread -L/usr/local/lib -I/usr/local/include \
          -L/opt/homebrew/opt/expat/lib -I/opt/homebrew/opt/expat/include -lexpat

SRC_DIR = src
TEST_DIR = testsrc
OBJ_DIR = obj
BIN_DIR = bin

# -lgtest -lgtest_main
# -I ./include/
# $(OBJ_DIR)/StringDataSinkTest.o $(OBJ_DIR)/StringDataSourceTest.o
OBJ = $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o \
	$(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o  \
	$(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o \
	$(OBJ_DIR)/DSVReaderTest.o $(OBJ_DIR)/DSVWriterTest.o  $(OBJ_DIR)/XMLTest.o

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

# $(OBJ_DIR)/StringDataSinkTest.o: $(TEST_DIR)/StringDataSinkTest.cpp
# 	g++ $(CXXFLAGS) -c $(TEST_DIR)/StringDataSinkTest.cpp -o $(OBJ_DIR)/StringDataSinkTest.o

# $(OBJ_DIR)/StringDataSourceTest.o: $(TEST_DIR)/StringDataSourceTest.cpp
# 	g++ $(CXXFLAGS) -c $(TEST_DIR)/StringDataSourceTest.cpp -o $(OBJ_DIR)/StringDataSourceTest.o

$(OBJ_DIR)/DSVWriterTest.o: $(TEST_DIR)/DSVWriterTest.cpp
	g++ $(CXXFLAGS) -c $(TEST_DIR)/DSVWriterTest.cpp -o $(OBJ_DIR)/DSVWriterTest.o

$(OBJ_DIR)/DSVReaderTest.o: $(TEST_DIR)/DSVReaderTest.cpp
	g++ $(CXXFLAGS) -c $(TEST_DIR)/DSVReaderTest.cpp -o $(OBJ_DIR)/DSVReaderTest.o

$(OBJ_DIR)/XMLTest.o: $(TEST_DIR)/XMLTest.cpp
	g++ $(CXXFLAGS) -c $(TEST_DIR)/XMLTest.cpp -o $(OBJ_DIR)/XMLTest.o

# magic
$(EXEC): bin $(OBJ)
	g++ -std=c++17 $(OBJ) $(LDFLAGS) -o $(EXEC)
# $(EXEC): bin $(OBJ)
# 	g++ -std=c++17 -lgtest -lgtest_main -lpthread $(PATHS) -o $(EXEC) $(OBJ)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf obj bin