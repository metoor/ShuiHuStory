/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	ClientLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/03
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "ClientLayer.h"
#include <thread>
#include "McLog.h"

USING_NS_CC;
using std::string;


bool ClientLayer::init()
{
	if ( !(Layer::init()))
	{
		return false;
	}
	addConnectMessageListener();


	return true;
}

void ClientLayer::initSocket(int port)
{
	_port = port;

	_client = SocketClient::construct();
	_client->onRecv = CC_CALLBACK_2(ClientLayer::onRecv, this);
	_client->onDisconnect = CC_CALLBACK_0(ClientLayer::onDisconnect, this);
}

void ClientLayer::connectThread()
{
	//从文件解析服务器ip，如果为空则设为本地连接
	if (_ip.empty())
	{
		_ip = FileUtils::getInstance()->getStringFromFile("datas/ipconfig.mc");

		if (_ip.empty())
		{
			_ip += "127.0.0.1";
		}
	}

	int result = 1;

	if (_client->connectServer(_ip.c_str(), _port))
	{
		result = 0;
	}
	else
	{
		McLog mc;
		mc.addWaring("connect server failed...", __FILE__, __LINE__);
	}

	//在cocos主线程中发送连接状态信息 0/表示成功 1/表示失败
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, result]() {
		_eventDispatcher->dispatchCustomEvent(msg_socket_connect, (void*)result);

		//McLog mc;
		//mc.addWaring("get data success, rung pvp scene...", __FILE__, __LINE__);
	});
}

void ClientLayer::onEnter()
{
	Layer::onEnter();

	//异步连接服务器
	std::thread th(&ClientLayer::connectThread, this);
	th.detach();
}

void ClientLayer::onExit()
{
	Layer::onExit();

	if (_client)
	{
		_client->destroy();
		_client = nullptr;
	}
}

void ClientLayer::addConnectMessageListener()
{
	//注册，socket初始话化状态消息，0表示成功，1表示失败
	auto listen = EventListenerCustom::create(msg_socket_connect, [&](EventCustom* event) {
		int result = (int)(event->getUserData());

		//连接失败，将自己从父节点移除
		if (result == 0)
		{
			//成功
		}
		else
		{
			McLog mc;
			mc.addWaring("can not init socket...", __FILE__, __LINE__);

			this->removeFromParentAndCleanup(true);
		}
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
}

void ClientLayer::onRecv(const char* data, int count)
{
	std::string*  content;

	auto msg = (MyMSG*)data;

	if (sizeof(*msg) == count)
	{
		if (msg->cmd == 0)  //如果cmd等于0，则把接收到的数据储存在m_enemyData中
		{
			content = &_data.enemyData;
		}
		else if (msg->cmd == 1 || msg->cmd == 2 || msg->cmd == 3)	//如果cmd等于1，2或3，则把接收到的数据储存在m_batteData中
		{
			content = &_data.battleData;
		}

		if (msg->checknum == check(msg) && strcmp(msg->head, "com.metoor.cai") == 0)
		{
			//接受英雄数据
			*content += msg->content;

			if (content->length() == msg->length)
			{
				switch (msg->cmd)
				{
				case 0:
					//接受敌方英雄数据
					return;
					break;
				case 1:
				case 2:
				case 3:
					//玩家id为3，则表示本客户端获胜，对方逃跑了
					//id为1或者2，表示该客户端是玩家1还是玩家2
					_data.playerId = msg->cmd;
					break;
				default:
					break;
				}

				//在cocos主线程中启动PVP场景
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]() {
					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(msg_create_pvp_scene, (void*)&_data);
					_client->destroy();
					_client = nullptr;
					this->removeFromParentAndCleanup(true);
					//McLog mc;
					//mc.addWaring("get data success, rung pvp scene...", __FILE__, __LINE__);
				});
			}
		}
		else
		{
			McLog mc;
			mc.addWaring("data head error...", __FILE__, __LINE__);
		}
	}
	else
	{
		McLog mc;
		mc.addWaring("receive data size error...", __FILE__, __LINE__);
	}
}

void ClientLayer::onDisconnect()
{
	//发送断开连接消息
	_eventDispatcher->dispatchCustomEvent(msg_socket_disconnect);
	this->removeFromParentAndCleanup(true);
}

void ClientLayer::sendMessage(std::string& content, int cmd)
{
	MyMSG msg;
	strcpy(msg.head, "com.metoor.cai");
	msg.cmd = cmd;
	msg.length = content.length();
	strcpy(msg.content, content.c_str());
	msg.checknum = check(&msg);

	_client->sendMessage((char*)&msg, sizeof(msg));
}

int ClientLayer::check(MyMSG* msg)
{
	return strlen(msg->content);
}