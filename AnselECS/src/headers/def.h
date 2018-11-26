#pragma once

#ifndef ECS_EXPORT
#	 define ECS_API __declspec(dllimport)
#else
#	 define ECS_API __declspec(dllexport)
#endif

typedef size_t EntityInt;