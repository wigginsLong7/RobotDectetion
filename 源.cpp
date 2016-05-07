#include<iostream>
#include<math.h>
#include<algorithm>
#include<sstream>
#include<time.h>
#include"VideoHandler.h"
using namespace std;
using namespace cv;


#define  ROBOT_MIN_RATIO                    1.25
#define  ROBOT_MAX_RATIO                    2.0

bool initialize(Robot[MAX_ROBOT_NUM], int&);
void CheckNotused(Robot[MAX_ROBOT_NUM], int, bool,int);
double FindClosestNotUsed(int, Robot[MAX_ROBOT_NUM], int, int &,bool);
bool update(int, int, Robot[MAX_ROBOT_NUM], bool);
int judgeStopPos(int, int, bool);
void printrobot(Robot[MAX_ROBOT_NUM], int);
bool check(Robot[MAX_ROBOT_NUM], int);

int main()
{
	///---------------------------------- initialize-------------------------------------//
	int N = 0;                            // actual number of robot
	Robot robot[MAX_ROBOT_NUM];           // robot set
	robot[0].SetX(0);
	VideoHandler handler = VideoHandler();
	if (!initialize(robot, N))
	{
		system("pause");
		return 0;
	}
	//--------------------------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////------------------------ algorithm-------------------------/////////////////////////////////
	// global value
	int i = 2;                     // left_sdie_index
	int j = N-1;                     // right_side_index
	int left_total_move = 0;       // record left side_total move distance
	int right_total_move = 0;      // record right side_total move distance
	double left_pos = 0;           // left_boundary
	double right_pos = LENGTH;     // right_boundary
	handler.drawpicture(robot, N, left_pos, right_pos, VIDEODPS);

	//----------------------------------two boundary setting--------------------------------------------//
	left_pos += 2 * RADIUS;     // leftside
	left_total_move += robot[1].GetX();
	robot[1].SetVisit(true);
	double original_left = robot[1].GetX();
	robot[1].SetX(original_left - left_total_move);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Robot old = Robot(left_total_move, robot[1].GetY(), true);
	Robot new_r = Robot(0, robot[1].GetY(), true);
	handler.circleMove(robot, N, left_pos, right_pos, old, new_r, 1);
	handler.drawpicture(robot, N, left_pos, right_pos, VIDEODPS);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	right_pos -= 2 * RADIUS;  // rightside
	right_total_move += LENGTH - 2 * RADIUS - robot[N].GetX();
	robot[N].SetVisit(true);
	double original_right = robot[N].GetX();
	robot[N].SetX(original_right + right_total_move);	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Robot old2 = Robot(right_pos - right_total_move, robot[N].GetY(), true);
	Robot new_r2 = Robot(right_pos, robot[N].GetY(), true);
	handler.circleMove(robot, N, left_pos, right_pos, old2, new_r2, N);
	handler.drawpicture(robot, N, left_pos, right_pos, VIDEODPS);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//----------------------------------------------------------------------------------------------------------------//

	//------
	while (left_pos < right_pos)
	{
		// temp value
		int stop_left = -1;                 // find the nearest unused robot of BOTHSIDE to fill to ith robot's gap
		int stop_right = -1;                // find the nearest unused robot of BOTHSIDE to fill to jth robot's gap
		double left_move = 0;               // record the move distance fo leftside  
		double right_move = 0;              // record the move distance fo righttside  
		double temp_left_pos = left_pos;    // temp store for left boundary position
		double temp_right_pos = right_pos;  // temp store for right boundary position

		/////-------------------------- leftside calculation----------------------------////		
		if (robot[i].GetVist() == false)                            // left side and right side haven't meet with each other
		{
			if (robot[i].GetX() - robot[i-1].GetX() <= 2 * RADIUS)    // two robot has overlapped part ,not move
			{
				temp_left_pos += robot[i].GetX() - robot[i-1].GetX();  // left boundary move right at less than 2*r distance
			}		
			else
			{                                                            // no overlapped part of two robots
				temp_left_pos += 2 * RADIUS;
				left_move += robot[i].GetX() - robot[i-1].GetX() - 2 * RADIUS;  // left boundary will move to right 2*r 
			}
		}
		else  // left side and right side meet with each other
		{
			// calculate safe distance
			double right_safe = 0; 
			if (i == N) // right hand side don't move ,stay in Nth position, and left side of Nth position have gap
			{
				cout << "too many robot at the leftside " << endl;
			}
			else if (i < N)
			{
				right_safe = 2 * RADIUS - (robot[i + 1].GetX() - robot[i].GetX());
			}
			double left_safe = 0;
			if (i == 2) // right hand side don't move ,stay in Nth position, and left side of Nth position have gap
			{
				cout << "too many robot at the rightside " << endl;
			}
			else if (i > 2)
			{
				left_safe = 2 * RADIUS - (robot[i - 1].GetX() - robot[i - 2].GetX());
			}
			if (right_safe < 0 || left_safe < 0)
			{
				cout << "error, the value of left_safe and right_safe must be not negative" << endl;
				cout << robot[i + 1].GetX() << " " << robot[i].GetX() << " " << robot[i - 1].GetX() << " " << robot[i - 2].GetX() << endl;
				printrobot(robot, N);
				system("pause");
				return 0;
			}
			if (right_safe + left_safe >= (robot[i].GetX() - robot[i-1].GetX() - 2 * RADIUS)) // finish
			{
				Robot old1 = robot[i - 1];
				Robot old2 = robot[i];
				int o = robot[i - 1].GetX();
				robot[i - 1].SetX( o + left_safe);
				robot[i].SetX(robot[i].GetX() - (robot[i].GetX() - o - 2 * RADIUS - left_safe));
				handler.circleMove(robot, N,left_pos, right_pos, old1, robot[i - 1], i - 1);
				handler.circleMove(robot, N,left_pos, right_pos, old2, robot[i], i);
				if (check(robot, N))
				{
					cout << "the total distance is " << (robot[i].GetX() - robot[i - 1].GetX() - 2 * RADIUS + right_total_move + left_total_move) << endl;					
				}
				else
				{
					cout << "faild" << endl;
				}
				if (handler.createavi())
				{
					cout << "create video success" << endl;
				}
				else
				{
					cout << "create video failed" << endl;
				}
				system("pause");
				return 0;
				
			}
			else
			{
				left_move = FindClosestNotUsed(N, robot, i-1, stop_left,true); // need to use other unused robot
				temp_left_pos += 2 * RADIUS;
			}
		}		
		if (left_move < 0)
		{
			cout << "left move error" << endl;
			system("pause");
			return 0;
		}

		///--------------------------------------------------------------------------------------------------///

		// --------------------------------rightside calculation-----------------------------------------//		
		if (robot[j].GetVist() == false)
		{
			if (robot[j+1].GetX() - robot[j].GetX() <= 2 * RADIUS)
			{
				temp_right_pos -= (robot[j+1].GetX() - robot[j].GetX());
			}
			else
			{
				temp_right_pos -= 2 * RADIUS;
				right_move += (robot[j+1].GetX() - robot[j].GetX() - 2 * RADIUS);
			}
		}
		else // stop condition
		{
			// calculate safe distance
			double left_safe = 0;
			if (j == 1) // right hand side don't move ,stay in Nth position, and left side of Nth position have gap
			{
				cout << "too many robot at the rightside " << endl;
			}
			else if (j > 1)
			{
				left_safe = 2 * RADIUS - (robot[j].GetX() - robot[j - 1].GetX());
			}
			double right_safe = 0;
			if (j == N - 1) // right hand side don't move ,stay in Nth position, and left side of Nth position have gap
			{
				cout << "too many robot at the leftside " << endl;
			}
			else if (j < N - 1)
			{
				right_safe = 2 * RADIUS - (robot[j + 2].GetX() - robot[j + 1].GetX());
			}
			if (right_safe < 0 || left_safe < 0)
			{
				cout << "error, the value of left_safe and right_safe must be not negative" << endl;
				cout << robot[j + 2].GetX() << " " << robot[j+1].GetX() << " " << robot[j].GetX() << " " << robot[j-1].GetX() << endl;
				printrobot(robot, N);
				system("pause");
				return 0;
			}
			if (right_safe + left_safe >= robot[j+1].GetX() - robot[j].GetX() - 2 * RADIUS)
			{
				/*Robot old1 = robot[j + 1];
				Robot old2 = robot[j - 1];
				int o = robot[j + 1].GetX();
				robot[j + 1].SetX( o- right_safe);
				robot[j - 1].SetX(robot[j - 1].GetX() + (o - robot[j].GetX() - 2 * RADIUS - right_safe));
				handler.circleMove(robot, N,  left_pos, right_pos, old1, robot[j + 1], j + 1);
				handler.circleMove(robot, N,  left_pos, right_pos, old2, robot[j - 1], j - 1);
				if (check(robot, N))
				{
					cout << "the total distance is " << (robot[j + 1].GetX() - robot[j].GetX() - 2 * RADIUS + right_total_move + left_total_move) << endl;
				}
				else
				{
					cout << "faild" << endl;
				}
				if (handler.createavi())
				{
					cout << "create video success" << endl;
				}
				else
				{
					cout << "create video failed" << endl;
				}
				system("pause");
				return 0;*/
			}
			else
			{
				right_move = FindClosestNotUsed(N, robot, j+1, stop_right,false);
				temp_right_pos -= 2 * RADIUS;
			}
		}		
		if (right_move < 0)
		{
			cout << "right move error" << endl;
			system("pause");
			return 0;
		}
		//------------------------------------------------------------------------//

		//------------------------compair the left_move and right_move-----------------------//
		if (left_move > right_move) // move right
		{
			int a = judgeStopPos(stop_left,stop_right,false);
			if (a == -1 || (a>0 && robot[a].GetVist() == true))
			{
				cout << "Error, stop right wrong" << endl;
				system("pause");
				return 0;
			}
			else if (a > 0 && robot[a].GetVist() == false)
			{
				Robot old = robot[a];
				if (update(j+1, a, robot, false))
				{
					if (j >= 2)
					{
						j--;
					}
				}
				handler.circleMove(robot, N, left_pos, right_pos, old, robot[j + 1], j + 1);
			}
			else if (a == 0)
			{
				robot[j].SetVisit(true);
				double original = robot[j].GetX();
				robot[j].SetX(original + right_move);
				if (right_move > 0)
				{
					Robot old = Robot(robot[j].GetX() - right_move, robot[j].GetY(), true);
					handler.circleMove(robot, N, left_pos, right_pos, old, robot[j], j);
				}
				CheckNotused(robot, j, false,N);	
				if (j >= 2)
				{
					j--; // move right pointer
				}
			}
			else
			{
				cout << "error" << endl;
				system("pause");
				return 0;
			}
			right_pos = temp_right_pos;
			right_total_move += right_move;
			handler.drawpicture(robot, N, left_pos, right_pos, VIDEODPS);
		}
		else
		{
			int a = judgeStopPos(stop_left, stop_right, true);
			if (a == -1 || ( a>0 && robot[a].GetVist() == true))
			{
				cout << "Error, stop left wrong" << endl;
				system("pause");
				return 0;
			}
			else if (a > 0 && robot[a].GetVist() == false)
			{
				Robot old = robot[a];
				if (update(i-1, a, robot, true))
				{
					if (i <= N - 1)
					{
						i++;
					}
				}
				handler.circleMove(robot, N, left_pos, right_pos, old, robot[i - 1], i - 1);
			}
			else if (a == 0)
			{				
				robot[i].SetVisit(true);
				double original = robot[i].GetX();
				robot[i].SetX(original - left_move);
				if (left_move > 0)
				{
					Robot old = Robot(robot[i].GetX() + left_move, robot[i].GetY(), true);
					handler.circleMove(robot, N, left_pos, right_pos, old, robot[i], i);
				}
				CheckNotused(robot,i, true,N);
				if (i <= N-1)
				{
					i++; // move left pointer
				}				
			}
			else
			{
				cout << "error" << endl;
				system("pause");
				return 0;
			}
			left_pos = temp_left_pos;
			left_total_move += left_move;
			handler.drawpicture(robot, N, left_pos, right_pos, VIDEODPS);
		}	
		//------------------------------------------------------------------------//
	}
	if (check(robot, N))
	{
		cout << "The Total Distance is " << left_total_move + right_total_move << endl;
	}
	else
	{
		cout << "fail" << endl;
	}	
	handler.drawpicture(robot, N, left_pos, right_pos, VIDEODPS);
	if (handler.createavi())
	{
		cout << "create video success" << endl;
	}
	else
	{
		cout << "create video failed" << endl;
	}
	system("pause");
	return 0;
}
void CheckNotused(Robot r[MAX_ROBOT_NUM], int i, bool left,int N)
{
	 if (left)  // leftside
	{
		 int look_back_distance = 2 * RADIUS;
		 if (r[i].GetX() < 2 * RADIUS)
		 {
			 look_back_distance = r[i].GetX();
		 }				 
		int c = i - 1;
		int count = 0;
		while (r[c].GetX() >= r[i].GetX() - look_back_distance)
		{
			count++;
			if (r[c].GetVist() == true)
			{
				r[c].SetVisit(false);
			}
			if (c <= 1)
			{
				c--;
				break;
			}
			c--;			
		}
		r[c + 1].SetVisit(true);			 		
     }
	 else   // rightside	 
	 {
		 int look_back_distance = 2 * RADIUS;
		 if (LENGTH - r[i].GetX()  < 4 * RADIUS)
		 {
			 look_back_distance = LENGTH - r[i].GetX();
		 }				 
		int c = i + 1;
		int count = 0;
		while (r[c].GetX() <= r[i].GetX() + look_back_distance)
		{
			count++;
			if (r[c].GetVist()== true)
			{
				r[c].SetVisit(false);
			}
			if (c >= N)
			{
				c++;
				break;
			}
			c++;
		}
		r[c - 1].SetVisit(true);						 			 
	 }		
	 return;
}
double FindClosestNotUsed(int N, Robot r[MAX_ROBOT_NUM], int i, int & stop,bool left)
{
	double l = LENGTH;
	double right = LENGTH;
	int c = i + 1;
	// search the right-hand-side not used robot
	while (r[c].GetVist() == true)
	{		
		if (c >= N)
		{
			break;
		}
		c++;
	}
	// search the left-hand-side not used robot
	int d = i - 1;
	while (r[d].GetVist() == true)
	{
		
		if (d <= 1)
		{
			break;
		}
		d--;
	}
	if (d <=1 && c >= N)
	{
		cout << "wrong" << endl;
		return -1;
	}
	if (d > 1)
	{   
		if (left)
		{
			l = r[i].GetX() - r[d].GetX() + 2 * RADIUS;
		}
		else
		{
			l = r[i].GetX() - r[d].GetX() - 2 * RADIUS;
		}
		
	}
	if (c < N)
	{
		if (left)
		{
			right = r[c].GetX() - r[i].GetX() - 2 * RADIUS;
		}
		else
		{
			right = r[c].GetX() - r[i].GetX() + 2 * RADIUS;
		}		
	}	
	if ((right < 0 || l < 0) || (l==LENGTH && right==LENGTH))
	{
		cout << "Error" << endl;
		return -1;
	}
	stop = (right < l) ? c : d;
	return right<l ? right : l;
}
bool update(int nowi, int updatai, Robot r[MAX_ROBOT_NUM], bool left)
{
	int y = r[updatai].GetY();
	if (left)
	{
		if (nowi < updatai) // move right unused robot to nowi position, update the right hand side part
		{
			int d = r[nowi].GetX();
			for (int i = updatai; i >= nowi + 2; i--)
			{
				r[i] = r[i - 1];
			}
			r[nowi + 1].SetX(d + 2 * RADIUS);
			r[nowi + 1].SetY(y);
			r[nowi + 1].SetVisit(true);
			return true;
		}
		else if (nowi > updatai)// move left unused robot to nowi position, update the left hand side part
		{
			int d = r[nowi].GetX();
			for (int i = updatai; i <= nowi - 1; i++)
			{
				r[i] = r[i + 1];
			}
			r[nowi].SetX(d + 2 * RADIUS);
			r[nowi].SetY(y);
			r[nowi].SetVisit(true);
			return false;
		}
		else
		{
			cout << "wrong" << endl;
			return false;
		}
	}
	else
	{
		if (nowi < updatai) // move right unused robot to nowi position, update the right hand side part
		{
			int d = r[nowi].GetX();
			for (int i = updatai; i >= nowi + 1; i--)
			{
				r[i] = r[i - 1];
			}
			r[nowi].SetX(d - 2 * RADIUS);
			r[nowi].SetY(y);
			r[nowi].SetVisit(true);
			return false;
		}
		else if (nowi > updatai)// move left unused robot to nowi position, update the left hand side part
		{
			int d = r[nowi].GetX();
			for (int i = updatai; i <= nowi - 2; i++)
			{
				r[i] = r[i + 1];
			}
			r[nowi - 1].SetX(d - 2 * RADIUS);
			r[nowi - 1].SetY(y);
			r[nowi - 1].SetVisit(true);
			return true;
		}
		else
		{
			cout << "wrong" << endl;
			return false;
		}
	}

}
int judgeStopPos(int left_p,int right_p, bool left)
{
	if (left_p == -1 && right_p == -1)
	{
		return 0;
	}
	else if ((left_p == -1 && right_p > 0) || (right_p == -1 && left_p > 0))
	{
		return -1;
	}
	else // stop[0]>0 && stop[1]>0 
	{
		return left ? left_p : right_p;
	}
}
void printrobot(Robot r[MAX_ROBOT_NUM], int N)
{
	for (int i = 1; i <= N; i++)
	{
		cout << i << " " << r[i].GetX() << endl;
	}
}
bool check(Robot r[MAX_ROBOT_NUM], int N)
{
	if (r[1].GetX() != 0 || r[N].GetX() != LENGTH - 2 * RADIUS)
	{
		return false;
	}
	for (int i = 2; i <= N; i++)
	{
		if (r[i - 1].GetX() + 2 * RADIUS < r[i].GetX())
		{
			cout << r[i - 1].GetX() << " " << r[i].GetX() << endl;
			printrobot(r, N);
			return false;
		}
	}
	return true;
}
bool initialize(Robot robot[MAX_ROBOT_NUM], int & N)
{
	int N_min = ceill(LENGTH*ROBOT_MIN_RATIO / (2 * RADIUS));
	int N_max = ceill(LENGTH*ROBOT_MAX_RATIO / (2 * RADIUS));
	string min_str = "";
	string max_str = "";
	stringstream ss;
	ss << N_max;
	ss >> max_str;
	ss.clear();
	ss << N_min;
	ss >> min_str;
	cout << "the number of robot is in the range of ( " + min_str + "," + max_str + " ), please input the exact number of robot" << endl;
	cin >> N;
	if (N<N_min || N>N_max)
	{
		cout << "wrong N" << endl;
		return false;
	}
	srand(time(NULL));
	for (int n = 1; n <= N; n++)
	{
		int a = 0 + rand() % (LENGTH - 2 * RADIUS); // we treate a as the most leftside of the circle, the range is [0,L-2r]
		int b = LOWER_Y + rand() % (UPPER_Y - LOWER_Y - RADIUS);
		robot[n].SetX(a);
		robot[n].SetY(b);
	}
	sort(robot, robot + (N + 1));  // sort O(NlogN)
	return true;
}
