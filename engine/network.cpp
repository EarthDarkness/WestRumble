#include "network.h"
#include <memory.h>

int NET_ThreadListen(void* userdata){
	return reinterpret_cast<network*>(userdata)->listenRoutine();
}
int NET_ThreadReceive(void* userdata){
	network* nt = reinterpret_cast<network*>(userdata);
	if(nt->_who == -1)
		return -1;
	return nt->receiveRoutine(nt->_who);
}

int network::launchListenThread(){
	_thread[NET_SERVER] = SDL_CreateThread(NET_ThreadListen,"NetworkingListen",this);

	if(_thread[NET_SERVER] == NULL)
		return -1;

	return 0;
}
int network::launchReceiveThread(int id){
	_who = id;
	_thread[id] = SDL_CreateThread(NET_ThreadReceive, "NetworkingReceive", this);
	_who = -1;

	if(_thread[id] == NULL)
		return -1;
	return 0;
}

void network::waitForThreads(){
	_signal = 1;

	for(int i=0;i<NET_MAXUSERSIZE;++i){
		int out;
		if(_thread[i] == NULL)
			continue;
		SDL_WaitThread(_thread[i],&out);
	}
	_signal = 0;
}

int network::listenRoutine(){
	while(_signal == 0){
		int spot=0;
		for(int i=NET_CLIENT1;i<NET_MAXUSERSIZE;++i){
			if(_socket[i] == NULL){
				spot = i;
				break;
			}
		}
		if(spot == 0)
			SDL_Delay(250);

		_socket[spot] = SDLNet_TCP_Accept(_socket[NET_SERVER]);

		if(_socket[spot] == NULL)
			continue;

		if(SDLNet_TCP_Send(_socket[spot],reinterpret_cast<char*>(&spot),sizeof(int)) < sizeof(int)){
			SDLNet_TCP_Close(_socket[spot]);
			_socket[spot] = NULL;
		}


	}
	return 0;
}
int network::receiveRoutine(int id){
	TCPsocket sock = _socket[id];
	while(_signal == 0){
		if(_hear == 0 && _netId == NET_SERVER)
			SDL_Delay(100);
		char buf[LEN];

		if(SDLNet_TCP_Recv(sock,(void*)buf,LEN) <= 0){
			SDLNet_TCP_Close(sock);
			_socket[id] = NULL;//maybe lock //maybe not here
			break;
		}

		if((_in+1)%WSIZE == _out)
			continue;//OUT OF MEMORY FOR MESSAGES

		memcpy(_window[_in],buf,LEN);
		_in = (_in+1)%WSIZE;

	}
	return 0;
}


network::network(){
	_who = -1;
	_signal = 0;
	_hear = 0;

	for(int i=0;i<NET_MAXUSERSIZE;++i){
		_socket[i] = NULL;
		_thread[i] = NULL;
	}

	_netId = -1;
	_working = false;

	if(SDLNet_Init() != -1)
		_working = true;

	for(int i=0;i<WSIZE;++i)
		memset(_window[i],0,LEN);

	_in = 0;
	_out = 0;


	//if(_working)
		//_sockets = SDLNet_AllocSocketSet(NET_MAXUSERSIZE);
}
network::~network(){
	if(!_working)
		return;

	_signal = 1;

	//SDLNet_FreeSocketSet(_sockets);
	int out;
	for(int i=0;i<NET_MAXUSERSIZE;++i){
		if(_thread[i] != NULL)
			SDL_WaitThread(_thread[i],&out);
		if(_socket[i] != NULL)
			SDLNet_TCP_Close(_socket[i]);
	}

	SDLNet_Quit();
}

int network::initServer(uint16_t port){
	if(!_working)
		return -1;

	IPaddress address;
	SDLNet_ResolveHost(&address,INADDR_ANY,port);

	_socket[NET_SERVER] = SDLNet_TCP_Open(&address);

	if(_socket[NET_SERVER] == NULL)
		return -1;

	_netId = NET_SERVER;

	launchListenThread();
	for(int i=NET_CLIENT1;i<NET_MAXUSERSIZE;++i)
		launchReceiveThread(i);

	return 0;
}
int network::initClient(const char* addr, uint16_t port){
	if(!_working)
		return -1;

	IPaddress address;
	SDLNet_ResolveHost(&address, addr, port);

	_socket[NET_SELF] = SDLNet_TCP_Open(&address);

	if(_socket[NET_SELF] == NULL)
		return -1;

	char buf[LEN];

	if(SDLNet_TCP_Recv(_socket[NET_SELF],(void*)buf,LEN) <= 0){
		SDLNet_TCP_Close(_socket[NET_SELF]);
		_socket[NET_SELF] = NULL;
		return -1;
	}

	_netId = *(reinterpret_cast<int*>(buf));

	return launchReceiveThread(NET_SELF);
}

int network::send(void* data, int len){
	if(!_working)
		return -1;

	if(_netId > NET_SERVER){
		if(SDLNet_TCP_Send(_socket[NET_SELF],data, len) < len){
			close();

			return -1;
		}
	}else{
		for(int i=NET_CLIENT1;i<NET_MAXUSERSIZE;++i){
			if(_socket[i] != NULL){
				if(SDLNet_TCP_Send(_socket[i], data, len) < len){
					SDLNet_TCP_Close(_socket[i]);

					_socket[i] = NULL;
				}
			}
		}
	}

	return 0;
}
int network::recv(void* data, int len){
	if(!_working)
		return -1;

	if(len < LEN)
		return -1;

	if(_in == _out)
		return 0;
	
	memcpy(data,_window[_out],LEN);
	_out = (_out+1)%WSIZE;

	return 1;
}

void network::close(){
	waitForThreads();

	if(_netId == NET_SERVER){
		for(int i=NET_CLIENT1;i<NET_MAXUSERSIZE;++i){
			if(_socket[i] == NULL)
				continue;

			SDLNet_TCP_Close(_socket[i]);
			_socket[i] = NULL;
		}
	}

	SDLNet_TCP_Close(_socket[NET_SELF]);
	_socket[NET_SELF] = NULL;

	_netId = -1;
	_in = 0;
	_out = 0;

}