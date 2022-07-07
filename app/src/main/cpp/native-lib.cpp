#include <jni.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <opencv2/imgproc/types_c.h>
#include "util.h"
#include "my_log.h"

using namespace std;
using namespace cv;

extern "C"
JNIEXPORT jobject JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_blurBitmap(JNIEnv *env, jobject thiz,
                                               jobject bitmap,
                                               jint width,
                                               jint height) {
    Mat m;
    BitmapToMat2(env, bitmap, m, false);

    Mat dst2;
    GaussianBlur(m, dst2, Size(15, 15), 15, 0, 4);

    //get source bitmap's config
    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap, mid);
    jobject _bitmap = matToBitmap(env, dst2, false, bitmap_config);
    return _bitmap;
}


extern "C"
JNIEXPORT jobject JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_flipBitmap(JNIEnv *env, jobject thiz,
                                               jobject bitmap,
                                               jint flipCode) {
    Mat src;

    BitmapToMat2(env, bitmap, src, true);

    flip(src, src, flipCode);

    //get source bitmap's config
    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap, mid);
    jobject _bitmap = matToBitmap(env, src, false, bitmap_config);
    return _bitmap;
}


extern "C"
JNIEXPORT jobject JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_waterMark(JNIEnv *env, jobject thiz, jobject bitmap,
                                              jobject mark,
                                              jint witch_corner) {
    if (nullptr == bitmap || nullptr == mark){
        jclass je = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(je, "Cloud not find the bitmap.");
    }

    Mat src, wtMark;

    BitmapToMat2(env, bitmap, src, true);
    BitmapToMat2(env, mark, wtMark, true);

    Mat imgROI;

    switch (witch_corner) {
        case 1: {       //左上角
            imgROI = Mat(src, Rect(0, 0,
                                   wtMark.cols, wtMark.rows));
            break;
        }
        case 2: {       //右上角
            imgROI = Mat(src, Rect(src.cols - wtMark.cols, 0,
                                   wtMark.cols, wtMark.rows));
            break;
        }
        case 3: {       //左下角
            imgROI = Mat(src, Rect(0, src.rows - wtMark.rows,
                                   wtMark.cols, wtMark.rows));
            break;
        }
        default: {       //右下角
            imgROI = Mat(src, Rect(src.cols - wtMark.cols, src.rows - wtMark.rows,
                                   wtMark.cols, wtMark.rows));
            break;
        }
    }

    wtMark.copyTo(imgROI);

    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap, mid);
    jobject _bitmap = matToBitmap(env, src, false, bitmap_config);
    return _bitmap;
}


extern "C"
JNIEXPORT jobject JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_saltBitmap(JNIEnv *env, jobject thiz, jobject bitmap,
                                               jint times) {
    Mat src;

    BitmapToMat2(env, bitmap, src, true);

    saltBitmap(src, times);

    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap, mid);
    jobject _bitmap = matToBitmap(env, src, false, bitmap_config);
    return _bitmap;
}


extern "C"
JNIEXPORT jobject JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_colorReduce(JNIEnv *env, jobject thiz, jobject bitmap,
                                                jint div) {
    Mat src;

    const int64 start = getTickCount();

    BitmapToMat2(env, bitmap, src, true);

    colorReduce(src, div);

    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap, mid);
    jobject _bitmap = matToBitmap(env, src, false, bitmap_config);

    double duration = (getTickCount() - start) / getTickFrequency();

    LOGE("duration %.6f", duration);

    return _bitmap;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_getOpenCVVersion(JNIEnv *env, jobject thiz) {
    String str = getVersionString();

    char * chars = const_cast<char *>(str.data());

    return env ->NewStringUTF(chars);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_mergeBitmap(JNIEnv *env, jobject thiz, jobject bitmap,
                                                jobject mark, jfloat merge_weight) {
    Mat src, bg, result;

    BitmapToMat2(env, bitmap, src, true);
    BitmapToMat2(env, mark, bg, true);

    addWeighted(src, (1 - merge_weight), bg, merge_weight, 0., result);

    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap, mid);
    jobject _bitmap = matToBitmap(env, result, false, bitmap_config);

    return _bitmap;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_top_topsea_ndkopencvdemo_MainActivity_maskBitmap(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat src;

    BitmapToMat2(env, bitmap, src, false);

    Mat mask(src.rows, src.cols, CV_8UC4, Scalar(0, 0, 0, 255));
    circle(mask, Point(src.cols / 2, src.rows / 2), 600,
           Scalar(255, 255, 255, 255), -1);

    Mat rst;

    bitwise_and(src, mask, rst);

    // mask之外的区域填充白色
//    const uchar white = 255;
//
//    for (int i = 0; i < rst.rows; ++i) {
//        for (int j = 0; j < rst.cols; ++j) {
//            if (!mask.at<Vec4b>(i, j)[0]) {
//                rst.at<Vec4b>(i, j)[0] = white;
//                rst.at<Vec4b>(i, j)[1] = white;
//                rst.at<Vec4b>(i, j)[2] = white;
//            }
//        }
//    }

    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap, mid);
    jobject _bitmap = matToBitmap(env, rst, false, bitmap_config);

    return _bitmap;
}