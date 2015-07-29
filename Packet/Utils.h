
#pragma once

#define MAX_UNSIGNED_INT16 65536
#define MAX_UNSIGNED_INT32 4294967295

#define EPSILON 0.00001f
#define PI 3.1415926535897932384626433832795028841971693993751f
#define PI_RAD 0.01745329251994329576923690768489f

#define GET_NETWORK_ADDRESS_LONG(a, b, c, d)  ( a << 24 ) | ( b << 16 ) | ( c << 8  ) | d;

#ifndef ENCODE_BIT
#define ENCODE_BIT(input, bit) input |= (1<<bit)
#endif

#ifndef MASK_OUT_BIT
#define MASK_OUT_BIT(input, bit) input &= ~(1 << bit)
#endif

#ifndef DECODE_BIT
#define DECODE_BIT(input, bit) (input >> bit) & 1
#endif

#ifndef MOVE_CIRCULAR_BUFFER_BACKWARD
#define MOVE_CIRCULAR_BUFFER_BACKWARD(pos, moveno, maxsize) { pos -= moveno; while(pos < 0) { pos += maxsize; } }
#endif

#ifndef MOVE_CIRCULAR_BUFFER_FORWARD
#define MOVE_CIRCULAR_BUFFER_FORWARD(pos, moveno, maxsize) { pos += moveno; while(pos >= maxsize) { pos -= maxsize; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(ptr) if(ptr != NULL){ptr->release();ptr = NULL;}
#endif

#ifndef SAFE_RELEASE_VECTOR
#define SAFE_RELEASE_VECTOR(ptr) for(Bounty::uint32 n = 0; n < ptr.size(); ++n){ SAFE_RELEASE(ptr[n]);} ptr.clear();
#endif

#ifndef SAFE_ASSIGN
#define SAFE_ASSIGN(a, b) if(a != b) { SAFE_RELEASE(a); a = b; if (a != NULL) { a->addRef(); } }
#endif

#ifndef SAFE_PUSH_BACK_VECTOR
#define SAFE_PUSH_BACK_VECTOR(a, b) if (b != NULL) { a.push_back(b); b->addRef(); }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(ptr) if(ptr != NULL){delete [] ptr;ptr = NULL;}
#endif

#ifndef SAFE_DELETE_VECTOR
#define SAFE_DELETE_VECTOR(ptr) for(Bounty::uint32 n = 0; n < ptr.size(); ++n) {SAFE_DELETE(ptr[n]);} ptr.clear();
#endif

#ifndef SAFE_DELETE_ARRAY_VECTOR
#define SAFE_DELETE_ARRAY_VECTOR(ptr) for(Bounty::uint32 n = 0; n < ptr.size(); ++n) {SAFE_DELETE_ARRAY(ptr[n]);} ptr.clear();
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) if(ptr != NULL){delete ptr;ptr = NULL;}
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) if(ptr != NULL){free(ptr);ptr = NULL;}
#endif

#ifndef SAFE_ALIGNED_FREE
#define SAFE_ALIGNED_FREE(ptr) if(ptr != NULL){_aligned_free(ptr);ptr = NULL;}
#endif

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

template <class  T>
void Swap(T a, T b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
