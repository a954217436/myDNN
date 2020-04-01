// For log_softmax
// #include "log_softmax.h"
// #include <iostream>
// int main()
// {
//     double a[]={3.1320703, -3.131977};
//     std::vector<double> v_a(a, a + sizeof a / sizeof a[0]);
//     std::vector<double>::const_iterator it = v_a.begin();
//     for (; it != v_a.end(); ++it)
//     {
//         std::cout << *it << " ";
//     }
//     std::cout << std::endl;
//     softmax(v_a);
//     it = v_a.begin();
//     for (; it != v_a.end(); ++it)
//     {
//         std::cout << *it << " ";
//     }
//     std::cout << std::endl;
//     return 0;
// }




// For Mask
// #include <ctime>
// #include <iostream>
// #include "mask.h"
// int main()
// {
//     cv::String centerfacePath  = "centerface.onnx";
//     cv::String maskPath  = "sbd_mask.onnx";

//     clock_t start;
//     start = clock();

//     Mask detector(maskPath, centerfacePath);

//     double endtime = (double)(clock() - start) / CLOCKS_PER_SEC;
//     std::cout << "Load Net Time: " << endtime << std::endl;

//     start = clock();

//     if(!detector.detect("1.jpg"))
//     {
//         std::cout << "Failed!" << std::endl;
//     }
//     endtime = (double)(clock() - start) / CLOCKS_PER_SEC;
//     std::cout << "Inference Image Time: " << endtime << std::endl;

//     detector.save("res.jpg");
//     return 0;
// }




////For SocketServer
#include "SocketServer.h"
int main()
{
	DetectorServer server;
	return 0;
}




//For yolo
/*
#include <cstdlib>
#include <ctime>
#include <yolo.h>

int main()
{
    cv::String config  = "yolo.cfg";
    cv::String weights = "yolo.weights";
    cv::String labels  = "yolo.names";

    clock_t start;
    start = clock();

    Yolo detector(config, weights, labels);

    double endtime = (double)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Load Net Time: " << endtime << std::endl;

    start = clock();

    if(!detector.detect("1.jpg"))
    {
        std::cout << "Failed!" << std::endl;
    }
    endtime = (double)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Inference Image Time: " << endtime << std::endl;
    return 0;
}*/





//For detector(Base)
/*
#include <detector.h>
int main1()
{
    Detector det;

    vector<std::string> a;
    a.push_back("nc");
    a.push_back("100");
    a.push_back("200");
    a.push_back("500");
    a.push_back("500");

    vector<std::string> b;
    b.push_back("gkxfw");
    b.push_back("10");
    b.push_back("20");
    b.push_back("50");
    b.push_back("50");

    vector<std::string> c;
    c.push_back("yw");
    c.push_back("1000");
    c.push_back("2000");
    c.push_back("5000");
    c.push_back("5000");


    vector<vector<std::string>> r_vector;
    r_vector.push_back(a);
    r_vector.push_back(b);
    r_vector.push_back(c);

    std::string res = det.to_lxywh_str(r_vector);
    std::cout << "To_lxywh: " << res << endl;
}*/
