
#ifndef  ROBOTMACHINE
#define ROBOTMACHINE

#define  RADIUS                             10  
#define  LENGTH                             500
#define  MAX_ROBOT_NUM                      LENGTH/RADIUS+3
class Robot
{
public:
	Robot();
	Robot(double,double);
	Robot(double, double,bool);
	~Robot();
	void SetVisit(bool);
	bool GetVist();
	void SetX(double);
	void SetY(double);
	double GetX();
	double GetY();
	bool operator<(Robot);
private:
	double x;
	double y;
	bool visit;

};
#endif // ! ROBOTMACHINE
