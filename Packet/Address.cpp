
#include <Packet\Address.h>

namespace Packet
{

	Address::Address()
		: m_Address(0)
		, m_PortNo(0)
	{
	}

	Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short portNo)
	{
		setAddress(a, b, c, d, portNo);
	}

	Address::Address(unsigned int address, unsigned short portNo)
	{
		setAddress(address, portNo);
	}

	void Address::setAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short portNo)
	{
		m_Address = ( a << 24 ) | ( b << 16 ) | ( c << 8  ) | d;
		m_PortNo = portNo;
	}

	void Address::setAddress(unsigned int address, unsigned short portNo)
	{
		m_Address = address;
		m_PortNo = portNo;
	}

	bool Address::operator< (const Address& address) const
	{
		unsigned long long addr1 = (unsigned long long)m_Address << 32 | m_PortNo;
		unsigned long long addr2 = (unsigned long long)address.m_Address << 32 | address.m_PortNo;
		return addr1 < addr2;
	}

	bool Address::operator== (const Address& address) const
	{
		return m_Address == address.m_Address && m_PortNo == address.m_PortNo;
	}

}
