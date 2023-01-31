#include "DBoW3/DBoW3.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>

using namespace cv;
using namespace std;

 
 
/***************************************************
 *  ./feature_training ../data/
 * ************************************************/

int main( int argc, char** argv ) {
    // read the image 
    cout<<"reading images... "<<endl;
    vector<Mat> images; 
    // 顺序名数据集
    // for ( int i=0; i<10; i++ )
    // {
    //     string path = "../data/"+to_string(i+1)+".png";
    //     images.push_back( imread(path) );
    // }
    // 非顺序名 数据集,暂时写这种丑陋的算法
    string str[] = {"000453","000454", "000908", "001362", "001816", "002270", "002724", "003178", "003632", "004086", "004540"};
    vector<string> image_names(str, str + 11);
    for ( int i=0; i<image_names.size(); i++ )
    {
        string path = "../data/"+image_names[i]+".png";
        // cout << path << endl;
        images.push_back( imread(path) );
    }

    // detect ORB features
    cout<<"detecting ORB features ... "<<endl;
    Ptr< Feature2D > detector = ORB::create();
    vector<Mat> descriptors;
    for ( Mat& image:images )
    {
        vector<KeyPoint> keypoints; 
        Mat descriptor;
        detector->detectAndCompute(image, Mat(), keypoints, descriptor); // orb-> 检测关键点并计算描述符(image,mask,keypoints,)
        descriptors.push_back( descriptor );
    }
    
    // create vocabulary 
    cout<<"creating vocabulary ... "<<endl;
    DBoW3::Vocabulary vocab;    //默认构造函数 k=10,d=5
    vocab.create( descriptors );
    cin.get();
    // cout << "vocabulary info: " << vocab << endl;
    vocab.save( "../vocabulary.yml.gz" );
    cout<<"done"<<endl;
    
    return 0;
}
