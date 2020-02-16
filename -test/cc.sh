rm -f *.o

gcc -O3 -fomit-frame-pointer -o libco.o -c ../libco.c
g++ -O3 -fomit-frame-pointer -c test_timing.cpp
g++ -O3 -fomit-frame-pointer -o test_timing libco.o test_timing.o

g++ -O3 -fomit-frame-pointer -c test_timing_multi.cpp
g++ -O3 -fomit-frame-pointer -o test_timing_multi test_timing_multi.o
