#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <pthread.h>
#include <stdio.h>
#include <cstdio>


using namespace std;
using namespace cv;


int main(int argc, char **argv)
{
    //Grab file
    // if (argc < 2)
    // {
    //     cout << "Invalid file name" << endl;
    //     return -1;
    // }
    
    clock_t startTime = clock();
    // if (!cap.isOpened())
    // {
    //     cout << "Can't Open Video File" << endl;
    //     return -1;
    // }

    //init threads
    // pthread_attr_init(&attr);
    // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //main loop
    while(1)
    {
        system("fswebcam -r 1920x1080 --no-banner 'image.jpg'");
        Mat frame = imread("image.jpg", CV_LOAD_IMAGE_UNCHANGED);

        if (frame.empty())
        {
            // cout << "Image is empty" << endl;
            break;
        }


        imshow("SobelVideo",frame);

        //Waitkey value changes depending on framerate and delay due to resolution
        if (waitKey(1) == 27)
            break;

    }    
    clock_t endTime = clock();
    clock_t clockTicksTaken = endTime - startTime;
    double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;   
    cout << timeInSeconds << endl;

    return 0;
}
