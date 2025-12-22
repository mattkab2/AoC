.SUFFIXES:
.SUFFIXES: .cpp .o .h .txt .dat
CC = clang++
CXXFLAGS = -std=c++20 -g -Wall

result : part1.exe part2.exe input_long.dat
	./part1.exe input_long.dat
	./part2.exe input_long.dat

test : part1.exe part2.exe input_short.dat
	./part1.exe input_short.dat
	./part2.exe input_short.dat

memcheck : part1.exe part2.exe input_long.dat
	valgrind ./part1.exe input_long.dat
	valgrind ./part2.exe input_long.dat

%.exe : %.o
	$(CC) -o $*.exe $<

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $<

input_short.dat : proc_input.sh input_short.txt
	./proc_input.sh input_short.txt

input_long.dat : proc_input.sh input_long.txt
	./proc_input.sh input_long.txt

.PHONY: clean all
clean:
	rm -f *.o *.dat *.exe

