
#pragma once

#include <Packet\CompileSettings.h>
#include <Packet\ReferenceCounter.h>
#include <Packet\Address.h>
#include <Packet\Constants.h>
#include <unordered_map>

namespace Packet
{

	class Udp;

	class PACKET_API UdpReliability : public ReferenceCounter
	{
	public:
		//constructor
		UdpReliability(Udp* pUdp);

		//sets packet reliability time out
		void setPacketTimeOut(float fPacketTimeOut);
		
		//gets packet reliability time out
		float getPacketTimeOut() const { return m_fPacketTimeOut; }

		//send data packet reliably to destination
		bool send(const Address& destination, char* data, int dataSize, unsigned int sequenceNumber);

		//removes data packet with sequence number from storage
		void remove(unsigned int sequenceNumber);

		//updates reliable packets
		bool update(float dt);
	protected:
		//destructor
		virtual ~UdpReliability();
	protected:

		class DataPacket
		{
		public:
			DataPacket();
			DataPacket(const Address& address, char* data, int dataSize, unsigned int sequenceNumber, float fAge);
			Address m_Address;
			char m_Data[MAX_RUDP_DATA_SIZE];
			int m_DataSize;
			unsigned int m_SequenceNumber;
			float m_fAge;
		};

#pragma warning ( push )
#pragma warning ( disable : 4251 ) 
		std::unordered_map<unsigned int, DataPacket> m_mDataPackets;
#pragma warning ( pop )

		Udp* m_pUdp;

		float m_fPacketTimeOut;
	};

}
