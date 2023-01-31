#include <iostream>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>

using namespace std;
using namespace cv;

float image_match( Mat img_1, Mat img_2)	{
  //-- 初始化
  std::vector<KeyPoint> keypoints_1, keypoints_2;
  Mat descriptors_1, descriptors_2;
  Ptr<FeatureDetector> detector = ORB::create();
  Ptr<DescriptorExtractor> descriptor = ORB::create();
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

  //-- 第一步:检测 Oriented FAST 角点位置
  detector->detect(img_1, keypoints_1);
  detector->detect(img_2, keypoints_2);
  //-- 第二步:根据角点位置计算 BRIEF 描述子
  descriptor->compute(img_1, keypoints_1, descriptors_1);
  descriptor->compute(img_2, keypoints_2, descriptors_2);
  //-- 第三步:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
  vector<DMatch> matches;
  matcher->match(descriptors_1, descriptors_2, matches);
  //-- 第四步:匹配点对筛选
  // 计算最小距离和最大距离
  auto min_max = minmax_element(matches.begin(), matches.end(),
                                [](const DMatch &m1, const DMatch &m2) { return m1.distance < m2.distance; });
  double min_dist = min_max.first->distance;
  double max_dist = min_max.second->distance;
  std::vector<DMatch> good_matches;
  for (int i = 0; i < descriptors_1.rows; i++) {
    if (matches[i].distance <= max(min_dist, 30.0)) {
      good_matches.push_back(matches[i]);
    }
  }
  float num_match = good_matches.size();
  int total_feature = keypoints_1.size();
//   cout << num_match << endl;
  return float(num_match / total_feature);
}

int main(){
    //    string str[] = {"000453","000454", "000908", "001362", "001816", 
    // "002270", "002724", "003178", "003632", "004086", "004540"};

    string path1 = "../data/000453.png";
    string path2 = "../data/000454.png";
    
    string str[] = {"000453","000454", "000908", "001362", "001816", 
    "002270", "002724", "003178", "003632", "004086", "004540"};
    vector<string> image_names(str, str + 11); 

    //-- 读取图像
    Mat img_1 = imread(path1, CV_LOAD_IMAGE_COLOR);
    for(auto &i : image_names){
        path2 = "../data/" + i + ".png";
        Mat img_2 = imread(path2, CV_LOAD_IMAGE_COLOR);
        float score = image_match(img_1, img_2);
        // cout << score << endl;
        if( score > 0.5){
            cout << "image_"<<i<<":"<<score << endl;
        }
    }
    return 0;
}
