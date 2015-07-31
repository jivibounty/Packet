
#include <Packet\UdpServer.h>

namespace Packet
{

	UdpServer::UdpServer(unsigned int protocolID, float fConnectionTimeOut, unsigned int maxNumClients)
		: UdpConnection(protocolID, fConnectionTimeOut)
		, m_bIsListening(false)
		, m_MaxNumClients(maxNumClients)
	{
	}

	UdpServer::~UdpServer()
	{
		m_mClientInfo.clear();
	}

	void UdpServer::listen()
	{
		m_bIsListening = true;
	}

	void UdpServer::stop()
	{
		m_bIsListening = false;
		UdpConnection::stop();
	}

	void UdpServer::update(float dt)
	{
		std::map<Address, ClientInfo>::iterator itrPrevious;
		bool bDeletePrevious = false;
		for(std::map<Address, ClientInfo>::iterator itr = m_mClientInfo.begin(); itr != m_mClientInfo.end(); ++itr)
		{
			if(bDeletePrevious)
			{
				if(m_pUdpAck)
				{
					m_pUdpAck->removeAddressAck(itrPrevious->first);
				}
				m_mClientInfo.erase(itrPrevious);
				bDeletePrevious = false;
			}
			itr->second.m_fConnectionInactiveTime += dt;
			if(itr->second.m_fConnectionInactiveTime >= m_fConnectionTimeOut)
			{
				itrPrevious = itr;
				bDeletePrevious = true;
			}
		}
		if(bDeletePrevious)
		{
			if(m_pUdpAck)
			{
				m_pUdpAck->removeAddressAck(itrPrevious->first);
			}
			m_mClientInfo.erase(itrPrevious);
			bDeletePrevious = false;
		}

		UdpConnection::update(dt);
	}

	bool UdpServer::sendPacket(const Address& destination, const char* data, int dataSize)
	{
		return send(destination, data, dataSize);
	}

	bool UdpServer::sendReliablePacket(const Address& destination, const char* data, int dataSize)
	{
		return sendReliable(destination, data, dataSize);
	}

	int UdpServer::receivePacket(Address& source, char* data, int dataSize)
	{
		if(!isListening())
		{
			printf("UdpServer::receivePacket; Cannot receive packet; server is not listening\n");
			return 0;
		}
		bool bIsTerminalMessage = false;
		int retSize = 0;
		while(retSize == 0 && !bIsTerminalMessage)
		{
			retSize = receive(source, data, dataSize, bIsTerminalMessage);
			if(retSize > 0)
			{
				std::map<Address, ClientInfo>::iterator itr = m_mClientInfo.find(source);
				if(itr == m_mClientInfo.end())
				{
					if(m_mClientInfo.size() >= m_MaxNumClients)
					{
						continue;
					}
					ClientInfo clientInfo;
					m_mClientInfo[source] = clientInfo;
				}
				else
				{
					itr->second.m_fConnectionInactiveTime = 0.0f;
				}
				break;
			}
		}
		if(retSize <= 0)
		{
			return 0;
		}
		
		return retSize;
	}

}
