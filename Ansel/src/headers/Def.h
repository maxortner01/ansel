#pragma once

#ifdef _EXPORTING
#    define ANSEL_API __declspec(dllexport)
#    define EXPIMP_TEMPLATE
#else
#    define ANSEL_API __declspec(dllimport)
#    define EXPIMP_TEMPLATE extern
#endif