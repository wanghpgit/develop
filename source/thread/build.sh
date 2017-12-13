#/bin/sh

g++ -o test_thread -D _DEBUG test_thread.cpp -I../other -Wl,-Bstatic -lboost_thread -lboost_system -Wl,-Bdynamic -lpthread -lrt -Wl,--as-needed
