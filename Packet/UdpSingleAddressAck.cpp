
#include <Packet\UdpSingleAddressAck.h>
#include <unordered_map>

namespace Packet
{

	UdpSingleAddressAck::UdpSingleAddressAck()
		: m_fAckTimeOut(20.0f)
	{
	}

	UdpSingleAddressAck::~UdpSingleAddressAck()
	{
		m_mAckData.clear();
	}

	void UdpSingleAddressAck::setAckTimeOut(float fAckTimeOut)
	{
		m_fAckTimeOut = fAckTimeOut;
	}

	void UdpSingleAddressAck::acknowledge(unsigned int sequenceNumber)
	{
		m_mAckData[sequenceNumber] = AckData(m_fAckTimeOut);
	}

	bool UdpSingleAddressAck::isAcked(unsigned int sequenceNumber) const
	{
		std::unordered_map<unsigned int, AckData>::iterator itr = m_mAckData.find(sequenceNumber);
		return itr != m_mAckData.end();
	}

	void UdpSingleAddressAck::update(float dt)
	{
		//iterate through sequence numbers and delete old ones at the end of their life span
		std::unordered_map<unsigned int, AckData>::iterator itrPrevious;
		bool bDeletePrevious = false;
		for(std::unordered_map<unsigned int, AckData>::iterator itr = m_mAckData.begin(); itr != m_mAckData.end(); ++itr)
		{
			if(bDeletePrevious)
			{
				m_mAckData.erase(itrPrevious);
				bDeletePrevious = false;
			}
			itr->second.m_fAge -= dt;
			if(itr->second.m_fAge <= 0.0f)
			{
				itrPrevious = itr;
				bDeletePrevious = true;
			}
		}
		if(bDeletePrevious)
		{
			m_mAckData.erase(itrPrevious);
			bDeletePrevious = false;
		}
	}

}
