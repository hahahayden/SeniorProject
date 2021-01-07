#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    VideoCapture cap;
    if (!cap.open(0))
    {
        cout << "Can't Open Video File" << endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);
    
    while(1)
    {
        Mat frame;
        cap >> frame;
        if (frame.empty())
        {
            break;
        }
        
        imshow("Webcam", frame);
        if (waitKey(1) == 27)
            break;
    }

    return 0;
}