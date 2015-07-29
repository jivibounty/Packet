
#pragma once

#include <Packet\CompileSettings.h>
#include <Packet\ReferenceCounter.h>
#include <Packet\Address.h>

namespace Packet
{

	class PACKET_API Udp : public ReferenceCounter
	{
	public:
		//constructor
		Udp();

		//checks if udp is initialized for usage
		bool isOpen() const { return m_SocketHandle > 0; }

		//initializes udp
		bool initialize(unsigned int portNo);

		//deinitializes udp
		void deInitialize();

		//sends data
		bool send(const Address& destination, const char* data, int dataSize);

		//receives data
		int receive(Address &source, char* data, int dataSize);
	protected:
		//destructor
		virtual ~Udp();
	protected:
		int m_SocketHandle;
	};

}
