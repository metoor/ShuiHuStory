/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	SocketClient.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/03
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef SOCKETCLIENT_H_  
#define SOCKETCLIENT_H_  
  
#include "SocketBase.h"

#define MAX_BUFFER 4 * 1024 //一次发送的最大缓存

//协议
typedef struct
{
	char head[15];
	int checknum;
	int cmd;			//-0 接收地方英雄数据  -1 / -2 玩家Id  -3 敌方逃跑，直接获胜
	unsigned length;
	char content[MAX_BUFFER];

}MyMSG;

class SocketClient : public SocketBase
{
public:
	static SocketClient* construct();
	void destroy();

	bool connectServer(const char* serverIP, unsigned short port);
	void sendMessage(const char* data, int count);

	std::function<void(const char* data, int count)> onRecv;
	std::function<void()> onDisconnect;

	void update(float dt);

CC_CONSTRUCTOR_ACCESS:
	SocketClient(void);
	~SocketClient(void);

private:
	bool initClient();
	void recvMessage();
	void clear();

private:
	HSocket _socketServer;
	HSocket _socektClient;
	std::list<SocketMessage*> _UIMessageQueue;
	std::mutex   _UIMessageQueueMutex;
};

#endif // SOCKETCLIENT_H_ 