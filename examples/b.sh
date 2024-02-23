g++ -g -Wall $1.cpp -o $1 -I../include -I../include/hiredis -L../lib/ -lhiredis -luv -Wl,-rpath,../lib
