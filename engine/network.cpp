#include "network.h"

int NET_ThreadListen(void* userdata){
	return reinterpret_cast<network*>(userdata)->listenRoutine();
}
int NET_ThreadReceive(void* userdata){
	network* nt = reinterpret_cast<network*>(userdata);
	if(nt->_who == -1)
		return;
	return nt->receiveRoutine(nt->_who);
}

int network::launchListenThread(){
	_thread[0] = SDL_CreateThread(NET_ThreadListen,"NetworkingListen",this);

	if(_thread[0] == NULL)
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

int network::listenRoutine(){
	while(_signal == 0){
		int spot=0;
		for(int i=1;i<NET_MAXUSERSIZE;++i){
			if(_peers[i] == NULL){
				spot = i;
				break;
			}
		}
		if(spot == 0)
			SDL_Delay(250);

		_peers[spot] = SDLNet_TCP_Accept(_self);

		if(_peers[spot] == NULL)
			continue;

		if(SDLNet_TCP_Send(_peers[spot],reinterpret_cast<char*>(&spot),sizeof(int)) < sizeof(int)){
			SDLNet_TCP_Close(_peers[spot]);
			_peers[spot] = NULL;
		}

	}
	return 0;
}
int network::receiveRoutine(int id){
	const int LEN = 1024;
	TCPsocket sock = _peers[id];
	while(_signal == 0){
		if(_hear == 0)
			SDL_Delay(100);
		char buf[LEN];

		if(SDLNet_TCP_Recv(sock,(void*)buf,LEN) <= 0){
			SDLNet_TCP_Close(sock);
			_peers[id] = NULL;//maybe lock //maybe not here
			break;
		}

		//TODO handle data

	}
	return 0;
}


network::network(){
	_who = -1;
	_signal = 0;
	_hear = 0;
	//_sockets = NULL;
	_self = NULL;
	for(int i=0;i<NET_MAXUSERSIZE;++i){
		_peers[i] = NULL;
		_thread[i] = NULL;
	}

	_netId = -1;
	_working = false;

	if(SDLNet_Init() != -1)
		_working = true;

	//if(_working)
		//_sockets = SDLNet_AllocSocketSet(NET_MAXUSERSIZE);
}
network::~network(){
	if(!_working)
		return;

	//SDLNet_FreeSocketSet(_sockets);
	int out;
	if(_thread != NULL)
		SDL_WaitThread(_thread,&out);

	SDLNet_Quit();
}

int network::initServer(uint16_t port){
	IPaddress address;
	SDLNet_ResolveHost(&address,INADDR_ANY,port);

	_self = SDLNet_TCP_Open(&address);

	if(_self == NULL)
		return -1;

	_netId = NET_SERVER;

	launchThread();

	return 0;
}
int network::initClient(const char* addr, uint16_t port){
	IPaddress address;
	SDLNet_ResolveHost(&address, addr, port);

	_self = SDLNet_TCP_Open(&address);

	if(_self == NULL)
		return -1;

	_netId = NET_CLIENT1;

	launchThread();

	return 0;
}

int network::send(void* data, int len){
	if(_netId > NET_SERVER){
		if(SDLNet_TCP_Send(_self,data, len) < len){
			int out;

			_signal = 1;
			SDL_WaitThread(_thread[0],&out);
			_signal = 0;

			SDLNet_TCP_Close(_self);

			_self = NULL;
			_netId = -1;

			return -1;
		}
	}else{
		for(int i=1;i<NET_MAXUSERSIZE;++i){
			if(_peers[i] != NULL){
				if(SDLNet_TCP_Send(_peers[i], data, len) < len){
					//TODO close connection
				}
			}
		}
	}

}