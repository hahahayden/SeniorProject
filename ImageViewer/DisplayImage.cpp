/*  CPE442 Lab 2
*   Display an Image using OpenCV.
*   Group Members: Aaron Teh, Lucas Chang
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "Invalid file name";
        return -1;
    }
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    if (image.empty())
    {
        cout << "Image could not be loaded";
        return -1;
    }

    imshow("Image", image);


    waitKey(0);

    return 0;
}