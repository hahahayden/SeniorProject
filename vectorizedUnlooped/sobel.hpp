#ifndef SOBEL_H_
#define SOBEL_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <pthread.h>
#include <arm_neon.h>
#include "libperf.h"
#include <ctime>

using namespace cv;

struct frameThread{
    Mat frame;
    Mat grayFrame;
    Mat sobelImage;
    int thread_id;
    uint64_t cache_misses;
    uint64_t cpu_cycles;
    clock_t time;
};


void to442_sobel(Mat frame, Mat sobelFrame, int tid);
void to442_grayscale(Mat frame, Mat grayFrame, int tid);
void *sobelThread(void *sobelArg); 
Mat multiThreadSobel(Mat frame, uint64_t* cache_misses, uint64_t* cpu_cycles, clock_t* time);

#endif