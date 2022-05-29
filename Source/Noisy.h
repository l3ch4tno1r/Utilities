#pragma once

#include <iostream>

#include "InstanceCounter.h"

class Noisy : public Counter<Noisy>
{
public:
	Noisy()                   { std::cout << "#" << this->Id() << " - default ctor." << std::endl; }
	Noisy(const Noisy& other) { std::cout << "#" << this->Id() << " - copy ctor from " << other.Id() << '.' << std::endl; }
	Noisy(Noisy&& other)      { std::cout << "#" << this->Id() << " - move ctor from " << other.Id() << '.' << std::endl; }

	Noisy operator=(const Noisy& other) { std::cout << "#" << this->Id() << " - copy assign from " << other.Id() << '.' << std::endl; }
	Noisy operator=(Noisy&& other)      { std::cout << "#" << this->Id() << " - move assign from " << other.Id() << '.' << std::endl; }

	~Noisy() { std::cout << "#" << this->Id() << " - dtor." << std::endl; }
};