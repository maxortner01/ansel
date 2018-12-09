#pragma once

#include "def.h"

#include <vector>
#include <string>

namespace Ansel
{
	class StringList
	{
		std::vector<std::string> list;
		int indexes[128] = { -1 };

	public:
		unsigned int ANSEL_API getIndex(std::string str) {

		}

		void ANSEL_API pushString(std::string str) {
			if (str.length() == 0) return;

			char c = str[0];
			if (indexes[c] != -1) {

			}
			else {

			}
		}

		void ANSEL_API deleteString(std::string str) {

		}

		void ANSEL_API deleteIndex(unsigned int index) {

		}
	};
}
