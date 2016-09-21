#ifndef NETPROTOCOL_H
#define NETPROTOCOL_H

#include <stdio.h>

enum NetProtocol{
	WRP_NONE = 0,
	WRP_END_TURN,
	WRP_MOVE,
	WRP_DYNAMITE,
	WRP_SHOT,
	WRP_TIME_UP,
	WRP_TIME_DOWN,
	WRP_THROW,
	WRP_BARREL,
	WRP_ROPE,
	WRP_DETONATE,
	WRP_MAX_SIZE
};
enum NetDir{
	WRP_UP = 0,
	WRP_DOWN,
	WRP_LEFT,
	WRP_RIGHT
};

//Encode message

inline void WrpEncodeEndTurn(char* msg){
	int p=-1;
	msg[++p] = (char)(WRP_END_TURN);
	msg[++p] = '\0';
}
inline void WrpEncodeMove(char* msg, int pid, int x, int y){
	int p=-1;
	msg[++p] = (char)(WRP_MOVE);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(x & 0xFF);
	msg[++p] = (char)(y & 0xFF);
	msg[++p] = '\0';
}
inline void WrpEncodeDynamite(char* msg, int pid, int x, int y){
	int p=-1;
	msg[++p] = (char)(WRP_DYNAMITE);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(x & 0xFF);
	msg[++p] = (char)(y & 0xFF);
	msg[++p] = '\0';
}
inline void WrpEncodeShot(char* msg, int pid, int dir){
	int p=-1;
	msg[++p] = (char)(WRP_SHOT);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(dir);
	msg[++p] = '\0';
}
inline void WrpEncodeTimeUp(char* msg, int pid, int x, int y){
	int p=-1;
	msg[++p] = (char)(WRP_TIME_UP);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(x & 0xFF);
	msg[++p] = (char)(y & 0xFF);
	msg[++p] = '\0';
}
inline void WrpEncodeTimeDown(char* msg, int pid, int x, int y){
	int p=-1;
	msg[++p] = (char)(WRP_TIME_DOWN);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(x & 0xFF);
	msg[++p] = (char)(y & 0xFF);
	msg[++p] = '\0';
}
inline void WrpEncodeThrow(char* msg, int pid, int xi, int yi, int x, int y){
	int p=-1;
	msg[++p] = (char)(WRP_THROW);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(xi & 0xFF);
	msg[++p] = (char)(yi & 0xFF);
	msg[++p] = (char)(x & 0xFF);
	msg[++p] = (char)(y & 0xFF);
	msg[++p] = '\0';
}
inline void WrpEncodeBarrel(char* msg, int pid, int x, int y){
	int p=-1;
	msg[++p] = (char)(WRP_BARREL);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(x & 0xFF);
	msg[++p] = (char)(y & 0xFF);
	msg[++p] = '\0';
}
inline void WrpEncodeRope(char* msg, int pid, int x, int y){
	int p=-1;
	msg[++p] = (char)(WRP_ROPE);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = (char)(x & 0xFF);
	msg[++p] = (char)(y & 0xFF);
	msg[++p] = '\0';
}
inline void WrpEncodeDetonate(char* msg, int pid){
	int p=-1;
	msg[++p] = (char)(WRP_BARREL);
	msg[++p] = (char)(pid & 0xFF);
	msg[++p] = '\0';
}

//decode message

inline bool WrpDecodeEndTurn(const char* msg){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_END_TURN)
		return false;

	return true;
}
inline bool WrpDecodeMove(const char* msg, int& pid, int& x, int& y){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_MOVE)
		return false;

	pid = msg[++p];
	x = msg[++p];
	y = msg[++p];

	return true;
}
inline bool WrpDecodeDynamite(const char* msg, int& pid, int& x, int& y){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_DYNAMITE)
		return false;

	pid = msg[++p];
	x = msg[++p];
	y = msg[++p];

	return true;
}
inline bool WrpDecodeShot(const char* msg, int& pid, int& dir){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_SHOT)
		return false;

	pid = msg[++p];
	dir = msg[++p];

	return true;
}
inline bool WrpDecodeTimeUp(const char* msg, int& pid, int& x, int& y){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_TIME_UP)
		return false;

	pid = msg[++p];
	x = msg[++p];
	y = msg[++p];

	return true;
}
inline bool WrpDecodeTimeDown(const char* msg, int& pid, int& x, int& y){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_TIME_DOWN)
		return false;

	pid = msg[++p];
	x = msg[++p];
	y = msg[++p];

	return true;
}
inline bool WrpDecodeThrow(const char* msg, int& pid, int& xi, int& yi, int& x, int& y){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_THROW)
		return false;

	pid = msg[++p];
	xi = msg[++p];
	yi = msg[++p];
	x = msg[++p];
	y = msg[++p];

	return true;
}
inline bool WrpDecodeBarrel(const char* msg, int& pid, int& x, int& y){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_BARREL)
		return false;

	pid = msg[++p];
	x = msg[++p];
	y = msg[++p];

	return true;
}
inline bool WrpDecodeRope(const char* msg, int& pid, int& x, int& y){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_ROPE)
		return false;

	pid = msg[++p];
	x = msg[++p];
	y = msg[++p];

	return true;
}
inline bool WrpDecodeDetonate(const char* msg, int& pid){
	if(msg == NULL)
		return false;

	int p=-1;
	if(msg[++p] != WRP_DETONATE)
		return false;

	pid = msg[++p];

	return true;
}



#endif