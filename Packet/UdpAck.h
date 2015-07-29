
#pragma once

#include <Packet\CompileSettings.h>
#include <Packet\ReferenceCounter.h>
#include <Packet\Address.h>
#include <map>

namespace Packet
{

	class UdpSingleAddressAck;

	class PACKET_API UdpAck : public ReferenceCounter
	{
	public:
		//constructor
		UdpAck();

		//sets packet acknowledgement time out
		void setAckTimeOut(float fAckTimeOut);
		
		//gets packet acknowledgement time out
		float getAckTimeOut() const { return m_fAckTimeOut; }

		//acknowlegdes packet reception from address and with sequence number
		void acknowledge(const Address& address, unsigned int sequenceNumber);

		//check if packet from address with sequence number is already acked at least once
		bool isAcked(const Address& address, unsigned int sequenceNumber) const;

		//remove all packet acknowledgements for address
		void removeAddressAck(const Address& address);

		//update packet acknowledgements
		void update(float dt);
	protected:
		//destructor
		virtual ~UdpAck();
	protected:

		class AckData
		{
		public:
			AckData(float fAge = 0.0f)
				: m_fAge(fAge)
			{}
			float m_fAge;
		};

#pragma warning ( push )
#pragma warning ( disable : 4251 ) 
		mutable std::map<Address, UdpSingleAddressAck*> m_mUdpAddressAcks;
#pragma warning ( pop )

		float m_fAckTimeOut;
	};

}
