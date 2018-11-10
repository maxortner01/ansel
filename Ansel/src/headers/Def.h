#pragma once

#ifdef _EXPORTING
#    define ANSEL_API __declspec(dllexport)
#    define EXPIMP_TEMPLATE
#else
#    define ANSEL_API __declspec(dllimport)
#    define EXPIMP_TEMPLATE extern
#endif

#define INSTANCE_COUNT 1000
#define LIGHT_COUNT 8

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>