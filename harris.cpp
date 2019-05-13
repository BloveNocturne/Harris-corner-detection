/******************************************************************************
    > File Name: harris.cpp

  @IMPORTANT: tested with GNU g++ 5.4.0 compiler. If you encounter any problems
  when compiling this file, please re-compile this file with GNU g++ compiler
  supporting c++ 11 standard and compile like:"g++ harris.cpp -std=c++11 -o harris"
 *****************************************************************************/

#include<iostream>
#include<Eigen/Eigen>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;
using namespace Eigen;

int debug=0;

void showPic(string windowName,Mat pic);

Mat_<int> mul(Mat,Mat);

Mat_<int> acc(Mat_<int>);


int main (int argc, char **argv)
{
    Mat srcImg;
	Mat grayImg;
	Mat Ix,Iy;

	Mat_<int> Ixx;
	Mat_<int> Ixy;
	Mat_<int> Iyy;

    srcImg = imread(argv[1], CV_LOAD_IMAGE_COLOR );
    if (argc != 2 || !srcImg.data) {
        cout << "No srcImg data\n";
        return -1;
    }

    cvtColor(srcImg, grayImg, CV_RGB2GRAY);

	Sobel(grayImg,Ix,srcImg.depth(),1,0);
	Sobel(grayImg,Iy,srcImg.depth(),0,1);

	Ixx=mul(Ix,Ix);
	Ixy=mul(Ix,Iy);
	Iyy=mul(Iy,Iy);

	Mat_<int> A(srcImg.size());
	Mat_<int> B(srcImg.size());
	Mat_<int> C(srcImg.size());

	A=acc(Ixx);
	B=acc(Iyy);
	C=acc(Ixy);
	
	Mat_<int> R(srcImg.size());

	for(int i=0;i<A.rows;++i)
	{
		for(int j=0;j<A.cols;++j)
		{
			R.at<int>(i,j)=A.at<int>(i,j)*B.at<int>(i,j)-C.at<int>(i,j)*C.at<int>(i,j)-0.022*(A.at<int>(i,j)+B.at<int>(i,j))*(A.at<int>(i,j)+B.at<int>(i,j));
		}
	}

	Mat dstImg(srcImg.size(),CV_8UC1);
	for(int i=0;i<A.rows;++i)
	{
		for(int j=0;j<A.cols;++j)
		{
			if(R.at<int>(i,j)>18000)
				dstImg.at<uchar>(i,j)=0;
			else
				dstImg.at<uchar>(i,j)=0;
		}
	}

	Mat compare;
	compare=grayImg.clone();

	for (int i=1; i<dstImg.rows-1; ++i)  
    {  
        for (int j=1; j<dstImg.cols-1; j++)  
        {  
            if (R.at<float>(i, j)>R.at<float>(i - 1, j - 1) &&  
                R.at<float>(i, j)>R.at<float>(i - 1, j) &&  
                R.at<float>(i, j)>R.at<float>(i - 1, j - 1) &&  
                R.at<float>(i, j)>R.at<float>(i - 1, j + 1) &&  
                R.at<float>(i, j)>R.at<float>(i, j - 1) &&  
                R.at<float>(i, j)>R.at<float>(i, j + 1) &&  
                R.at<float>(i, j)>R.at<float>(i + 1, j - 1) &&  
                R.at<float>(i, j)>R.at<float>(i + 1, j) &&  
                R.at<float>(i, j)>R.at<float>(i + 1, j + 1))  
            {  
                if ((int)R.at<float>(i, j) > 3000)  
                {  
                    circle(compare, Point(j,i), 5, Scalar(0,0,255), 2, 8, 0);  
					dstImg.at<uchar>(i,j)=255;
                }  
            }  
  
        }  
    }  



    showPic("source image",srcImg);
    showPic("harris edge",dstImg);
    showPic("compare",compare);

	waitKey(0);	
	cin.get();
    return 0;
}



Mat_<int> mul(Mat a,Mat b)
{
	Mat_<int> temp(a.size());

	for(int i=0;i<a.rows;++i)
	{
		for(int j=0;j<a.cols;++j)
		{
			temp.at<int>(i,j)=a.at<uchar>(i,j)*b.at<uchar>(i,j);
		}
	}
	return temp;
}


Mat_<int> acc(Mat_<int> a)
{
	Mat_<int> temp(a.size());

	for(int i=0;i<a.rows;++i)
	{
		for(int j=0;j<a.cols;++j)
		{
			if(i==0||j==0||i==a.rows-2||j==a.cols-2)
				temp.at<int>(i,j)=0;
			else
			temp.at<int>(i,j)=a.at<int>(i,j)+a.at<int>(i+1,j)+a.at<int>(i-1,j)+a.at<int>(i,j+1)+a.at<int>(i,j-1)+a.at<int>(i+1,j-1)+a.at<int>(i-1,j-1)+a.at<int>(i+1,j+1)+a.at<int>(i-1,j+1);
		}
	}
	
	return temp;
}

void showPic(string windowName,Mat pic)
{
    namedWindow(windowName,CV_WINDOW_AUTOSIZE);
    imshow(windowName,pic);
}
