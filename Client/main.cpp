
#include <Packet\Packet.h>
#include <Packet\UdpClient.h>
#if defined(WIN32)
#include <Windows.h>
#endif
#include <stdio.h>

int main()
{
	Packet::initialize();
	Packet::UdpClient* pClient = new Packet::UdpClient(343218, 30.0f);
	pClient->start(20);
	pClient->connect(Packet::Address(127, 0, 0, 1, 30));
	char szMessage[256];
	while(true)
	{
#if defined(WIN32)
		Sleep(50);
#endif
		pClient->update(0.05f);
		memset(szMessage, 0, sizeof(szMessage));
		char* szSendMessage = "I am Client";
		pClient->sendReliablePacket(szSendMessage, strlen(szSendMessage));
		if(pClient->receivePacket(szMessage, sizeof(szMessage)) > 0)
		{
			printf("The client received: %s\n", szMessage);
		}
	}
	Packet::deInitialize();
	return 0;
}
