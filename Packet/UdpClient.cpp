
#include <Packet\UdpClient.h>

namespace Packet
{

	UdpClient::UdpClient(unsigned int protocolID, float fConnectionTimeOut)
		: UdpConnection(protocolID, fConnectionTimeOut)
		, m_ConnectionState(UNCONNECTED)
		, m_fConnectionInactiveTime(0.0f)
	{
	}

	UdpClient::~UdpClient()
	{
	}

	void UdpClient::connect(const Address& serverAddress)
	{
		//remove previous server address from ack
		if(m_pUdpAck)
		{
			m_pUdpAck->removeAddressAck(m_ServerAddress);
		}
		//set new server address;
		m_ServerAddress = serverAddress;
		m_ConnectionState = CONNECTING;
	}

	void UdpClient::stop()
	{
		m_ConnectionState = UNCONNECTED;
		m_ServerAddress = Address();
		UdpConnection::stop();
	}

	void UdpClient::update(float dt)
	{
		m_fConnectionInactiveTime += dt;
		if(m_fConnectionInactiveTime >= m_fConnectionTimeOut)
		{
			m_ConnectionState = CONNECTION_FAILED;
		}
		UdpConnection::update(dt);
	}

	bool UdpClient::sendPacket(const char* data, int dataSize)
	{
		return send(m_ServerAddress, data, dataSize);
	}

	bool UdpClient::sendReliablePacket(const char* data, int dataSize)
	{
		return sendReliable(m_ServerAddress, data, dataSize);
	}

	int UdpClient::receivePacket(char* data, int dataSize)
	{
		Address source;
		bool bIsTerminalMessage = false;
		int retSize = 0;
		while(retSize == 0 && !bIsTerminalMessage)
		{
			retSize = receive(source, data, dataSize, bIsTerminalMessage);
		}
		bool bSuccess = retSize > 0 && source == m_ServerAddress;
		//reset connection inactive time as data is received from the server
		if(bSuccess)
		{
			m_fConnectionInactiveTime = 0.0f;
			m_ConnectionState = CONNECTED;
		}
		else
		{
			retSize = 0;
		}
		return retSize;
	}

}
