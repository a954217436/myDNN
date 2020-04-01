#include "mask.h"
//#include "logSoftmax.h"
//#include <cmath>


Mask::Mask(const cv::String& classifierPath,
           const cv::String& centerfacePath,
           const float confThres,
           const float faceThres,
           const int width,
           const int height)
{
    confThreshold  = confThres;
    faceThreshold  = faceThres;
    inpWidth       = width;
    inpHeight      = height;

    net = cv::dnn::readNetFromONNX(classifierPath);  //net : classifier
    centerface = Centerface(centerfacePath, inpWidth, inpHeight);

    classes.push_back("nomask");
    classes.push_back("mask");
    printf("<Mask::Mask> ==> Read Mask: classifier = %s, centerface = %s ...\n", classifierPath.c_str(), centerfacePath.c_str());

}


std::vector<cv::String> getOutputsNames(cv::dnn::Net& net)
{
    static std::vector<cv::String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
 
        //get the names of all the layers in the network
        std::vector<cv::String> layersNames = net.getLayerNames();
 
        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}


bool Mask::classify(cv::Mat& box)
{
    cv::Mat blob = cv::dnn::blobFromImage(box,
                                          1 / 255.0,
                                          cv::Size(224, 224),
                                          cv::Scalar(0, 0, 0),
                                          true,
                                          false);

    net.setInput(blob);
    std::vector<cv::String> output_names = {"349"};
    std::vector<cv::Mat> heatmap;
    net.forward(heatmap, output_names);

    if (heatmap[0].at<float>(0, 0) > heatmap[0].at<float>(0, 1))
        return false;
    else
        return true;

    // std::vector<float> heat;
    // heat.push_back(heatmap[0].at<float>(0, 0));
    // heat.push_back(heatmap[0].at<float>(0, 1));
    // logSoftmax(heat);
    // int index = argmax(heat.begin(), heat.end());
    // if (index > confThreshold)
    //     return true;
    // else
    //     return false;
}


void Mask::classify_boxes(std::vector<FaceInfo> &faces)
{
    int xmin, ymin, width, height;
    vector<vector<std::string>> result_vector;
    for (int i = 0; i < faces.size(); i++)
    {
        // xmin = int(faces[i].x1 / w_scale);
        // xmax = int(faces[i].x2 / w_scale);
        // ymin = int(faces[i].y1 / h_scale);
        // ymax = int(faces[i].y2 / h_scale);
        xmin   = int(faces[i].x1);
        ymin   = int(faces[i].y1);
        width  = int(faces[i].x2 - faces[i].x1);
        height = int(faces[i].y2 - faces[i].y1);

        cv::Mat imgArr = frame(cv::Rect(xmin, ymin, width, height));

        bool res = classify(imgArr);

        vector<std::string> obj;
        obj.push_back(classes[int(res)]);
        obj.push_back(to_string(xmin));
        obj.push_back(to_string(ymin));
        obj.push_back(to_string(width));
        obj.push_back(to_string(height));

        result_vector.push_back(obj);

        drawPred(int(res), xmin, ymin, faces[i].x2, faces[i].y2, 1.0);
    }
    resultStr = to_lxywh_str(result_vector);
    printf("<Mask::classify_boxes> ==> Detect Result: %s ...\n", resultStr.c_str());
    
}


bool Mask::detect(const cv::String &image_path)
{
    frame = cv::imread(image_path);
    if (frame.empty())
        return false;
    printf("<Mask::detect> ==> Detecting: %s ...\n", image_path.c_str());

    cv::Mat frame_resized;
    cv::resize(frame, frame_resized, cv::Size(inpWidth, inpHeight));

    // int height = frame.rows;
    // int width = frame.cols;
    // float w_scale = width / inpWidth;
    // float h_scale = height / inpHeight;

    std::vector<FaceInfo> face_info;
	centerface.detect(frame, face_info);

    printf("<Mask::detect> ==> Find faces: %ld ...\n", face_info.size());

    classify_boxes(face_info);

    //save("result.jpg");
    return true;
}
