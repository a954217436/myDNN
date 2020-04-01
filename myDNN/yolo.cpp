#include <fstream>
#include "yolo.h"


Yolo::Yolo(const cv::String& modelConfigPath, 
           const cv::String& weightsPath,
           const cv::String& labelPath,
           const float confThres,
           const float nmsThres,
           const int width,
           const int height)
{
    confThreshold = confThres;
    nmsThreshold  = nmsThres;
    inpWidth      = width;
    inpHeight     = height;
    
    printf("<Yolo::Yolo> ==> Read Darknet: cfg = %s, weights = %s ...\n", modelConfigPath.c_str(), weightsPath.c_str());
    net = cv::dnn::readNetFromDarknet(modelConfigPath, weightsPath);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Load names of classes
    std::ifstream classNamesFile(labelPath.c_str());
    if (classNamesFile.is_open())
    {
        std::string className = "";
        while (std::getline(classNamesFile, className))
            classes.push_back(className);
    }
    else
    {
        printf("<Yolo::Yolo> ==> Can not open classNamesFile: %s ...\n", labelPath.c_str());
    }

}


std::vector<cv::String> Yolo::getOutputsNames()
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



// Remove the bounding boxes with low confidence using non-maxima suppression
void Yolo::postprocess(std::vector<cv::Mat>& outs)
{
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
 
    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
 
            if (confidence > confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
 
                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }
 
    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

    vector<vector<std::string>> result_vector;

    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        vector<std::string> obj;
        obj.push_back(classes[classIds[idx]]);
        obj.push_back(to_string(box.x));
        obj.push_back(to_string(box.y));
        obj.push_back(to_string(box.width));
        obj.push_back(to_string(box.height));

        result_vector.push_back(obj);

        drawPred(classIds[idx], 
                 box.x, 
                 box.y,
                 box.x + box.width, 
                 box.y + box.height, 
                 confidences[idx]);
    }
    resultStr = to_lxywh_str(result_vector);
    printf("<Yolo::postprocess> ==> Detect Result: %s ...\n", resultStr.c_str());
}

bool Yolo::detect(const cv::String& image_path)
{
    frame = cv::imread(image_path);
    if (frame.empty())
        return false;
    printf("<Yolo::detect> ==> Detecting: %s ...\n", image_path.c_str());
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0,0,0), true, false);
    net.setInput(blob);
    
    std::vector<cv::Mat> outs;
    net.forward(outs, getOutputsNames());
    postprocess(outs);
    save("./result.jpg");
    //show("Res");

    return true;
}