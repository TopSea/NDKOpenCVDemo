//
// Created by Gao on 2022/6/29.
//

#ifndef OPENCV_UTIL_H
#define OPENCV_UTIL_H
#include <jni.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>

using namespace std;
using namespace cv;

void BitmapToMat2(JNIEnv *env, jobject &bitmap, Mat &mat, jboolean needUnPremultiplyAlpha);

jobject matToBitmap(JNIEnv * env, Mat & src, bool needPremultiplyAlpha, jobject bitmap_config);

void saltBitmap(Mat src, int times);

void colorReduce(Mat src, int var);

#endif //OPENCV_UTIL_H


