#include "Quadratic.h"
#include <algorithm>
#include <cmath>

void Quadratic::solve()
{
	this->d = this->b * this->b - 4 * this->a * this->c;
	if (this->d < 0)
	{
		this->solutions = 0;
		return;
	}
	if (this->d == 0)
	{
		this->solutions = 1;
		this->t1 = -this->b / (2 * this->a);
		return;
	}
	this->solutions = 2;
	this->t1 = (-this->b + sqrt(this->d)) / (2 * this->a);
	this->t2 = (-this->b - sqrt(this->d)) / (2 * this->a);
}

float Quadratic::getMinPosT()
{
	if (this->solutions == 1)
		return (this->t1);
	if (this->solutions == 2)
	{
		if (this->t1 < 0)
			return (this->t2);
		if (this->t2 < 0)
			return (this->t1);
		return (std::min(this->t1, this->t2));
	}
	return (-1);
}
