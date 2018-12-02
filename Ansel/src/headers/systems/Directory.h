#pragma once

#include "../Def.h"

namespace Ansel
{
	struct Directory
	{
		enum STATUS
		{
			EXISTS,   // Folder/file exists
			NO_DIR,   // Folder/file does not exist
			NO_ACCESS // Can't get access to folder/file
		} typedef status;

		static status checkFolder(const char* dir_name);
		static bool   makeFolder(const char* _directory);
	};
}