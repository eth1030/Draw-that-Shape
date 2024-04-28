#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace cv;
void detectAndDisplay( Mat frame );

cv::CascadeClassifier triangle_cascade;
CascadeClassifier rectangle_cascade;
CascadeClassifier circle_cascade;

int main( int argc, const char** argv )
{

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
while ( capture.read(frame) )
	{
		if( frame.empty() )
		{
			cout << "no captured frame - break\n";
			break;
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

		// display
		for ( size_t i = 0; i < circles.size(); i++ ) {
			Point center( circles[i].x + circles[i].width/2, circles[i].y + circles[i].height/2 );
			ellipse( frame, center, Size( circles[i].width/2, circles[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
		}
		// output what was detected
		cout << "Triangles: " << triangles.size() << "\n";
		cout << "Circles: " << circles.size() << "\n";
		cout << "Rectangles: " << rectangles.size() << "\n";
		
	    imshow( "circle detection", frame );
		if( waitKey(10) == 27 )
		 {
		 break; // escape
		 }
		
	}

return 0;

}
