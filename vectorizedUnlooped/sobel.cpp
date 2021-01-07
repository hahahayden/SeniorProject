#include "sobel.hpp"
#include <stdio.h>
#define NUM_THREADS 4
using namespace std;
using namespace cv;


void to442_sobel(Mat frame, Mat sobelFrame, int tid)
{
    /*Gx={  {-1, 0, 1},
            {-2, 0, 2},
            (-1, 0, 1)};
    Gy = {  {1, 2, 1},
            {0, 0, 0},
            {-1, -2, -1}};

    G = |Gx| + |Gy|
    */
   
    /*  Splitting Frames
    *   TR = (0,0) -> (cols/2 + 1,rows/2 + 1)
    *   TL = (cols/2 - 1, 0) -> (cols, rows/2 + 1)
    *   BR = (0, rows/2 - 1) -> (cols/2 + 1, rows)
    *   BL = (cols/2 - 1, rows/2 - 1) -> (cols, rows)
    */

    /*  Vectorizing Notes
    *   Each pixel is 8bits; 128 bit data allows us for 16 data sets per vector
    *   G = |Gx| + |Gy|
    */  
    int rowmin, colmin, rowmax, colmax;
    rowmin = 0;
    rowmax = frame.rows;
    colmin = 0;
    colmax = frame.cols;
    uchar sobelValue; 
    int16_t tr, mr, br, tm, /*mm,*/ bm, tl, ml, bl;
    int16_t Gx, Gy, G;

    int16_t Matx[] = {-1, 0, 1, -2, 2, -1, 0, 1};   //Gx
    int16_t Maty[] = {1, 2, 1, 0, 0, -1, -2, -1};  //Gy

    switch(tid)
    {
        case 0: // top left (0, 0), (x/2, y/2)
            rowmin = 0;
            rowmax = frame.rows/2 + 1;
            colmin = 0;
            colmax = frame.cols/2 + 1;
            break;
        case 1: // bottom left (0, y/2), (x/2, y)
            rowmin = frame.rows/2 - 2;
            rowmax = frame.rows - 1;
            colmin = 0;
            colmax = frame.cols/2 + 1;
            break;
        case 2: // top right (x/2, 0), (x, y/2)
            rowmin = 0;
            rowmax = frame.rows/2 + 1;
            colmin = frame.cols/2 - 2;
            colmax = frame.cols - 1;
            break;
        case 3: // bottom right(x/2, y/2), (x, y)
            rowmin = frame.rows/2 - 2;
            rowmax = frame.rows - 1;
            colmin = frame.cols/2 - 2;
            colmax = frame.cols - 1;
            break;
        default:
            rowmin = 0;
            rowmax = frame.rows;
            colmin = 0;
            colmax = frame.cols;
            break;
    }
 
    for (int rows = rowmin + 1; rows < rowmax; rows++)
    {
        uchar* topRow = frame.ptr<uchar>(rows-1);
        uchar* middleRow = frame.ptr<uchar>(rows);
        uchar* bottomRow = frame.ptr<uchar>(rows+1);
        uchar* sobelRow = sobelFrame.ptr<uchar>(rows-1);
        for (int cols = colmin + 1; cols < colmax; cols++)
        {
            Gx = Gy = 0;

            //Vec3b* colorFrame = frame.ptr<Vec3b>(rows);

            // cout << "Thread: " << tid << endl;
            tr = topRow[cols - 1]; //11
            mr = middleRow[cols - 1];//21
            br = bottomRow[cols - 1]; //31
            tm = topRow[cols]; //12
            // mm = frame.at<Vec3b>(Point(cols,    rows))   .val[0];  //22
            bm = bottomRow[cols]; //32
            tl = topRow[cols + 1]; //13
            ml = middleRow[cols + 1]; //23
            bl = bottomRow[cols + 1]; //33

            int16_t pixels[] = {tr, mr, br, tm, bm, tl, ml, bl};

            //prefetch array
            __builtin_prefetch(pixels);
            
            int16x8_t frame_vector = vld1q_s16(pixels);
            int16x8_t Matx_vector = vld1q_s16(Matx);
            int16x8_t Maty_vector = vld1q_s16(Maty);

            
            int16x8_t Gx_vector = vmulq_s16(frame_vector, Matx_vector);
            Gx += vgetq_lane_s16(Gx_vector,0);
            Gx += vgetq_lane_s16(Gx_vector,1);
            Gx += vgetq_lane_s16(Gx_vector,2);
            Gx += vgetq_lane_s16(Gx_vector,3);
            Gx += vgetq_lane_s16(Gx_vector,4);
            Gx += vgetq_lane_s16(Gx_vector,5);
            Gx += vgetq_lane_s16(Gx_vector,6);
            Gx += vgetq_lane_s16(Gx_vector,7);


            int16x8_t Gy_vector = vmulq_s16(frame_vector, Maty_vector);
            Gy += vgetq_lane_s16(Gy_vector,0);
            Gy += vgetq_lane_s16(Gy_vector,1);
            Gy += vgetq_lane_s16(Gy_vector,2);
            Gy += vgetq_lane_s16(Gy_vector,3);
            Gy += vgetq_lane_s16(Gy_vector,4);
            Gy += vgetq_lane_s16(Gy_vector,5);
            Gy += vgetq_lane_s16(Gy_vector,6);
            Gy += vgetq_lane_s16(Gy_vector,7);


            G = abs(Gx) + abs(Gy);

            if (G > 255)
                G = 255;
            sobelValue = G;
            sobelRow[cols-1] = sobelValue;
        }
    }
}

void to442_grayscale(Mat frame, Mat grayFrame, int tid)
{
    int rowmin, colmin, rowmax, colmax;

    uchar gray,r,g,b;
    float grayVector[4];
    float grayValue[] = {.0722, .2126, .7152};

    switch(tid)
    {
        case 0: // top left (0, 0), (x/2, y/2)
            rowmin = 0;
            rowmax = frame.rows/2;
            colmin = 0;
            colmax = frame.cols/2;
            break;
        case 1: // bottom right (0, y/2), (x/2, y)
            rowmin = frame.rows/2;
            rowmax = frame.rows;
            colmin = 0;
            colmax = frame.cols/2;
            break;
        case 2: // top right (x/2, 0), (x, y/2)
            rowmin = 0;
            rowmax = frame.rows/2;
            colmin = frame.cols/2;
            colmax = frame.cols;
            break;
        case 3: // bottom right(x/2, y/2), (x, y)
            rowmin = frame.rows/2;
            rowmax = frame.rows;
            colmin = frame.cols/2;
            colmax = frame.cols;
            break;
        default:
            rowmin = 0;
            rowmax = frame.rows;
            colmin = 0;
            colmax = frame.cols;
            break;
    }
    for (int rows = rowmin; rows < rowmax; rows++)
    {
        Vec3b* colorFrame = frame.ptr<Vec3b>(rows);
        uchar* grayRow = grayFrame.ptr<uchar>(rows);
        for (int cols = colmin; cols < colmax; cols++)
        {
            //Vec3b in form BRG
            gray = 0;

            //1 Channel Grayscale
            // float gray = .0722*colorFrame[0] + .2126*colorFrame[1] + .7152*colorFrame[2];
            
            // Vectorized Grayscale
            // /* 
            b = colorFrame[cols][0];
            r = colorFrame[cols][1];
            g = colorFrame[cols][2];
            float colors[] = {(float)b,(float)r,(float)g};
            __builtin_prefetch(colors);

            float32x4_t colors_vector = vld1q_f32(colors);
            float32x4_t gray_value = vld1q_f32(grayValue);
            float32x4_t grayscale = vmulq_f32(colors_vector, gray_value);

            vst1q_f32(grayVector, grayscale);

            for (int i = 0; i < 3; i++)
            {
                gray += grayVector[i];
            }
            // */

            grayRow[cols] = gray;
        }
    }
}

void* sobelThread(void *sobelArg)
{
    struct frameThread *sobel;
    uint64_t CACHE_MISSES, CPU_CYCLES;
    sobel = (struct frameThread *) sobelArg;
    clock_t startTime = clock();
    // cout << "Thread ID:" << sobel->thread_id << endl;

    struct libperf_data *pd = libperf_initialize(-1, -1);
    libperf_enablecounter(pd, LIBPERF_COUNT_HW_CPU_CYCLES);
    libperf_enablecounter(pd, LIBPERF_COUNT_HW_CACHE_MISSES);

    //Grayscale
    to442_grayscale(sobel->frame, sobel->grayFrame, sobel->thread_id);
    //Sobel Filter
    to442_sobel(sobel->grayFrame, sobel->sobelImage, sobel->thread_id);

    clock_t endTime = clock();
    
    CPU_CYCLES = libperf_readcounter(pd, LIBPERF_COUNT_HW_CPU_CYCLES);
    CACHE_MISSES = libperf_readcounter(pd, LIBPERF_COUNT_HW_CACHE_MISSES);

    libperf_disablecounter(pd, LIBPERF_COUNT_HW_CPU_CYCLES);
    libperf_disablecounter(pd, LIBPERF_COUNT_HW_CACHE_MISSES);

    libperf_close(pd);

    sobel->cache_misses += CACHE_MISSES;
    sobel->cpu_cycles += CPU_CYCLES;
    sobel->time += (endTime - startTime);


    pthread_exit(NULL);
}

Mat multiThreadSobel(Mat frame, uint64_t* cache_misses, uint64_t* cpu_cycles, clock_t* time)
{
    int rc;
    
    struct frameThread th[NUM_THREADS];
    void *status;

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    
    Mat frames[NUM_THREADS];
    Mat grayframe(frame.rows, frame.cols, CV_8UC1);
    Mat vert1, vert2;
    Mat sobelImage(frame.rows - 2, frame.cols - 2, CV_8UC1);
    pthread_attr_init(&attr);
    

    for (int i = 0; i < NUM_THREADS; i++)
    {
        // cout << "Creating Thread: " << i << endl;
        th[i].thread_id = i;
        th[i].frame = frame;
        th[i].grayFrame = grayframe;
        th[i].sobelImage = sobelImage;
        th[i].cache_misses = 0;
        th[i].cpu_cycles = 0;
        th[i].time = 0;
        rc = pthread_create(&threads[i], NULL, sobelThread, &th[i]);
        if (rc)
        {
            cout << "Unable to create thread " << rc << endl;
            exit(-1);
        }
        
    }
    
    for (int i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_join(threads[i],&status);
        frames[i] = th[i].sobelImage;
        cache_misses[i] += th[i].cache_misses;
        cpu_cycles[i] += th[i].cpu_cycles;
        time[i] += th[i].time;
        if (rc)
        {
            cout << "Unable to join threads" << rc << endl;
            exit(-1);
        }
    }
    frame = frames[0];

    return frame;
}