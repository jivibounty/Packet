
#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	#define PACKET_WINAPI
#elif defined MACOSX
	#define PACKET_MACOSX
#else
	#define PACKET_LINUX
#endif

#if defined(_MSC_VER)
	#if defined(PACKET_STATICLIB)
		#define PACKET_API
	#else
		#if defined(PACKET_SOURCE)
			// If we are building the DLL, export the symbols tagged like this
			#define PACKET_API __declspec(dllexport)
		#else
			// If we are consuming the DLL, import the symbols tagged like this
			#define PACKET_API __declspec(dllimport)
		#endif
	#endif
#elif defined(__GNUC__)
	#if defined(PACKET_STATICLIB)
		#define PACKET_API
	#else
		#if defined(PACKET_SOURCE)
			#define PACKET_API __attribute__ ((visibility ("default")))
		#else
			// If you use -fvisibility=hidden in GCC, exception handling and RTTI
			// would break if visibility wasn't set during export _and_ import
			// because GCC would immediately forget all type infos encountered.
			// See http://gcc.gnu.org/wiki/Visibility
			#define PACKET_API __attribute__ ((visibility ("default")))
		#endif
	#endif
#else
	#ifdef PACKET_WINAPI || __CYGWIN__
		#ifdef PACKET_EXPORTS
			#define PACKET_API __declspec(dllexport)
		#else
			#define PACKET_API __declspec(dllimport)
		#endif
	#elif PACKET_LINUX || PACKET_MACOSX
		#define PACKET_API
	#else
		#error Unknown compiler/platform, please implement shared library macros
	#endif
#endif

#if defined(_WIN32)
	#define PACKET_CALLBACK __stdcall
#else
    #define PACKET_CALLBACK
#endif

#if defined( _MSC_VER )

	#define PACKET_BEGIN_ALIGNED( n, a )                            __declspec( align( a ) ) n
	#define PACKET_END_ALIGNED( n, a )

#elif defined( __GNUC__ )

	#define PACKET_BEGIN_ALIGNED( n, a )                            n
	#define PACKET_END_ALIGNED( n, a )                              __attribute__( (aligned( a ) ) )

#else
		#error Unknown compiler/platform, please implement shared library macros
#endif

