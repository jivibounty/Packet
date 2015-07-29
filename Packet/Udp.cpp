
#include <Packet\Udp.h>
#include <Packet\Address.h>
#include <assert.h>
#include <stdio.h>

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

	Udp::Udp()
		: m_SocketHandle(0)
	{
	}

	Udp::~Udp()
	{
		deInitialize();
	}

	bool Udp::initialize(unsigned int portNo)
	{
		//close socket if open
		if(isOpen())
		{
			deInitialize();
		}

		//create socket
		m_SocketHandle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

		if ( m_SocketHandle <= 0 )
		{
			printf( "Udp::initialize; failed to create socket\n" );
			return false;
		}

		//bind socket
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons( (unsigned short) portNo );

		if ( bind( m_SocketHandle, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
		{
			printf( "Udp::initialize; failed to bind socket\n" );
			deInitialize();
			return false;
		}

		//set non-blocking mode
#if defined(WIN32)

		DWORD nonBlocking = 1;
		if ( ioctlsocket( m_SocketHandle, FIONBIO, &nonBlocking ) != 0 )
		{
			printf( "Udp::initialize; failed to set non-blocking\n" );
			deInitialize();
			return false;
		}

#else if defined(__linux__) || defined(__APPLE__)

		int nonBlocking = 1;
		if ( fcntl( m_SocketHandle, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
		{
			printf( "Udp::initialize; failed to set non-blocking\n" );
			deInitialize();
			return false;
		}
#endif
		return true;
	}

	void Udp::deInitialize()
	{
		if(isOpen())
		{
#if defined(WIN32)

			closesocket( m_SocketHandle );

#else if defined(__linux__) || defined(__APPLE__)

			close( m_SocketHandle );

#endif
			m_SocketHandle = 0;
		}
	}

	bool Udp::send(const Address& destination, const char* data, int dataSize)
	{
		assert(data);
		assert(dataSize > 0);

		if(!isOpen())
		{
			printf( "Udp::send; cannot send; socket is not open\n" );
			return false;
		}

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl( destination.getAddress() );
		address.sin_port = htons( (unsigned short) destination.getPortNumber() );

		//send data
		int sentBytes = sendto( m_SocketHandle, data, dataSize, 0, (const sockaddr*)&address, sizeof(sockaddr_in) );

		//check if data is successfully sent
		if ( sentBytes != dataSize )
		{
			printf( "Udp::send; failed to send packet\n" );
			return false;
		}
		return true;
	}

	int Udp::receive(Address &source, char* data, int dataSize)
	{
		assert(data);
		assert(dataSize > 0);

		if (!isOpen())
		{
			printf( "Udp::receive; cannot receive; socket is not open\n" );
			return 0;
		}

#if defined(WIN32)
		typedef int socklen_t;
#endif
		//receive data
		sockaddr_in from;
		socklen_t fromLength = sizeof(from);

		int receivedBytes = recvfrom(m_SocketHandle, (char*)data, dataSize, 0, (sockaddr*)&from, &fromLength);

		if(receivedBytes <= 0)
		{
			return 0;
		}

		unsigned int address = ntohl(from.sin_addr.s_addr);
		unsigned int portNo = ntohs(from.sin_port);

		source = Address(address, portNo);

		return receivedBytes;
	}

}
