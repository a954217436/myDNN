#ifndef __MY_MASK__
#define __MY_MASK__

#include "detector.h"
#include "centerface.h"


class Mask: public Detector
{
public:
    Mask(){};

    Mask(const cv::String& classifierPath,
         const cv::String& centerfacePath,
         const float confThres = 0.5,
         const float faceThres = 0.5,
         const int width = 640,
         const int height = 480);

    bool detect(const cv::String &image_path="/home/robot/ocr.jpg");

private:
    bool classify(cv::Mat& box);
    void classify_boxes(std::vector<FaceInfo> &faces);

private:
    Centerface centerface;
    float faceThreshold; //centerface Threshold
};


#endif  //__MY_MASK__