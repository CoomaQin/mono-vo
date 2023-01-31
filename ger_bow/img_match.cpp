#include <iostream>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>

using namespace std;
using namespace cv;

// 对于输入的img，进行匹配相似读计算，大于阈值的匹配正确
void image_match(Mat img_1, string close_data_path, bool flag=false)	{
    //-- 初始化
    Mat img_2;
    vector<Mat> image_all;
    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    vector<vector<KeyPoint>> keypoints_close;
    vector<Mat> descriptors_close;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    //-- 读取图像,保存其特征
    detector->detect(img_1, keypoints_1); 
    descriptor->compute(img_1, keypoints_1, descriptors_1);

    string str[] = {"000453","000454", "000908", "001362", "001816", 
    "002270", "002724", "003178", "003632", "004086", "004540"};
    vector<string> image_names(str, str + 11);
    for (int i = 0; i < image_names.size(); i++)
    {
        string path = close_data_path + image_names[i] + ".png";
        img_2 = imread(path,CV_LOAD_IMAGE_COLOR);
        detector->detect(img_2, keypoints_2);
        descriptor->compute(img_2, keypoints_2, descriptors_2);

        descriptors_close.push_back(descriptors_2);
        keypoints_close.push_back(keypoints_2);
        image_all.push_back(img_2);
    };
    
    vector<DMatch> matches;
    for (int i = 0; i < descriptors_close.size(); i++)
    {
        matcher->match(descriptors_1, descriptors_close[i], matches);
        auto min_max = minmax_element(matches.begin(), matches.end(),
                                        [](const DMatch &m1, const DMatch &m2) { return m1.distance < m2.distance; });
        double min_dist = min_max.first->distance;
        double max_dist = min_max.second->distance;

        std::vector<DMatch> good_matches;
        for (int i = 0; i < descriptors_1.rows; i++) {
            if (matches[i].distance <= max(2 * min_dist, 30.0)) {
            good_matches.push_back(matches[i]);
            }
        }
        cout << good_matches.size() << endl;
   

        // TODO
        // 1, 相似型 判断有问题 
        // 2, 修改跑通后，需要嫁接到src 里面，链接的时候需要注意opencv本版链接问题。
         
        // if (good_matches.size() > 50)
        // {
        //     cout << "匹配正确" << endl;
        //     flag = true;
        //     break;
        // }
    };
}
int main(){


    // char filename[200];
    // sprintf(filename, "/home/hq63/innov-train/tunnel_backward/000453.png", 0);
    Mat currImage = imread("../data/000453.png",CV_LOAD_IMAGE_COLOR);
    string close_data_path = "../data/";
    image_match(currImage, close_data_path, false);

    return 0;
}