#pragma once

#include <iostream>

template<typename T>
short sign(T a)
{
	return (a > T(0)) - (a < T(0));
}

#define BIT(X) (1 << X)