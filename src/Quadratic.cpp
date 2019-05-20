#include "Quadratic.h"
#include "Consts.h"
#include <algorithm>
#include <cmath>

bool Quadratic::solve()
{
	this->d = this->b * this->b - 4 * this->a * this->c;
	if (this->d < 0)
		return false;
	if (this->d == 0)
	{
		this->solutions = 1;
		this->t1 = -this->b / (2 * this->a);
		this->minT = this->t1;
		return true;
	}
	this->solutions = 2;
	this->t1 = (-this->b + sqrt(this->d)) / (2 * this->a);
	this->t2 = (-this->b - sqrt(this->d)) / (2 * this->a);
	if (this->t1 < EPSILON)
		this->minT = this->t2;
	else if (this->t2 < EPSILON)
		this->minT = this->t1;
	else
		this->minT = std::min(this->t1, this->t2);
	return true;
}

float Quadratic::getMinPosT()
{
	return this->minT;
}
