
#include <Packet\UdpConnection.h>
#include <Packet\Udp.h>
#include <Packet\Utils.h>
#include <Packet\Constants.h>
#include <string.h>
#include <stdio.h>

namespace Packet
{

	UdpConnection::UdpConnection(unsigned int protocolID, float fConnectionTimeOut)
		: m_ProtocolID(protocolID)
		, m_fConnectionTimeOut(fConnectionTimeOut)
		, m_pUdp(NULL)
		, m_pUdpReliability(NULL)
		, m_pUdpAck(NULL)
		, m_fAckTimeOut(20.0f)
		, m_fReliabilityTimeOut(20.0f)
	{
	}

	UdpConnection::~UdpConnection()
	{
		stop();
	}

	void UdpConnection::setAckTimeOut(float fAckTimeOut)
	{
		m_fAckTimeOut = fAckTimeOut;
		if(m_pUdpAck)
		{
			m_pUdpAck->setAckTimeOut(fAckTimeOut);
		}
	}

	void UdpConnection::setReliabilityTimeOut(float fReliabilityTimeOut)
	{
		m_fReliabilityTimeOut = fReliabilityTimeOut;
		if(m_pUdpReliability)
		{
			m_pUdpReliability->setPacketTimeOut(fReliabilityTimeOut);
		}
	}

	bool UdpConnection::start(unsigned short portNo)
	{
		//stop if previously started
		stop();
		bool bSuccess = false;
		if(m_pUdp == NULL)
		{
			m_pUdp = new Udp;
			bSuccess = m_pUdp->initialize(portNo);
			m_pUdpReliability = new UdpReliability(m_pUdp);
			m_pUdpReliability->setPacketTimeOut(m_fReliabilityTimeOut);
			m_pUdpAck = new UdpAck;
			m_pUdpAck->setAckTimeOut(m_fAckTimeOut);
		}
		return bSuccess;
	}

	void UdpConnection::stop()
	{
		SAFE_RELEASE(m_pUdp);
		SAFE_RELEASE(m_pUdpReliability);
		SAFE_RELEASE(m_pUdpAck);
	}

	void UdpConnection::update(float dt)
	{
		if(m_pUdpAck == NULL || m_pUdpReliability == NULL)
		{
			//UdpConnection::update; Cannot update; Udp ack system or Udp reliability system is NULL;
			return;
		}
		m_pUdpAck->update(dt);
		m_pUdpReliability->update(dt);
	}

	bool UdpConnection::send(const Address& destination, const char* data, int dataSize)
	{
		if(m_pUdp == NULL)
		{
			printf("UdpConnection::send; Cannot send data; Udp socket is NULL\n");
			return false;
		}
		char sendData[MAX_UDP_DATA_SIZE];
		unsigned int sequenceNumber = 0;
		unsigned char messageType = (unsigned char)UNRELIABLE;
		unsigned int offset = 0;
		memcpy(&sendData[offset], &m_ProtocolID, sizeof(m_ProtocolID));
		offset += sizeof(m_ProtocolID);
		memcpy(&sendData[offset], &sequenceNumber, sizeof(sequenceNumber));
		offset += sizeof(sequenceNumber);
		memcpy(&sendData[offset], &messageType, sizeof(messageType));
		offset += sizeof(messageType);
		if((dataSize + offset) > MAX_UDP_DATA_SIZE)
		{
			return false;
		}
		memcpy(&sendData[offset], data, dataSize);
		return m_pUdp->send(destination, sendData, dataSize + offset);
	}

	bool UdpConnection::sendReliable(const Address& destination, const char* data, int dataSize)
	{
		if(m_pUdp == NULL || m_pUdpReliability == NULL)
		{
			printf("UdpConnection::sendReliable; Cannot send data; Udp socket or Udp reliability system is NULL\n");
			return false;
		}
		char sendData[MAX_UDP_DATA_SIZE];
		unsigned int sequenceNumber = m_IndexGenerator.generateIndex();
		unsigned char messageType = (unsigned char)RELIABLE;
		unsigned int offset = 0;
		memcpy(&sendData[offset], &m_ProtocolID, sizeof(m_ProtocolID));
		offset += sizeof(m_ProtocolID);
		memcpy(&sendData[offset], &sequenceNumber, sizeof(sequenceNumber));
		offset += sizeof(sequenceNumber);
		memcpy(&sendData[offset], &messageType, sizeof(messageType));
		offset += sizeof(messageType);
		if((dataSize + offset) > MAX_UDP_DATA_SIZE)
		{
			return false;
		}
		memcpy(&sendData[offset], data, dataSize);
		return m_pUdpReliability->send(destination, sendData, dataSize + offset, sequenceNumber);
	}

	int UdpConnection::receive(Address& source, char* data, int dataSize, bool& bIsTerminalMessage)
	{
		bIsTerminalMessage = true;
		if(m_pUdp == NULL || m_pUdpAck == NULL || m_pUdpReliability == NULL)
		{
			printf("UdpConnection::receive; Cannot receive data; Udp socket or Udp Ack system or Udp reliability system is NULL\n");
			return 0;
		}
		
		char receiveData[MAX_UDP_DATA_SIZE];
		int receiveDataSize = MAX_UDP_DATA_SIZE;
		Address receiveSource;
		int retSize = m_pUdp->receive(receiveSource, receiveData, receiveDataSize);
		if(retSize <= 0)
		{
			return 0;
		}
		else
		{
			bIsTerminalMessage = false;
		}

		if(retSize > MAX_UDP_DATA_SIZE)
		{
			printf("UdpConnection::receive; Cannot receive data; Message larger than maximum allowed data size\n");
			return 0;
		}
		unsigned int offset = 0;
		unsigned int protocolID = 0;
		memcpy(&protocolID, &receiveData[offset], sizeof(protocolID));
		offset += sizeof(protocolID);
		if(protocolID != m_ProtocolID)
		{
			printf("UdpConnection::receive; Packet Protocol ID does not match\n");
			return 0;
		}

		unsigned int sequenceNumber = 0;
		memcpy(&sequenceNumber, &receiveData[offset], sizeof(sequenceNumber));
		offset += sizeof(sequenceNumber);

		unsigned char messageType = (unsigned char)RELIABLE;
		memcpy(&messageType, &receiveData[offset], sizeof(messageType));
		offset += sizeof(messageType);

		if(messageType == (unsigned char)RELIABLE)
		{
			//send ack
			char ackData[100];
			unsigned char ackMessageType = (unsigned char)ACK;
			unsigned int ackOffset = 0;
			memcpy(&ackData[ackOffset], &m_ProtocolID, sizeof(m_ProtocolID));
			ackOffset += sizeof(m_ProtocolID);
			memcpy(&ackData[ackOffset], &sequenceNumber, sizeof(sequenceNumber));
			ackOffset += sizeof(sequenceNumber);
			memcpy(&ackData[ackOffset], &ackMessageType, sizeof(ackMessageType));
			int ackDataSize = ackOffset + sizeof(messageType);
			m_pUdp->send(receiveSource, ackData, ackDataSize);
			//register ack
			if(!m_pUdpAck->isAcked(receiveSource, sequenceNumber))
			{
				m_pUdpAck->acknowledge(receiveSource, sequenceNumber);
			}
			else
			{
				//return
				return 0;
			}
		}
		else if(messageType == (unsigned char)ACK)
		{
			//remove from reliable packets since it is acknowledged
			m_pUdpReliability->remove(sequenceNumber);
			return 0;
		}
		int size = retSize - offset;
		if(size <= 0)
		{
			printf("UdpConnection::receive; size of data payload is 0\n");
			return false;
		}
		memcpy(data, &receiveData[offset], size);
		source = receiveSource;
		return size;

	}

}
