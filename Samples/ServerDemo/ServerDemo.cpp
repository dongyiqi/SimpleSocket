// ServerDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../SimpleSocket.h"
#include "../../PassiveSocket.h"
#include "../../ActiveSocket.h"
#pragma comment(lib, "clsocket.lib")

void startSimpleSocketServer()
{
	CPassiveSocket* pServerSocket = new CPassiveSocket(CSimpleSocket::SocketTypeUdp);
	CActiveSocket* pClientSocket = NULL;
	pServerSocket->Initialize();
	pServerSocket->SetNonblocking();
	//pServerSocket->SetBoardCast(true);
	bool bOk = pServerSocket->Listen(NULL, 8888);
	while (true)
	{
		if (pServerSocket->Select())
		{
			int32 nData = pServerSocket->Receive(800);
			if (nData > 0)
			{
				pServerSocket->Select();
				uint8* pClientAddr = pServerSocket->GetClientAddr();
				int16 nClientPort = pServerSocket->GetClientPort();

				byte* pData = pServerSocket->GetData();			
				pData[nData] = '\0';
				printf("recieved %d : %s\n", nData, pData);

				pServerSocket->Send((byte*)"imserver",8);
			}
		}
		Sleep(100);
	}
	delete pServerSocket;
}
void startServer()
{
	WSADATA wsaData;                                   //指向WinSocket信息结构的指针
	SOCKET sockListener;
	SOCKADDR_IN sin,saClient;                          //设置两个地址，sin用来绑定
	//saClient用来从广播地址接收消息
	char cRecvBuff[800];                               //定义接收缓冲区
	int nSize,nbSize;
	int iAddrLen=sizeof(saClient);
	if(WSAStartup(MAKEWORD( 1, 1 ), &wsaData )!=0)           //进行WinSocket的初始化
	{
		printf("Can't initiates windows socket!Program stop.\n");//初始化失败返回-1
		return ;
	}
	sockListener=socket(AF_INET, SOCK_DGRAM,0);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);             //发送端使用的发送端口，可以根据需要更改
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind( sockListener, (SOCKADDR FAR *)&sin, sizeof(sin))!=0)
	{
		printf("Can't bind socket to local port!Program stop.\n");//初始化失败返回-1
		return ;
	}
	while(1)
	{  
		nSize = sizeof ( SOCKADDR_IN );
		if((nbSize=recvfrom (sockListener,cRecvBuff,800,0,
			(SOCKADDR FAR *) &saClient,&nSize))==SOCKET_ERROR) //若接收失败则提示错误
		{
			printf("Recive Error");
			break;
		}
		cRecvBuff[nbSize] = '\0';                              //字符串终止
		printf("%s\n",cRecvBuff);                              //显示所接收到的字符串


	}
	return;	
}

int _tmain(int argc, _TCHAR* argv[])
{
	startSimpleSocketServer();
	return 0;
}

