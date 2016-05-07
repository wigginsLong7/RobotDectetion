#include"Robot.h"
Robot::Robot()
{
	this->x = LENGTH;
	this->y = 0;
	this->visit = false;
}

Robot::Robot(double a, double b)
{
	this->x = a;
	this->y = b;
	this->visit = false;
}

Robot::Robot(double a, double b,bool flag)
{
	this->x = a;
	this->y = b;
	this->visit = flag;
}
bool Robot::GetVist()
{
	return this->visit;
}
double Robot::GetX()
{
	return this->x;
}
double Robot::GetY()
{
	return this->y;
}
bool Robot::operator<(Robot r)
{
	if (this->x < r.GetX())
		return true;
	else
		return false;
}
void Robot::SetVisit(bool v)
{
	this->visit = v;
}
void Robot::SetX(double a)
{
	this->x = a;
}
void Robot::SetY(double a)
{
	this->y = a;
}
Robot::~Robot()
{
}
