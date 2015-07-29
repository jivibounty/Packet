
#pragma once

#include <Packet\CompileSettings.h>

namespace Packet
{

	class PACKET_API IndexGenerator
	{
	public:
		//constructor
		IndexGenerator();

		//generates new index
		unsigned int generateIndex();
	private:
		unsigned int m_LastIndex;
	};

}

