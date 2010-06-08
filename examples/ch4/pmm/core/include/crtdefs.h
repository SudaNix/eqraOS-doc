#ifndef CRTDEFS_H
#define CRTDEFS_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
	#pragma once
#endif

#if !defined (CRT_EXPORTS) && !defined (CRT_IMPORTS)
#define CRT_EXPORTS
#endif

#if defined (_WIN32)
#if defined (_MSC_VER)

//MSVC++ specific

#ifndef i386
#define i386
#endif

#if defined ( BUILD_STATIC )
#define _CRTIMP
#else // build dyanimc
#ifdef CRT_EXPORTS
#define _CRTIMP __declspec (dllexport)
#else
#define _CRTIMP __declspec (dllimport)
#endif
#endif

#else //MSVC++
#error "The Win32 environment is currently  not supported."
#endif

#else //WIN32
#error "Platform is not implemented"
#endif

#undef far
#undef near
#undef pascal

#define far
#define near

#ifdef _WIN32
#if (!defined(_MAC)) && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
#define pascal __stdcall
#else
#define pascal
#endif
#endif

#ifdef _MAC
#ifndef _CRTLIB
#define _CRTLIB    __cdecl
#endif
#ifdef _68K_
#ifndef __pascal
#define __pascal
#endif
#endif
#elif defined( _WIN32)
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#ifndef _CRTLIB
#define _CRTLIB __stdcall
#endif
#else
#ifndef _CRTLIB
#define _CRTLIB
#endif
#endif
#endif

#endif //CRTDEFS_H