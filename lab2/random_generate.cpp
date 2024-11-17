// #include <opencv2/opencv.hpp>
// #include <iostream>

// using namespace cv;
// using namespace std;

// int main(int argc, char** argv) {
//     if (argc != 2) {
//         cout << "Usage: " << argv[0] << " <image_path>" << endl;
//         return -1;
//     }

//     // ��ȡ�Ҷ�ͼ��
//     Mat src = imread(argv[1], IMREAD_GRAYSCALE);
//     if (src.empty()) {
//         cout << "Error: Cannot open image " << argv[1] << endl;
//         return -1;
//     }

//     // ����ֱ��ͼ�Ĳ���
//     int histSize = 256;
//     float range[] = { 0, 256 };  // �Ҷ�ֵ��Χ
//     const float* histRange = { range };

//     Mat hist;

//     // ����Ҷ�ͼ���ֱ��ͼ
//     calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);

//     // ��һ��ֱ��ͼ��ʹ��ͼ��߶�Ϊ 400 ����
//     int histImageHeight = 400;
//     int histImageWidth = 512;
//     normalize(hist, hist, 0, histImageHeight, NORM_MINMAX);

//     // ������ʾֱ��ͼ��ͼ�񣬴�СΪ 512x400������Ϊ��ɫ
//     Mat histImage(histImageHeight, histImageWidth, CV_8UC3, Scalar(0, 0, 0));

//     // ��ͼ���ϻ��ƻҶ�ֱ��ͼ��ֻ��һ��ͨ����
//     for (int i = 1; i < histSize; i++) {
//         line(histImage,
//              Point((i - 1) * histImageWidth / histSize, histImageHeight - cvRound(hist.at<float>(i - 1))),
//              Point(i * histImageWidth / histSize, histImageHeight - cvRound(hist.at<float>(i))),
//              Scalar(255, 255, 255), 2); // ��ɫ
//     }

//     // ��ʾ�Ҷ�ͼ���ֱ��ͼ
//     imshow("Gray Histogram", histImage);

//     // �ȴ��û�������رմ���
//     waitKey(0);

//     return 0;
// }
