
#include <Packet\IndexGenerator.h>

namespace Packet
{

	IndexGenerator::IndexGenerator()
		: m_LastIndex(0)
	{
	}

	unsigned int IndexGenerator::generateIndex()
	{
		++m_LastIndex;
		return m_LastIndex;
	}

}
