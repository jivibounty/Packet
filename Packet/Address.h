

#pragma once

#include <Packet\CompileSettings.h>

namespace Packet
{

	class PACKET_API Address
	{
	public:
		//default constructor
		Address();

		//constructor
		Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short portNo);

		//constructor
		Address(unsigned int address, unsigned short portNo);
		
		//sets ipv4 address and port number
		void setAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short portNo);
		
		//sets ipv4 address and port number
		void setAddress(unsigned int address, unsigned short portNo);

		//gets ipv4 address
		unsigned int getAddress() const { return m_Address; }

		//gets port number
		unsigned short getPortNumber() const { return m_PortNo; }
		
		//less than operator specifically for stl maps
		bool operator< (const Address& address) const;
		
		//equal to operator
		bool operator== (const Address& address) const;
	protected:
		unsigned int m_Address;

		unsigned short m_PortNo;
	};

}
