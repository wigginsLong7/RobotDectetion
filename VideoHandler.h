#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>
#include"Robot.h"
#include<string>

#ifndef VIDEOCREATE
#define VIDEOCREATE

#define  UPPER_Y                            400 
#define  LOWER_Y                            100 
#define  IMAGE_LENGTH                       1000
#define  VIDEODPS                           7
class VideoHandler
{
public:
	VideoHandler();
	~VideoHandler();
	bool circleMove(Robot[MAX_ROBOT_NUM], int,int,int, Robot, Robot, int);
	bool createavi();
	bool drawpicture(Robot[MAX_ROBOT_NUM], int, int, int, int);
	bool ShowPicture(int);
	int GetImgaeIndex();
	void SetImageIndex(int);
	std::string GetVideoPath();
	void SetVideoPath(std::string);
	std::string GetImagePath();
	void SetImagePath(std::string);

private:
	IplImage *image[IMAGE_LENGTH];
	int image_index;
	std::string videopath;
	std::string imagepath;
};
#endif
