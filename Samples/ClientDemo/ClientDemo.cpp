// ClientDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../SimpleSocket.h"
#include "../../PassiveSocket.h"
#include "../../ActiveSocket.h"
#pragma comment(lib, "clsocket.lib")

struct UdpHeartPack {
	char UDPData[16] ;
};

void startSimpleSocketClient()
{
	bool bRet = false;
	CActiveSocket* pClientSocket = new CActiveSocket(CSimpleSocket::SocketTypeUdp);
	pClientSocket->Initialize();
	//pClientSocket->SetNonblocking();
	bRet = pClientSocket->SetBoardCast(true, 8888);
	{
		int nSend = pClientSocket->Send((byte*)"imclient",8);
		int nDataLen = pClientSocket->Receive(800);
		byte* pData = pClientSocket->GetData();
		pData[nDataLen] = '\0';
		printf("get server ancall:%s\n",pData);
		
	}
}

void startClient()
{
	struct UdpHeartPack udpPack;
	static int UDP_PORT = 8888;
	udpPack.UDPData[0] = 'h';
	udpPack.UDPData[1] = 'e';
	udpPack.UDPData[2] = 'l';
	udpPack.UDPData[3] = 'l';
	udpPack.UDPData[4] = 'o';
	udpPack.UDPData[5] = ' ';
	udpPack.UDPData[6] = 'w';
	udpPack.UDPData[7] = 'o';
	udpPack.UDPData[8] = 'r';
	udpPack.UDPData[9] = 'l';
	udpPack.UDPData[10] = 'd';
	udpPack.UDPData[11] = '\0';
	char *pPack = (char *) &udpPack;
	WSADATA wsaData;                                    //ָ��WinSocket��Ϣ�ṹ��ָ��
	SOCKET sockListener;                                //�����׽���
	SOCKADDR_IN saUdpServ;                              //ָ��ͨ�Ŷ���Ľṹ��ָ��                          
	BOOL fBroadcast = TRUE;                             //����setsockopt(),��ʾ����
	char sendBuff[800];                                 //��������ŷ��͵�����
	int ncount=0;                                       //������ʾ��Ϣ��Ŀ
	//*************************** ��һ����ʼ��Winsock   *****************************//
	if(WSAStartup(MAKEWORD( 1, 1 ), &wsaData )!=0)     //����WinSocket�ĳ�ʼ��
	{
		printf("Can't initiates windows socket!Program stop.\n");//��ʼ��ʧ�ܷ���-1
		return ;
	}
	//********************   �ڶ�������һ�����ݱ����͵�UDP�׽���  ******************//
	sockListener=socket(PF_INET,SOCK_DGRAM,0);    
	//       setsockopt�������������׽ӿ�ѡ��
	//       ���ù㲥��ʽ�뽫��������������ΪSO_BROADCAST
	setsockopt ( sockListener,SOL_SOCKET,SO_BROADCAST, (CHAR *)&fBroadcast, sizeof (BOOL) );
	//  �������ã�ע��Ҫ��IP��ַ��ΪINADDR_BROADCAST����ʾ���͹㲥UDP���ݱ�
	saUdpServ.sin_family = AF_INET;
	saUdpServ.sin_addr.s_addr = htonl ( INADDR_BROADCAST );
	saUdpServ.sin_port = htons (UDP_PORT);               //�����õĶ˿ڣ����Ը�����Ҫ����

	while(1)                                         //ѭ����������
	{
		Sleep(1000);
		sprintf(sendBuff,"Message %d is: ok",ncount++);    //��ncount��ֵ�����ַ���senBuff��
		//**********************  ������ʹ��sendto��������ͨ��    *************************// 
		sendto ( sockListener,/*sendBuff*/pPack, strlen (sendBuff)/*sizeof(udpPack)*/, 0, (SOCKADDR *) &saUdpServ, sizeof (SOCKADDR_IN));
		printf("%s\n",sendBuff);                    //��Ҫ�㲥�����ݴ����
	} 
	//*********************   ���Ĳ��ر�socket  ***************************************//
	closesocket(sockListener);         //�رռ���socket
	WSACleanup();
	return;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//startClient();
	startSimpleSocketClient();
	return 0;
}

