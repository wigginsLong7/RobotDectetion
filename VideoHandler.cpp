#include"VideoHandler.h"

VideoHandler::VideoHandler()
{
	CvSize sz;//Í¼Ïñ³ß´ç
	sz.height = 500;
	sz.width = 500;
	IplImage *image[IMAGE_LENGTH];
	for (int m = 0; m < IMAGE_LENGTH; m++)
	{
		this->image[m] = cvCreateImage(sz, IPL_DEPTH_8U, 0);
	}
	this->image_index = 0;
	this->videopath = "C:\\Users\\peter\\Desktop\\Algorithm\\3.avi";
	this->imagepath = "C:\\Users\\peter\\Desktop\\Algorithm\\1.jpg";
}

VideoHandler::~VideoHandler()
{

}
bool VideoHandler::circleMove(Robot r[MAX_ROBOT_NUM], int n, int left_line, int right_line, Robot old, Robot new_r, int r_index)
{
	int old_x = old.GetX();
	int new_x = new_r.GetX();
	int dup_times = abs(old_x - new_x) + 1;
	if (image_index >= IMAGE_LENGTH)
	{
		return false;
	}
	for (int j = 0; j < dup_times; j++)
	{
		for (int i = 1; i <= n; i++)
		{
			if (image_index >= IMAGE_LENGTH)
			{
				return false;
			}
			CvPoint centerpoint;
			centerpoint.x = int(r[i].GetX() + RADIUS);
			centerpoint.y = int(r[i].GetY());
			if (i == r_index)
			{
				if (new_x>old_x)
				{
					centerpoint.x = int(old_x + j + RADIUS);
					centerpoint.y = int(old.GetY());
				}
				else
				{
					centerpoint.x = int(old_x - j + RADIUS);
					centerpoint.y = int(old.GetY());
				}
			}
			if (r[i].GetVist() == true)
			{
				cvCircle(this->image[this->image_index], centerpoint, RADIUS, CV_RGB(0, 255, 0), 1, 8, 3);
			}
			else
			{
				cvCircle(this->image[this->image_index], centerpoint, RADIUS, CV_RGB(0, 0, 255), 1, 8, 3);
			}
		}
		cvLine(this->image[this->image_index], cv::Point(0, UPPER_Y), cv::Point(LENGTH, UPPER_Y), CV_RGB(255, 0, 0));
		cvLine(this->image[this->image_index], cv::Point(left_line, LOWER_Y), cv::Point(left_line, UPPER_Y), CV_RGB(255, 0, 0));
		cvLine(this->image[this->image_index], cv::Point(right_line, LOWER_Y), cv::Point(right_line, UPPER_Y), CV_RGB(255, 0, 0));
		this->image_index++;
	}
	return true;
}
bool VideoHandler::createavi()
{ 
	if (this->image_index >= IMAGE_LENGTH)
	{
		return false;
	}
	CvSize size = cvSize(500, 500);
	CvVideoWriter *writer = cvCreateVideoWriter(this->videopath.c_str(), -1, VIDEODPS, size); //´´½¨ÊÓÆµ
	for (int i = 0; i < this->image_index; i++)
	{
		if (cvWriteFrame(writer, image[i]) == 0)
		{
			return false;
		}
	}
	cvReleaseVideoWriter(&writer);
	return true;
}
bool  VideoHandler::drawpicture(Robot r[MAX_ROBOT_NUM], int n, int left_line, int right_line, int dup_times)
{
	if (image_index >= IMAGE_LENGTH)
	{
		return false;
	}
	for (int j = 0; j < dup_times; j++)
	{
		for (int i = 1; i <= n; i++)
		{
			if (image_index >=IMAGE_LENGTH)
			{
				return false;
			}
			CvPoint centerpoint;
			centerpoint.x = int(r[i].GetX() + RADIUS);
			centerpoint.y = int(r[i].GetY());
			if (r[i].GetVist() == true)
			{
				cvCircle(this->image[this->image_index], centerpoint, RADIUS, CV_RGB(0, 255, 0), 1, 8, 3);
			}
			else
			{
				cvCircle(this->image[this->image_index], centerpoint, RADIUS, CV_RGB(0, 0, 255), 1, 8, 3);
			}
		}
		cvLine(this->image[this->image_index], cv::Point(0, UPPER_Y), cv::Point(LENGTH, UPPER_Y), CV_RGB(255, 0, 0));
		cvLine(this->image[this->image_index], cv::Point(left_line, LOWER_Y), cv::Point(left_line, UPPER_Y), CV_RGB(255, 0, 0));
		cvLine(this->image[this->image_index], cv::Point(right_line, LOWER_Y), cv::Point(right_line, UPPER_Y), CV_RGB(255, 0, 0));
		this->image_index++;
		
	}
	return true;
}
int VideoHandler::GetImgaeIndex()
{
	return this->image_index;
}
void VideoHandler::SetImageIndex(int a)
{
	this->image_index = a;
}
std::string VideoHandler::GetVideoPath()
{
	return this->videopath;
}
void VideoHandler::SetVideoPath(std::string path)
{
	this->videopath = path;
}
std::string VideoHandler::GetImagePath()
{
	return this->imagepath;
}
void VideoHandler::SetImagePath(std::string path)
{
	this->imagepath = path;
}
bool VideoHandler::ShowPicture(int index)
{
	if (index >= this->image_index)
	{
		return false;
	}
	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	cvShowImage("image", this->image[index]);
	cvWaitKey(100);
	return true;
}

