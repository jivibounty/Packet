
#include <Packet\UdpAck.h>
#include <Packet\UdpSingleAddressAck.h>
#include <map>

namespace Packet
{

	UdpAck::UdpAck()
		: m_fAckTimeOut(20.0f)
	{
	}

	UdpAck::~UdpAck()
	{
		for(std::map<Address, UdpSingleAddressAck*>::iterator itr = m_mUdpAddressAcks.begin(); itr != m_mUdpAddressAcks.end(); ++itr)
		{
			SAFE_RELEASE(itr->second);
		}
		m_mUdpAddressAcks.clear();
	}

	void UdpAck::setAckTimeOut(float fAckTimeOut)
	{
		m_fAckTimeOut = fAckTimeOut;
		for(std::map<Address, UdpSingleAddressAck*>::iterator itr = m_mUdpAddressAcks.begin(); itr != m_mUdpAddressAcks.end(); ++itr)
		{
			if(itr->second)
			{
				itr->second->setAckTimeOut(fAckTimeOut);
			}
		}
	}

	void UdpAck::acknowledge(const Address& address, unsigned int sequenceNumber)
	{
		UdpSingleAddressAck* pUdpSingleAddressAck = NULL;
		std::map<Address, UdpSingleAddressAck*>::iterator itr = m_mUdpAddressAcks.find(address);
		if(itr == m_mUdpAddressAcks.end())
		{
			pUdpSingleAddressAck = new UdpSingleAddressAck;
			pUdpSingleAddressAck->setAckTimeOut(m_fAckTimeOut);
			m_mUdpAddressAcks[address] = pUdpSingleAddressAck;
		}
		else
		{
			pUdpSingleAddressAck = itr->second;
		}
		pUdpSingleAddressAck->acknowledge(sequenceNumber);
	}

	bool UdpAck::isAcked(const Address& address, unsigned int sequenceNumber) const
	{
		std::map<Address, UdpSingleAddressAck*>::iterator itr = m_mUdpAddressAcks.find(address);
		if(itr == m_mUdpAddressAcks.end())
		{
			return false;
		}
		if(!itr->second)
		{
			return false;
		}
		return itr->second->isAcked(sequenceNumber);
	}

	void UdpAck::removeAddressAck(const Address& address)
	{
		std::map<Address, UdpSingleAddressAck*>::iterator itr = m_mUdpAddressAcks.find(address);
		if(itr != m_mUdpAddressAcks.end())
		{
			SAFE_RELEASE(itr->second);
			m_mUdpAddressAcks.erase(itr);
		}
	}

	void UdpAck::update(float dt)
	{
		for(std::map<Address, UdpSingleAddressAck*>::iterator itr = m_mUdpAddressAcks.begin(); itr != m_mUdpAddressAcks.end(); ++itr)
		{
			if(itr->second)
			{
				itr->second->update(dt);
			}
		}
	}

}
