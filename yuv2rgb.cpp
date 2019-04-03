    #include <iostream>
	#include <fstream>
    #include <sstream>
	#include <opencv2/highgui/highgui.hpp>
    #include <opencv/cv.h>
    using namespace std;
    #define FCount 10
    #define ISizeX 352	//图像宽度
    #define ISizeY 288	//图像高度
     
    unsigned char Y[FCount][ISizeY][ISizeX];   
    unsigned char U[FCount][ISizeY/2][ISizeX/2];	
    unsigned char V[FCount][ISizeY/2][ISizeX/2];
     
    // 将图片文件写入
    void FileWriteFrames()
    {
    	char *filename = "file_1533731243565.yuv";
    	ifstream readMe(filename, ios::in | ios::binary);  // 打开并读yuv数据
    	IplImage *image, *rgbimg, *yimg, *uimg, *vimg, *uuimg, *vvimg;
    	cvNamedWindow("yuv",CV_WINDOW_AUTOSIZE);
    	rgbimg = cvCreateImage(cvSize(ISizeX, ISizeY), IPL_DEPTH_8U, 3);
    	image = cvCreateImage(cvSize(ISizeX, ISizeY), IPL_DEPTH_8U, 3);
        
    	yimg = cvCreateImageHeader(cvSize(ISizeX, ISizeY), IPL_DEPTH_8U, 1);    // 亮度分量
    	uimg = cvCreateImageHeader(cvSize(ISizeX/2, ISizeY/2), IPL_DEPTH_8U, 1);  // 这两个都是色度分量
    	vimg = cvCreateImageHeader(cvSize(ISizeX/2, ISizeY/2), IPL_DEPTH_8U, 1);
        
    	uuimg = cvCreateImage(cvSize(ISizeX, ISizeY), IPL_DEPTH_8U, 1);
    	vvimg = cvCreateImage(cvSize(ISizeX, ISizeY), IPL_DEPTH_8U, 1);
    	int nframes;
    	for(nframes = 0; nframes < FCount; nframes ++)
    	{
    		char nframesstr[20];
     
    		readMe.read((char*)Y[nframes],ISizeX*ISizeY);	//读取Y分量
    		readMe.read((char*)U[nframes],ISizeX/2*ISizeY/2);//读取U分量
    		readMe.read((char*)V[nframes],ISizeX/2*ISizeY/2);//读取V分量
     
    		cvSetData(yimg,Y[nframes],ISizeX);
    		cvSetData(uimg,U[nframes], ISizeX/2);
    		cvSetData(vimg,V[nframes], ISizeX/2);
    		
    		
    		cvResize(uimg,uuimg, CV_INTER_LINEAR);
    		cvResize(vimg,vvimg, CV_INTER_LINEAR);
    		cvMerge(yimg,uuimg,vvimg,NULL,image);   // 合并单通道为三通道
    		cvCvtColor(image,rgbimg,CV_YCrCb2RGB);  //	YUV转换为RGB
    		
    		stringstream ss;  // 类型转换统一转换为char* 类型
    		ss << nframes;
    		ss << ".jpg" ;
    		ss >> nframesstr;
    		cvShowImage("yuv", rgbimg);
    		cvSaveImage(nframesstr,rgbimg);
    		int c = cvWaitKey(300);
    		if((char)c == 27)
    		{
    			break;
    		}
    	}
    	readMe.close();
    	cvReleaseImage(&uuimg);
        <span style="white-space:pre">	</span>cvReleaseImage(&vvimg);
    	cvReleaseImageHeader(&yimg);
    	cvReleaseImageHeader(&uimg);
    	cvReleaseImageHeader(&vimg);
    	cvReleaseImage(&image);
    	cvDestroyWindow("yuv");	
    }
     
     
    int main()
    {
    	FileWriteFrames();
    	return 0;
    }
