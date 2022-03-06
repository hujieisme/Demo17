#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

class ImageProcessing
{
public:
    ImageProcessing();
    void ImageCrop();
    void ImageRotate();
    void ImageSort();
};

#endif // IMAGEPROCESSING_H
