all: quick_2_sort merge_1_sort

clean: 
	rm -f *.txt *.o quick_2_sort merge_1_sort

CFLAGS = -O

.SUFFIXES: .cpp .o
.cpp.o:
	g++ $(CFLAGS) -c $*.cpp

quick_2_sort: quick_2_sort.o sort_driver.o
	g++ $(CFLAGS) -o quick_2_sort quick_2_sort.o sort_driver.o

merge_1_sort: merge_1_sort.o sort_driver.o
	g++ $(CFLAGS) -g -o merge_1_sort merge_1_sort.o sort_driver.o

