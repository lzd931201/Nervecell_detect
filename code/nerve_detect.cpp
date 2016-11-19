
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<iostream>
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////
void VideoGetFrame();
cv::VideoCapture v1(0);
cv::Mat frame1,frame2,Grayframe1, Grayframe2,Blurfram,Cannyframe;

//////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	
	string answer;
	
	VideoGetFrame();
	cout << "another frame?(Y/N)" << endl;
	cin >> answer;
	while (answer == "Y" || answer == "y")
	{
		cv::destroyWindow("Videoframe");
		VideoGetFrame();
		cout << "another frame?(Y/N)" << endl;
		cin >> answer;
	}
	cv::Mat centerPart1 = frame1(cv::Range(90, 585), cv::Range(265, 690));		//get the center part of the frame
	cv::cvtColor(centerPart1, Grayframe1, CV_BGR2GRAY);	//get the grayscale image
	cv::Mat centerPart2 = frame2(cv::Range(90, 585), cv::Range(265, 690));
	cv::cvtColor(centerPart2, Grayframe2, CV_BGR2GRAY);	//get the grayscale image
	

	/*cv::xfeatures2d::SiftFeatureDetector detector;
	std::vector<cv::KeyPoint> keypoints;
	detector.detect(Grayframe, keypoints);

	cv::Mat output;
	cv::drawKeypoints(Grayframe, keypoints, output);*/


	cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
	//cv::Ptr<Feature2D> f2d = xfeatures2d::SURF::create();
	//cv::Ptr<Feature2D> f2d = ORB::create();


	//-- Step 1: Detect the keypoints:
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	f2d->detect(Grayframe1, keypoints_1);
	f2d->detect(Grayframe2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)    
	cv::Mat descriptors_1, descriptors_2;
	f2d->compute(Grayframe1, keypoints_1, descriptors_1);
	f2d->compute(Grayframe2, keypoints_2, descriptors_2);

	//-- Step 3: Matching descriptor vectors using BFMatcher :
	cv::Mat output;
	cv::Mat frame_keypoint;
	vector<cv::DMatch> matches;
	cv::BFMatcher matcher;
	matcher.match(descriptors_1, descriptors_2, matches);
	cv::drawMatches(Grayframe1,keypoints_1, Grayframe2,keypoints_2,matches,output);
	cv::drawKeypoints(Grayframe1, keypoints_1, frame_keypoint);

	//cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();								//can't see point
	//std::vector<cv::KeyPoint> keypoints;
	//f2d->detect(Grayframe, keypoints);															
	//std::vector<cv::KeyPoint> keypoints1(keypoints.begin() , keypoints.begin() + 100);
	//cv::Mat output;
	//cv::drawKeypoints(Grayframe, keypoints1, output );
	//
	
	
	//cv::imshow("Grayframe", Grayframe);
	//cvWaitKey(0);
	cv::imshow("output", output);
	cv::imshow("frame1&point", frame_keypoint);
	cvWaitKey(0);
	v1.release();
	return(0);
}

void VideoGetFrame()
{
	//string VideoName;
	//cout << "which video?" << endl;
	//cin >> VideoName;			//use when there is more than one video
	int  frameId;
	cout << "which frame?" << endl;
	cin >> frameId;						//frame's ID

	v1.open("Video1.avi");
	if (!v1.isOpened())  // check if we succeeded
	{
		cout << "video not loaded!!" << endl;
		system("pause");
		
	}


	double rate = v1.get(CV_CAP_PROP_FPS);
	int delay = 1 / rate;
	//while (true)
	//{

		
		v1.set(CV_CAP_PROP_POS_FRAMES, frameId);
		bool success = v1.read(frame1);
		v1.set(CV_CAP_PROP_POS_FRAMES, frameId+2);
		v1.read(frame2);
		if (!success) {
			cout << "Cannot read  frame " << endl;

		}
		//cv::imwrite("image1.jpg", frame);
		//imshow("Videoframe", frame);
		
		//if (cv::waitKey(delay) >= 0)
		//{
		//}
		//	break;

	//}
	//system("pause");
	//v1.release();
}									//get a specific frame from a video