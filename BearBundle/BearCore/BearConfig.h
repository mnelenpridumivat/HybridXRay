#pragma once
#define PLATFORM_WINDOWS	0
#define PLATFORM_LINUX		1
#define COMPILER_MSVC		0
#define COMPILER_GCC		1
#define PROCCESOR_X86		0
#define PROCCESOR_AMD64		1

#ifdef _WIN32
#define CURRENT_PLATFORM PLATFORM_WINDOWS
#else
#error "Unkown Platform"
#endif

#ifdef _MSC_VER
#define CURRENT_COMPILER COMPILER_MSVC
#elif __GNUC__
#define CURRENT_COMPILER COMPILER_GCC
#else 
#error "Unkown Compiller"
#endif

#if CURRENT_PLATFORM == PLATFORM_WINDOWS
#ifdef _M_AMD64
#define CURRENT_PROCCESOR PROCCESOR_AMD64
#else
#define CURRENT_PROCCESOR PROCCESOR_X86
#endif
#elif CURRENT_PLATFORM == PLATFORM_LINUX 
#define CURRENT_PROCCESOR PROCCESOR_AMD64
#endif

#ifndef FINAL
#define DEVELOPER_VERSION
#endif