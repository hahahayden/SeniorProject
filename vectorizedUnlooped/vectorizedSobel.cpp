#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <pthread.h>
#include <stdio.h>
#include <cstdio>
#include <ctime>

#include "sobel.hpp"

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
    VideoCapture cap;
    clock_t startTime = clock();
    frameThread th;
    uint64_t cpu_cycles[4] = {0,0,0,0};
    uint64_t cache_misses[4] = {0,0,0,0};
    clock_t frameTime[4] = {0,0,0,0};
    int frames = 0;
    

    if (!cap.open(0))
    {
        cout << "Can't Open Video File" << endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);

    //init threads
    // pthread_attr_init(&attr);
    // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //main loop
    while(1)
    {
        Mat frame;
        cap >> frame;
        frames += 1;
        if (frame.empty())
        {
            // cout << "Image is empty" << endl;
            break;
        }

        frame = multiThreadSobel(frame, cache_misses, cpu_cycles, frameTime);

        imshow("Vectorized Sobel",frame);

        //Waitkey value changes depending on framerate and delay due to resolution
        if (waitKey(1) == 27)
            break;

    } 
    clock_t endTime = clock();
    clock_t clockTicksTaken = endTime - startTime;
    clock_t clockTicksPerFrame = 0;
    double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;   
    cout << timeInSeconds << endl;
    cout << "Frames: " << frames << endl << endl;
    for (int i = 0; i < 4; i++)
    {
        float avg_cycles = cpu_cycles[i]/frames;
        float avg_misses = cache_misses[i]/frames;
        cout << "CPU Cycles " << i << ": " << cpu_cycles[i] << endl;
        cout << "Cache Misses " << i << ": " << cache_misses[i] << endl;
        cout << "Avg CPU Cycles per Frame " << i << ": " << avg_cycles << endl;
        cout << "Avg Cache Misses per Frame " << i << ": " << avg_misses << endl << endl;
        clockTicksPerFrame += frameTime[i];
    }
    double processingTime = clockTicksPerFrame / ((double) CLOCKS_PER_SEC * 4);
    float avg_frameTime = processingTime/frames;
    float fps = frames/processingTime;
    cout << endl << "Avg Time per Frame: " << avg_frameTime << endl;
    cout << "Avg Processing Time per Thread: " << processingTime << endl;
    cout << "Avg FPS: " << fps << endl;

    return 0;
}
