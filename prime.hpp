#ifndef PRIME_HPP
#define PRIME_HPP

#include <math.h>

class PrimeCandidate
{
public:
	PrimeCandidate(unsigned int _number) : number(_number) {}
	
	PrimeCandidate()
	{
		number = 0;
	}

	void Set(unsigned int val)
	{
		number = val;
	}

	bool IsPrime(void)
	{
		if (number < 2)
			return false;

		if (number == 2)
			return true;

		if(number % 2 == 0)
			return false;

		for (int i = 3; i <= sqrt(number); i += 2)
		{
			if (number % i == 0)
				return false;
		}
		return true;
	}

private:
	unsigned int number;
};

#endif
