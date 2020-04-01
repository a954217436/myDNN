#include "SocketServer.h"
#include "yolo.h"
#include "mask.h"

#define DEFAULT_PORT 7788 
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

//#pragma GCC diagnostic error "-std=c++11"

using std::string;


// cv::String yoloConfig  = "yolo.cfg";
// cv::String yoloWeights = "yolo.weights";
// cv::String yoloLabels  = "yolo.names";
// Yolo detector;

cv::String centerfacePath  = "centerface.onnx";
cv::String maskPath  = "sbd_mask.onnx";
Mask detector;

DetectorServer::DetectorServer()
{
	ModelLoaded = false;
	StatusStr = "";

	//设置一个socket地址结构server_addr,代表服务器internet地址, 端口
	bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(DEFAULT_PORT);

	//创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
 	server_socket = -1;
	server_socket = socket(PF_INET,SOCK_STREAM,0);
	if( server_socket < 0)
	{
		printf("<DetectorServer::DetectorServer> ==> Create Socket Failed!\n");
		return;
	}
	int opt =1;
	setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	//把socket和socket地址结构联系起来
	if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		printf("<DetectorServer::DetectorServer> ==> Server Bind Port Failed!\n"); 
		return;
	}

	//server_socket用于监听
	if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
	{
		printf("<DetectorServer::DetectorServer> ==> Server Listen Failed!\n"); 
		return;
	}

	printf("<DetectorServer::DetectorServer> ==> Classify Server Start Listening ...\n");

	StatusStr = "serverlisten";

	StartModel();

	StartServer();

	return;
}

DetectorServer::~DetectorServer()
{
	close(server_socket);
}

void DetectorServer::StartModel()
{
	ModelLoaded = false;

	StatusStr = "modelloading";

    //detector = Yolo(config, weights, labels);
	detector = Mask(maskPath, centerfacePath);

	if (0)
	{
		printf("<DetectorServer::StartModel> ==> Load Yolo Model Failed.\n");
		StatusStr = "modelloadfailed";
		return;
	}
	
	printf("<DetectorServer::StartModel> ==> Finish Model Loading! \n");

	ModelLoaded = true;
	return;
}

bool DetectorServer::StartServer()
{
	//定义客户端的socket地址结构client_addr
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	while(1)
	{
		//接受一个到server_socket代表的socket的一个连接
		//如果没有连接请求,就等待到有连接请求--这是accept函数的特性
		//accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信
		//new_server_socket代表了服务器和客户端之间的一个通信通道
		//accept函数把连接到的客户端信息填写到客户端的socket地址结构client_addr中
		int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr,&length);

		if ( new_server_socket < 0)
		{
			printf("<DetectorServer::StartServer> ==> Server Accept Failed!\n");
			break;
		}

		if(ModelLoaded)
			StatusStr = "serverready";

		char recvBuf[BUFFER_SIZE] = "\0";
		bzero(recvBuf, BUFFER_SIZE);
		length = recv(new_server_socket, recvBuf, BUFFER_SIZE, 0);
		if (length < 0)
		{
			printf("<DetectorServer::StartServer> ==> Server Recieve Data Failed!\n");
			break;
		}

		printf("<DetectorServer::StartServer> ==> Server Received: %s \n", recvBuf);

		std::string sendstr = "";
		if(std::string(recvBuf).compare("serverstatus") == 0)
		{
			printf("<DetectorServer::StartServer> ==> Status Request, Current Status = %s \n", StatusStr.c_str());
			sendstr = StatusStr;
		}
		else if(std::string(recvBuf).compare("recognize") == 0)
		{
			printf("<DetectorServer::StartServer> ==> Recognize Request: \n");

			if(detector.detect("/home/robot/ocr.jpg"))
			{
				sendstr = detector.resultStr;
			}
			else
			{
				sendstr = "empty";
			}	
		}
		else
		{
			printf("<DetectorServer::StartServer> ==> Unknown Request String: %s \n", recvBuf);
			sendstr = "unknown";
		}

		send(new_server_socket, sendstr.c_str(), sendstr.length(), 0);

		//close connected sock
		close(new_server_socket);
	}

	return true;
}


