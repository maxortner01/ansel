#include "../../headers/systems/Directory.h"

#include <sys/stat.h>
#include <direct.h>

struct stat info;

namespace Ansel
{
	Directory::status Directory::checkFolder(const char* dir_name) {
		/**/ if ( stat(dir_name, &info) != 0 ) return NO_ACCESS;
		else if ( info.st_mode & S_IFDIR     ) return EXISTS;
		
		return NO_DIR;
	}

	bool Directory::makeFolder(const char* _directory) {
		return _mkdir(_directory);
	}
}