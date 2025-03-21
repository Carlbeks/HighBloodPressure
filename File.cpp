
#include "def.h"

#include "exception.h"
#include "File.h"

File FileAccessor::getAccess(const String& path) {
	namespace fs = std::filesystem;
	using Path = fs::path;
	Path original = path;
	Path p = original.parent_path();
	if (!fs::exists(p)) {
		create_directories(p);
		Logger.info(L"Created directory " + p.wstring());
	}
	return File(p);
}
