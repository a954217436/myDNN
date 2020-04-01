#include <stdio.h>
#include <algorithm>
#include "detector.h"


void Detector::save(const cv::String& savePath)
{
    if(frame.empty())
        return;
    cv::imwrite(savePath, frame);
}


void Detector::show(const cv::String& winName)
{
    if(frame.empty())
        return;
    cv::imshow(winName, frame);
    cv::waitKey(0);
    cv::destroyAllWindows();
}


bool compare(const vector<string>& a, const vector<string>& b) 
{
    return std::stoi(a[1]) + std::stoi(a[2]) < std::stoi(b[1]) + std::stoi(b[2]);
}


string Detector::to_lxywh_str(vector<vector<string>>& result_vector)
{
    string resultString = "";
    if(result_vector.size()==0)
        return resultString;

    sort(result_vector.begin(), result_vector.end(), compare);
    for (int i = 0; i < result_vector.size(); i++)
    {
        if(result_vector[i].size() != 5)
            continue;
        for (int j = 0; j < 5; j++)
        {
            resultString += result_vector[i][j];
            resultString += " ";
        }
    }
    return resultString;
}


void Detector::drawPred(const int classId, 
                        const int left, 
                        const int top, 
                        const int right, 
                        const int bottom,
                        const float conf)
{
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 0, 255));

    std::string label = cv::format("%.2f", conf);
    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId] + ":" + label;
    }
    else
    {
        printf("<Detector::drawPred> ==> Classes is Empty!\n");
    }

    int baseLine;
    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    cv::putText(frame, label, cv::Point(left, std::max(top, labelSize.height)), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
}