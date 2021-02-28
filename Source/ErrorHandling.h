#pragma once

#include <exception>

#ifdef DEBUG
#define ASSERT(X) if(!(X)) __debugbreak()
#else
#define ASSERT(X)
#endif // !DEBUG


std::exception buildexception(const char* expr, const char* file, int line);

#define THROWEXCEPTIF(X) if(X) throw buildexception(#X, __FILE__, __LINE__);

#define BREACKIF(X) if(X) __debugbreak()