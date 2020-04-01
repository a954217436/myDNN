#ifndef __DETECTOR__
#define __DETECTOR__

#include <vector>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

class Detector
{
public:
    Detector(){}

    void save(const cv::String& savePath);

    void show(const cv::String& winName);

    virtual bool detect(const cv::String &imgPath) = 0;

    string to_lxywh_str(vector<vector<string>>& result_vector);

public:
    void drawPred(const int classId, 
                  const int left, 
                  const int top, 
                  const int right, 
                  const int bottom,
                  const float conf);

    
public:
    cv::dnn::Net net;
    cv::Mat frame;
    std::vector<std::string> classes;
    std::string resultStr;

    float confThreshold;
    float nmsThreshold;
    int inpWidth;
    int inpHeight;
};































#endif  //__DETECTOR__