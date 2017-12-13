#/bin/sh

g++ -otest_named_mutex test_named_mutex.cpp -I../other -Wl,-Bstatic -lboost_thread -lboost_system -Wl,-Bdynamic -lpthread -lrt -Wl,--as-needed
