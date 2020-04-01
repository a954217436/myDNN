#ifndef __MY_YOLO__
#define __MY_YOLO__

#include "detector.h"


class Yolo: public Detector
{
public:
    Yolo(){};

    Yolo(const cv::String& modelConfigPath, 
         const cv::String& weightsPath,
         const cv::String& labelPath,
         const float confThres = 0.5,
         const float nmsThres = 0.4,
         const int width = 320,
         const int height = 320);

    bool detect(const cv::String &image_path="/home/robot/ocr.jpg");

private:
    std::vector<cv::String> getOutputsNames();
    void postprocess(std::vector<cv::Mat>& outs);

};



#endif  //__MY_YOLO__