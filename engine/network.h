#ifndef NETWORK_H
#define NETWORK_H

#include <SDL_net.h>
#include <SDL_thread.h>

enum NETID{
	NET_SERVER = 0,
	NET_SELF = 0, 
	NET_CLIENT1,
	NET_CLIENT2,
	NET_CLIENT3,
	NET_MAXUSERSIZE
};

const uint16_t UDP_BROADCAST_PORT = 5123;

const int LEN = 4096;
const int WSIZE = 64;
class network {
private:
	//SDLNet_SocketSet _sockets;//hidden pointer like (_SDLNet_SocketSet*)
	TCPsocket _socket[NET_MAXUSERSIZE];//hidden pointer like (_TCPsocket*)//port 5651
	SDL_Thread* _thread[NET_MAXUSERSIZE];

	UDPsocket _ask;//port 5123
	UDPsocket _feed;//port 5512
	UDPpacket* _msg;

	char _ip[16];

	char _window[WSIZE][LEN];
	int _in;
	int _out;


	int _netId;
	bool _working;

	int launchListenThread();
	int launchReceiveThread(int id);

	void waitForThreads();

	void resolveLocalIp();

public:
	volatile int _who;
	volatile int _signal;
	volatile int _hear;


	int listenRoutine();
	int receiveRoutine(int id);

	network();
	~network();

	int init();
	int quit();

	const char* getLocalIp();

	int initServer(uint16_t port);
	int initClient();
	int initClient(const char* addr, uint16_t port);

	int send(void* data, int len);
	int recv(void* data, int len);

	void close();

};

#endif