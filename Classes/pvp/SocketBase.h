/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	SocketBase.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/03
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef SOCKETBASE_H_  
#define SOCKETBASE_H_  
  
#include "cocos2d.h"
#include <list>
#include <thread>
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <fcntl.h>
//#include <error.h>
#include <arpa/inet.h>		// for inet_**
#include <netdb.h>			// for gethost**
#include <netinet/in.h>		// for sockaddr_in
#include <sys/types.h>		// for socket
#include <sys/socket.h>		// for socket
#include <unistd.h>
#include <stdio.h>		    // for printf
#include <stdlib.h>			// for exit
#include <string.h>			// for bzero
#define HSocket int
#endif 

enum MessageType
{
	DISCONNECT,
	RECEIVE,
	NEW_CONNECTION
};

class SocketMessage
{
private:
	MessageType msgType;
	Data* msgData;

public:
	SocketMessage(MessageType type, unsigned char* data, int dataLen)
	{
		msgType = type;
		msgData = new Data;
		msgData->copy(data, dataLen);
	}

	SocketMessage(MessageType type)
	{
		msgType = type;
		msgData = nullptr;
	}

	Data* getMsgData() { return msgData; }
	MessageType getMsgType() { return msgType; }

	~SocketMessage()
	{
		if (msgData)
			CC_SAFE_DELETE(msgData);
	}
};

class SocketBase : public Ref
{
public:
	SocketBase();
	~SocketBase();

	bool nonBlock(HSocket socket);

protected:
	void closeConnect(HSocket socket);
	bool error(HSocket socket);
	
protected:
	std::mutex _mutex;

private:
	bool _bInitSuccess;
};

#endif //SOCKETBASE_H_ 