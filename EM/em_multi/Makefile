GTEST_DIR = /home/xh/software/googletest

# Where to find user code.
EXE = em

# Flags passed to the preprocessor.
CPPFLAGS += -I$(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra

#LINKFLAGS += -lpthread -L${GTEST_DIR}/make -lgtest
# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.

all : $(EXE)

clean :
	rm -f $(EXE) 

em : em.cpp Matrix.h em.h
	g++ -lboost_thread em.cpp  -o em
# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.
test.o : Matrix.h test.cpp
	$(CXX) $(CPPFLAGS) -c test.cpp

test : test.o 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread ${GTEST_DIR}/make/libgtest.a $^ -o $@
