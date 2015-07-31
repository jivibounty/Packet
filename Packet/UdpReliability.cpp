
#include <Packet\UdpReliability.h>
#include <Packet\Udp.h>
#include <Packet\Utils.h>
#include <unordered_map>
#include <stdio.h>
#include <string.h>
#include <assert.h>

namespace Packet
{

	UdpReliability::UdpReliability(Udp* pUdp)
		: m_fPacketTimeOut(20.0f)
	{
		m_pUdp = NULL;
		SAFE_ASSIGN(m_pUdp, pUdp);
	}

	UdpReliability::~UdpReliability()
	{
		m_mDataPackets.clear();
		SAFE_RELEASE(m_pUdp);
	}

	void UdpReliability::setPacketTimeOut(float fPacketTimeOut)
	{
		m_fPacketTimeOut = fPacketTimeOut;
	}

	bool UdpReliability::send(const Address& destination, const char* data, int dataSize, unsigned int sequenceNumber)
	{
		assert(data);
		assert(dataSize > 0);
		//return false if m_pUdp socket is NULL
		if(m_pUdp == NULL)
		{
			printf("UdpReliability::send; udp socket is NULL\n");
			return false;
		}
		//check if dataSize exceeds MAX_RUDP_DATA_SIZE
		if(dataSize > MAX_RUDP_DATA_SIZE)
		{
			printf("UdpReliability::send; dataSize is greater than data size limit\n");
			return false;
		}
		DataPacket packet(destination, data, dataSize, sequenceNumber, m_fPacketTimeOut);

		m_mDataPackets[packet.m_SequenceNumber] = packet;
		//send packet immediately first
		m_pUdp->send(destination, data, dataSize);
		return true;
	}

	void UdpReliability::remove(unsigned int sequenceNumber)
	{
		//remove packet with specified sequence number
		std::unordered_map<unsigned int, DataPacket>::iterator itr = m_mDataPackets.find(sequenceNumber);
		if(itr != m_mDataPackets.end())
		{
			m_mDataPackets.erase(itr);
		}
	}

	bool UdpReliability::update(float dt)
	{
		//return false if m_pUdp socket is NULL
		if(m_pUdp == NULL)
		{
			printf("UdpReliability::resend ; udp socket is NULL\n");
			return false;
		}
		std::unordered_map<unsigned int, DataPacket>::iterator itrPrevious;
		bool bDeletePrevious = false;
		for(std::unordered_map<unsigned int, DataPacket>::iterator itr = m_mDataPackets.begin(); itr != m_mDataPackets.end(); ++itr)
		{
			if(bDeletePrevious)
			{
				m_mDataPackets.erase(itrPrevious);
				bDeletePrevious = false;
			}
			itr->second.m_fAge -= dt;
			if(itr->second.m_fAge <= 0.0f)
			{
				itrPrevious = itr;
				bDeletePrevious = true;
			}
			else
			{
				m_pUdp->send(itr->second.m_Address, itr->second.m_Data, itr->second.m_DataSize);
			}
		}
		if(bDeletePrevious)
		{
			m_mDataPackets.erase(itrPrevious);
			bDeletePrevious = false;
		}
		return true;
	}

	UdpReliability::DataPacket::DataPacket()
		: m_DataSize(0)
		, m_SequenceNumber(0)
		, m_fAge(0.0f)
	{
	}

	UdpReliability::DataPacket::DataPacket(const Address& address, const char* data, int dataSize, unsigned int sequenceNumber, float fAge)
	{
		m_Address = address;
		memcpy(m_Data, data, dataSize);
		m_DataSize = dataSize;
		m_SequenceNumber = sequenceNumber;
		m_fAge = fAge;
	}

}
