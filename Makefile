SOURCE := $(wildcard src/*.cc)
CLIENT := csapp.o echoclient.o
SERVER := csapp.o echoserveri.o kv_encode.o kv_io.o kv_cluster_epoll.o kv_conn.o kv_command.o storage_engine.o kv_exception.o serverstats.o

CXXFLAGS += -g
INCLUDE_PATH = -I ./include \

all:
	g++ -c $(CXXFLAGS) $(INCLUDE_PATH) $(SOURCE)
	g++ -o client $(CXXFLAGS) $(CLIENT) -lpthread -lleveldb -lboost_system -lboost_filesystem -lglog
	g++ -o server $(CXXFLAGS) $(SERVER) -lpthread -lleveldb -lboost_system -lboost_filesystem -lglog

clean:
	rm *.o
	rm -r client server
