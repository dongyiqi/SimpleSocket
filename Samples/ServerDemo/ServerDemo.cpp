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
	WSADATA wsaData;                                   //ָ��WinSocket��Ϣ�ṹ��ָ��
	SOCKET sockListener;
	SOCKADDR_IN sin,saClient;                          //����������ַ��sin������
	//saClient�����ӹ㲥��ַ������Ϣ
	char cRecvBuff[800];                               //������ջ�����
	int nSize,nbSize;
	int iAddrLen=sizeof(saClient);
	if(WSAStartup(MAKEWORD( 1, 1 ), &wsaData )!=0)           //����WinSocket�ĳ�ʼ��
	{
		printf("Can't initiates windows socket!Program stop.\n");//��ʼ��ʧ�ܷ���-1
		return ;
	}
	sockListener=socket(AF_INET, SOCK_DGRAM,0);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);             //���Ͷ�ʹ�õķ��Ͷ˿ڣ����Ը�����Ҫ����
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind( sockListener, (SOCKADDR FAR *)&sin, sizeof(sin))!=0)
	{
		printf("Can't bind socket to local port!Program stop.\n");//��ʼ��ʧ�ܷ���-1
		return ;
	}
	while(1)
	{  
		nSize = sizeof ( SOCKADDR_IN );
		if((nbSize=recvfrom (sockListener,cRecvBuff,800,0,
			(SOCKADDR FAR *) &saClient,&nSize))==SOCKET_ERROR) //������ʧ������ʾ����
		{
			printf("Recive Error");
			break;
		}
		cRecvBuff[nbSize] = '\0';                              //�ַ�����ֹ
		printf("%s\n",cRecvBuff);                              //��ʾ�����յ����ַ���


	}
	return;	
}

int _tmain(int argc, _TCHAR* argv[])
{
	startSimpleSocketServer();
	return 0;
}

