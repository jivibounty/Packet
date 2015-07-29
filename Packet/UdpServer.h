
#pragma once

#include <Packet\UdpConnection.h>
#include <Packet\Address.h>
#include <map>

namespace Packet
{

	class PACKET_API UdpServer : public UdpConnection
	{
	public:
		//constructor
		UdpServer(unsigned int protocolID, float fConnectionTimeOut, unsigned int maxNumClients);

		//starts listening for connections from clients
		virtual void listen();

		//checks if its listening to connections
		virtual bool isListening() const { return m_bIsListening; }

		//stops listening for client connections
		virtual void stop();

		//gets number of clients
		virtual unsigned int getNumberOfClients() const { return m_mClientInfo.size(); }

		//gets maximum number of clients
		virtual unsigned int getMaximumNumberOfClients() const { return m_MaxNumClients; }

		//updates server
		virtual void update(float dt);

		//sends data packet to destination
		bool sendPacket(const Address& destination, char* data, int dataSize);
		
		//sends data packet reliably to destination
		bool sendReliablePacket(const Address& destination, char* data, int dataSize);
		
		//receive data packet from a client
		int receivePacket(Address& source, char* data, int dataSize);
	protected:
		//destructor
		virtual ~UdpServer();
	protected:

		class ClientInfo
		{
		public:
			ClientInfo()
				: m_fConnectionInactiveTime(0.0f)
			{}
			float m_fConnectionInactiveTime;
		};
		

#pragma warning ( push )
#pragma warning ( disable : 4251 ) 
		std::map<Address, ClientInfo> m_mClientInfo;
#pragma warning ( pop )
		unsigned int m_MaxNumClients;
		bool m_bIsListening;
	};

}
