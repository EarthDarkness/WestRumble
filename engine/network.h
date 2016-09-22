#ifndef NETWORK_H
#define NETWORK_H

#include <SDL_net.h>
#include <SDL_thread.h>

enum NETID{
	NET_SERVER = 0,
	NET_CLIENT1,
	NET_CLIENT2,
	NET_CLIENT3,
	NET_MAXUSERSIZE
};

class network {
private:
	//SDLNet_SocketSet _sockets;//hidden pointer like (_SDLNet_SocketSet*)
	TCPsocket _self;//hidden pointer like (_TCPsocket*)
	TCPsocket _peers[NET_MAXUSERSIZE-1];

	SDL_Thread* _thread[NET_MAXUSERSIZE];

	int _netId;
	bool _working;

	int launchListenThread();
	int launchReceiveThread(int id);

public:
	volatile int _who;
	volatile int _signal;
	volatile int _hear;

	int listenRoutine();
	int receiveRoutine(int id);

	network();
	~network();

	int initServer(uint16_t port);
	int initClient(const char* addr, uint16_t port);

	int send(void* data, int len);

};

#endif