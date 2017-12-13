#/bin/sh

g++ test_pool.cpp -I../other -Wl,-Bstatic -lboost_system -Wl,-Bdynamic -lpthread -lrt -Wl,--as-needed
