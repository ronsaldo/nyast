#ifndef NYAST_DLL_EXPORT_HPP
#define NYAST_DLL_EXPORT_HPP

#pragma once

#ifdef _WIN32
#	define NYAST_EXPORT __declspec(dllexport)
#	define NYAST_IMPORT __declspec(dllimport)
#else
#	define NYAST_EXPORT
#	define NYAST_IMPORT
#endif

#ifdef NYAST_CORE_BUILD_DLL
#	define NYAST_CORE_EXPORT NYAST_EXPORT
#else
#	define NYAST_CORE_EXPORT NYAST_IMPORT
#endif

#endif //NYAST_DLL_EXPORT_HPP
