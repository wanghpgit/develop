#/bin/sh

#g++ tcp_boost.cpp -o tcp_boost -I../other -Wl,-Bstatic -lboost_system -Wl,-Bdynamic -Wl,--as-needed
g++ tcp_test.cpp -o tcp_test -I.. -Wl,-Bstatic -lboost_system -lboost_thread -Wl,-Bdynamic -lpthread -lrt -Wl,--as-needed
