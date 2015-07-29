
#include <Packet\Packet.h>
#include <Packet\UdpServer.h>
#if defined(WIN32)
#include <Windows.h>
#endif
#include <stdio.h>

int main()
{
	Packet::initialize();
	Packet::UdpServer* pServer = new Packet::UdpServer(343218, 30.0f, 10);
	pServer->start(30);
	pServer->listen();
	Packet::Address clientAddress;
	char szMessage[256];
	while(pServer->isListening())
	{
#if defined(WIN32)
		Sleep(50);
#endif
		pServer->update(0.05f);
		memset(szMessage, 0, sizeof(szMessage));
		if(pServer->receivePacket(clientAddress, szMessage, sizeof(szMessage)) > 0)
		{
			printf("The server received: %s\n", szMessage);
			char* szSendMessage = "I am Server";
			pServer->sendReliablePacket(clientAddress, szSendMessage, strlen(szSendMessage));
		}
	}
	Packet::deInitialize();
	return 0;
}
