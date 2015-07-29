
#pragma once

#include <Packet\CompileSettings.h>
#include <Packet\ReferenceCounter.h>
#include <Packet\UdpAck.h>
#include <Packet\UdpReliability.h>
#include <Packet\IndexGenerator.h>

namespace Packet
{

	class Udp;

	class PACKET_API UdpConnection : public ReferenceCounter
	{
	public:
		//constructor
		UdpConnection(unsigned int protocolID, float fConnectionTimeOut);

		//sets packet acknowledgement timeout
		virtual void setAckTimeOut(float fAckTimeOut);

		//gets packet acknowledgement timeout
		virtual float getAckTimeOut() const { return m_fAckTimeOut; }

		//sets reliable packet time out
		virtual void setReliabilityTimeOut(float fReliabilityTimeOut);

		//gets reliable packet timeout
		virtual float getReliabilityTimeOut() const { return m_fReliabilityTimeOut; }

		//start connection socket
		virtual bool start(unsigned short portNo);

		//stop connection socket
		virtual void stop();
		
		//update connection
		virtual void update(float dt);
	protected:
		//destructor
		virtual ~UdpConnection();

		//sends data packet to destination
		bool send(const Address& destination, char* data, int dataSize);
		
		//sends data packet reliably to destination
		bool sendReliable(const Address& destination, char* data, int dataSize);
		
		//receives data packet
		int receive(Address& source, char* data, int dataSize, bool& bIsTerminalMessage);
	protected:

		enum MessageType
		{
			RELIABLE = 0,
			UNRELIABLE,
			ACK
		};
		unsigned int m_ProtocolID;
		float m_fConnectionTimeOut;
		float m_fReliabilityTimeOut;
		float m_fAckTimeOut;
		Udp* m_pUdp;
		IndexGenerator m_IndexGenerator;
		UdpReliability* m_pUdpReliability;
		UdpAck* m_pUdpAck;
	};

}
