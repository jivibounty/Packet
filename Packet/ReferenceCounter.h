
#pragma once

#include <Packet\CompileSettings.h>
#include <Packet\Utils.h>

namespace Packet
{

	class PACKET_API ReferenceCounter
	{
	public:
		ReferenceCounter() :
		  m_Count(1)
		  {
		  }

		  //adds reference
		  void addRef()
		  {
			  ++m_Count;
		  }

		  //releases reference
		  void release()
		  {
			  if (--m_Count <= 0)
			  {
				  delete this;
			  }
		  }

		  //gets reference count
		  int getReferenceCount()
		  {
			  return m_Count;
		  }
	protected:
		  virtual ~ReferenceCounter(){}
	private:
		int m_Count;
	};

}
