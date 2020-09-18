diseaseMonitor: bucket_list.o main.o record.o avl.o hash.o max_heap.o
	g++ -std=c++11 bucket_list.o main.o record.o avl.o hash.o max_heap.o -g -o diseaseMonitor 
bucket_list.o: bucket_list.c bucket.h
	g++ -std=c++11 -c bucket_list.c
main.o: main.cpp hash.h bucket.h avl.h
	g++ -std=c++11 -c main.cpp
record.o: record.c avl.h
	g++ -std=c++11 -c record.c
avl.o: avl.c avl.h
	g++ -std=c++11 -c avl.c
hash.o: hash.c hash.h
	g++ -std=c++11 -c hash.c 
max_heap.o: max_heap.c max_heap.h
	g++ -std=c++11 -c max_heap.c
clean: 
	rm bucket_list.o main.o record.o avl.o hash.o max_heap.o diseaseMonitor
