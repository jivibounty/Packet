
#pragma once

#include <Packet\UdpConnection.h>
#include <Packet\Address.h>

namespace Packet
{

	class PACKET_API UdpClient : public UdpConnection
	{
	public:
		//constructor
		UdpClient(unsigned int protocolID, float fConnectionTimeOut);

		//connect to server with address of the server
		virtual void connect(const Address& serverAddress);

		//check if connection is connecting
		virtual bool isConnecting() const { return m_ConnectionState == CONNECTING; }
		
		//check if connection succeeded
		virtual bool isConnected() const { return m_ConnectionState == CONNECTED; }
		
		//check if connection failed
		virtual bool connectionFailed() const { return m_ConnectionState == CONNECTION_FAILED; }

		//stop connection
		virtual void stop();

		//update client
		virtual void update(float dt);

		//send data packet to server
		bool sendPacket(const char* data, int dataSize);
		
		//send data packet reliably to server
		bool sendReliablePacket(const char* data, int dataSize);

		//receive data packet from server
		int receivePacket(char* data, int dataSize);
	protected:
		//destructor
		virtual ~UdpClient();
	protected:

		enum ConnectionState
		{
			UNCONNECTED = 0,
			CONNECTING,
			CONNECTED,
			CONNECTION_FAILED
		};
		ConnectionState m_ConnectionState;
		Address m_ServerAddress;
		float m_fConnectionInactiveTime;
	};

}
