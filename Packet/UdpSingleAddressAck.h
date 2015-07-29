
#pragma once

#include <Packet\CompileSettings.h>
#include <Packet\ReferenceCounter.h>
#include <unordered_map>

namespace Packet
{

	class PACKET_API UdpSingleAddressAck : public ReferenceCounter
	{
	public:
		//constructor
		UdpSingleAddressAck();

		//sets packet acknowledgement time out
		void setAckTimeOut(float fAckTimeOut);
		
		//gets packet acknowledgement time out
		float getAckTimeOut() const { return m_fAckTimeOut; }

		//acknowlegdes packet reception
		void acknowledge(unsigned int sequenceNumber);
		
		//check if packet with sequence number is already acked at least once
		bool isAcked(unsigned int sequenceNumber) const;
		
		//update packet acknowledgements
		void update(float dt);
	protected:
		//destructor
		virtual ~UdpSingleAddressAck();
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
		mutable std::unordered_map<unsigned int, AckData> m_mAckData;
#pragma warning ( pop )

		float m_fAckTimeOut;
	};

}
