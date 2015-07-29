
#include <Packet\Packet.h>

#if defined(WIN32)

#ifdef _MSC_VER
#pragma comment(lib,"WSOCK32.LIB")
#endif

#include <winsock2.h>

#elif defined(__linux__) || defined(__APPLE__)

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

namespace Packet
{

	bool initialize()
	{
#if defined (WIN32)
		WSADATA WsaData;
		return WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR;
#else
		return true;
#endif
	}

	void deInitialize()
	{
#if defined (WIN32)
		WSACleanup();
#endif
	}

}
