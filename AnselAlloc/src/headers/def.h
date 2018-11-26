#pragma once

#ifndef ALLOC_EXPORT
#	 define ALLOC_API __declspec(dllimport)
#else
#	 define ALLOC_API __declspec(dllexport)
#endif

