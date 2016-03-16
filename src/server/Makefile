TARGETS_DIR = ../../bin
TARGETS = ${TARGETS_DIR}/stdio ${TARGETS_DIR}/unix_domain_socket

TESTS_INSTALL_DIR = ../../tests/bin/server
GTEST_OBJ_DIR = ${TESTS_INSTALL_DIR}/../gtest
GTEST_DIR = ../third_party/googletest/googletest
GTEST_DEPS = ${GTEST_OBJ_DIR}/libgtest_main.a ${GTEST_OBJ_DIR}/libgtest.a

CXX=g++
# CXX = clang++
WARNING_OPTIONS = -Wall -Wextra -Woverloaded-virtual -Werror # -fcolor-diagnostics
INCLUDE_OPTIONS = -I../ -I${GTEST_DIR}/include/ -I../third_party
CXXFLAGS = $(WARNING_OPTIONS) $(INCLUDE_OPTIONS) -std=c++11
TEST_CXXFLAGS = $(INCLUDE_OPTIONS) -std=c++11

TEST_SOURCES = $(wildcard *_test.cpp)

TESTS = $(patsubst %_test.cpp, ${TESTS_INSTALL_DIR}/%_test, $(TEST_SOURCES))

DEPS = ../engine/engine.o ../dictionary/map_dictionary.o ../converter/bigram.o ../converter/bigram_converter.o request.o ../converter/lattice.o ../converter/lattice_builder.o


all:${TARGETS_DIR} $(TARGETS)

test: ${TESTS_INSTALL_DIR} ${TESTS}

clean:
	rm -rf  ../base/*.o ./*.o ${TESTS} ${TESTS_INSTALL_DIR} ${TARGETS}

${TARGETS_DIR}:
	@mkdir -p $@

${TESTS_INSTALL_DIR}:
	@mkdir -p $@

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

%_test.o: %_test.cpp
	$(CXX) -o $@ -c $< $(TEST_CXXFLAGS)

../base/%.o: ../base/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

${TARGETS_DIR}/stdio:stdio.cpp ${DEPS}
	$(CXX) -o $@ $^ $(CXXFLAGS)

${TARGETS_DIR}/unix_domain_socket:unix_domain_socket.cpp ${DEPS}
	$(CXX) -o $@ $^ $(CXXFLAGS)
