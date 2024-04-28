#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
using namespace std;
using namespace cv;

#define BUF_SIZE 256
#define CIRCLE 0
#define TRIANGLE 1
#define RECTANGLE 2
void detectAndDisplay( Mat frame );

cv::CascadeClassifier triangle_cascade;
CascadeClassifier rectangle_cascade;
CascadeClassifier circle_cascade;
char buf[BUF_SIZE];

int main( int argc, const char** argv )
{
	// open char dev for write
	fd = open("/dev/mytimer", O_RDWR| O_TRUNC);
    if (fd == -1) {
		char *print = "character device not loaded\n";
		write(STDERR_FILENO, print, strlen(print));
        return -1;
    }

// load classifiers
if( !triangle_cascade.load("../training_data/triangle_cascade/cascade.xml") )
	{
		cout << "error loading triangle cascade\n";
		return -1;
	};
if( !rectangle_cascade.load("../training_data/rectangle_cascade/cascade.xml") )
	{
		cout << "error loading rectangle cascade\n";
		return -1;
	};
if( !circle_cascade.load("../training_data/circle_cascade/cascade.xml") )
	{
		cout << "error loading circle cascade\n";
		return -1;
	};

// read video stream
VideoCapture capture;
capture.open(0);

if ( ! capture.isOpened() )
	{
		cout << "error opening video capture\n";
		return -1;
	}

Mat frame;
while (1)
	{
		if (!read(fd, buf, BUF_SIZE)) continue;
		if( frame.empty() )
		{
			cout << "no captured frame\n";
			continue;
	    }
		// code to detect and display
		Mat frame_gray;
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		// detect shapes
		std::vector<Rect> triangles;
		triangle_cascade.detectMultiScale(frame_gray, triangles);
		std::vector<Rect> circles;
		circle_cascade.detectMultiScale(frame_gray, circles);
		std::vector<Rect> rectangles;
		rectangle_cascade.detectMultiScale(frame_gray, rectangles);

		// // display circles
		// for ( size_t i = 0; i < circles.size(); i++ ) {
		// 	Point center( circles[i].x + circles[i].width/2, circles[i].y + circles[i].height/2 );
		// 	ellipse( frame, center, Size( circles[i].width/2, circles[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 0 ), 4 );
		// }
		// // display triangles
		// for ( size_t i = 0; i < triangles.size(); i++ ) {
		// 	Point center( triangles[i].x + triangles[i].width/2, triangles[i].y + triangles[i].height/2 );
		// 	ellipse( frame, center, Size( triangles[i].width/2, triangles[i].height/2 ), 0, 0, 360, Scalar( 0, 0, 255 ), 4 );
		// }
		// // display rectangles
		// for ( size_t i = 0; i < rectangles.size(); i++ ) {
		// 	Point center( rectangles[i].x + rectangles[i].width/2, rectangles[i].y + rectangles[i].height/2 );
		// 	ellipse( frame, center, Size( rectangles[i].width/2, rectangles[i].height/2 ), 0, 0, 360, Scalar( 0, 255, 0 ), 4 );
		// }
		// output what was detected
		cout << "Triangles: " << triangles.size() << "\n";
		cout << "Circles: " << circles.size() << "\n";
		cout << "Rectangles: " << rectangles.size() << "\n";

		int count = 0;		
		int ret;
		int max_num = max(triangles.size(), circles.size());
		max_num = max(max_num, rectangles,size());
		// write to char dev
		memset(buf, 0, BUF_SIZE);
		if (circle.size() == max_num) {
			count++;
			ret =  CIRCLE;
		}
		if (triangles.size() == max_num) {
			count++;
			ret =  TRIANGLE;
		}
		if (rectangles.size() == max_num) {
			count++;
			ret =  RECTANGLE;
		}
		if (count > 1) {
			ret = 5;
		}
		int len = sprintf(buf, "%d", ret);
		write(fd, buf, len);

	}

return 0;

}
