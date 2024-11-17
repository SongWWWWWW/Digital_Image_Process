// #include <opencv2/opencv.hpp>
// #include <iostream>

// using namespace cv;
// using namespace std;

// int main(int argc, char** argv) {
//     if (argc != 2) {
//         cout << "Usage: " << argv[0] << " <image_path>" << endl;
//         return -1;
//     }

//     // 读取灰度图像
//     Mat src = imread(argv[1], IMREAD_GRAYSCALE);
//     if (src.empty()) {
//         cout << "Error: Cannot open image " << argv[1] << endl;
//         return -1;
//     }

//     // 定义直方图的参数
//     int histSize = 256;
//     float range[] = { 0, 256 };  // 灰度值范围
//     const float* histRange = { range };

//     Mat hist;

//     // 计算灰度图像的直方图
//     calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);

//     // 归一化直方图，使得图像高度为 400 像素
//     int histImageHeight = 400;
//     int histImageWidth = 512;
//     normalize(hist, hist, 0, histImageHeight, NORM_MINMAX);

//     // 创建显示直方图的图像，大小为 512x400，背景为黑色
//     Mat histImage(histImageHeight, histImageWidth, CV_8UC3, Scalar(0, 0, 0));

//     // 在图像上绘制灰度直方图（只有一个通道）
//     for (int i = 1; i < histSize; i++) {
//         line(histImage,
//              Point((i - 1) * histImageWidth / histSize, histImageHeight - cvRound(hist.at<float>(i - 1))),
//              Point(i * histImageWidth / histSize, histImageHeight - cvRound(hist.at<float>(i))),
//              Scalar(255, 255, 255), 2); // 白色
//     }

//     // 显示灰度图像的直方图
//     imshow("Gray Histogram", histImage);

//     // 等待用户按键后关闭窗口
//     waitKey(0);

//     return 0;
// }
