#include <sstream>

#include "Utilities/ErrorHandling.h"

std::runtime_error buildexception(const char* expr, const char* file, int line)
{
	std::stringstream msg;

	msg << expr << " | " << file << " | " << line;

	return std::runtime_error(msg.str().c_str());
}
