// #include <stdio.h>      // 用于标准输入输出函数，如 wprintf
// #include <stdlib.h>     // 用于内存分配函数，如 malloc 和 free
// #include <string.h>     // 用于 memset 函数
// #include <stdint.h>     // 用于 uint8_t 类型
// #define BMP_HEADER_SIZE 54
// #define MAX_COLOR_VALUE 255
// #define HISTOGRAM_WIDTH 256
// #define HISTOGRAM_HEIGHT 256

// // 读取 BMP 图像数据
// unsigned char* readBMP(const char* filename, int* width, int* height, unsigned char* header) {
//     FILE* file = fopen(filename, "rb");
//     if (!file) {
//         printf("Error: Cannot open file %s.\n", filename);
//         return NULL;
//     }

//     fread(header, sizeof(unsigned char), BMP_HEADER_SIZE, file);

//     *width = *(int*)&header[18];
//     *height = *(int*)&header[22];

//     int imageSize = (*width) * (*height);
//     unsigned char* imageData = (unsigned char*)malloc(imageSize);
//     fread(imageData, sizeof(unsigned char), imageSize, file);

//     fclose(file);
//     return imageData;
// }

// // 保存 BMP 图像
// void saveBMP(const char* filename, const unsigned char* header, const unsigned char* image, int width, int height) {
//     FILE* file = fopen(filename, "wb");
//     if (!file) {
//         printf("Error: Cannot write to file %s.\n", filename);
//         return;
//     }

//     fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, file);
//     fwrite(image, sizeof(unsigned char), width * height, file);
//     fclose(file);
// }

// // 计算直方图并生成 BMP 图像
// void saveHistogramImage(const int* histogram, const char* filename) {
//     unsigned char histogramImage[HISTOGRAM_HEIGHT][HISTOGRAM_WIDTH];
//     memset(histogramImage, 255, sizeof(histogramImage)); // 填充白色

//     int maxCount = 0;
//     for (int i = 0; i <= MAX_COLOR_VALUE; i++) {
//         if (histogram[i] > maxCount) {
//             maxCount = histogram[i];
//         }
//     }

//     printf("Max histogram count: %d\n", maxCount); // 打印调试信息

//     // 画直方图的条形
//     for (int i = 0; i <= MAX_COLOR_VALUE; i++) {
//         int barHeight = (histogram[i] * (HISTOGRAM_HEIGHT - 1)) / maxCount; // 确保条形高度在图像高度范围内
//         barHeight = barHeight > 0 ? barHeight : 1; 
//         printf("Histogram[%d] = %d, barHeight = %d\n", i, histogram[i], barHeight); // 打印调试信息
//         for (int j = 0; j < barHeight; j++) {
//             histogramImage[HISTOGRAM_HEIGHT - 1 - j][i] = 255; // 255 for red
//             histogramImage[HISTOGRAM_HEIGHT - 1 - j][i] = 0; // 0 for green
//             histogramImage[HISTOGRAM_HEIGHT - 1 - j][i] = 0; // 0 for blue
//         }
//     }

//     // 为直方图创建 BMP 文件头
//     unsigned char header[54] = {
//         0x42, 0x4D, 0x36, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 文件标志 "BM"
//         0x36, 0x00, 0x00, 0x00, // 数据偏移量
//         0x28, 0x00, 0x00, 0x00, // 信息头大小
//         0x00, 0x01, 0x00, 0x00, // 宽度 256
//         0x00, 0x01, 0x00, 0x00, // 高度 256
//         0x01, 0x00, // 平面数
//         0x08, 0x00, // 每像素位数（8 位）
//         0x00, 0x00, 0x00, 0x00, // 压缩类型（无压缩）
//         0x00, 0x04, 0x00, 0x00, // 图像大小
//         0x13, 0x0B, 0x00, 0x00, // 水平分辨率
//         0x13, 0x0B, 0x00, 0x00, // 垂直分辨率
//         0x01, 0x00, 0x00, 0x00, // 调色板颜色数
//         0x00, 0x00, 0x00, 0x00  // 重要颜色数
//     };

//     // 写入文件
//     FILE* file = fopen(filename, "wb");
//     if (!file) {
//         printf("Error: Cannot write to file %s.\n", filename);
//         return;
//     }

//     fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, file);
//     fwrite(histogramImage, sizeof(unsigned char), HISTOGRAM_WIDTH * HISTOGRAM_HEIGHT, file);
//     fclose(file);
// }
// typedef struct {
//     uint16_t bfType;        // 文件类型，必须是 'BM'
//     uint32_t bfSize;        // 文件大小
//     uint16_t bfReserved1;   // 保留字段，通常为 0
//     uint16_t bfReserved2;   // 保留字段，通常为 0
//     uint32_t bfOffBits;     // 从文件头到数据部分的偏移量
// } BMPFileHeader;

// // BMP 信息头
// typedef struct {
//     uint32_t biSize;        // 信息头的大小
//     int32_t biWidth;        // 图像宽度
//     int32_t biHeight;       // 图像高度
//     uint16_t biPlanes;      // 色彩平面数，必须为 1
//     uint16_t biBitCount;    // 每个像素的位数，8 位（256 色）
//     uint32_t biCompression; // 压缩方式（0 表示不压缩）
//     uint32_t biSizeImage;   // 图像数据的大小
//     uint32_t biClrUsed;     // 使用的颜色数
//     uint32_t biClrImportant; // 重要的颜色数
// } BMPInfoHeader;
// void write_bmp(const char *outputImagePath, BMPFileHeader *fileHeader, BMPInfoHeader *infoHeader, uint8_t *data, int padding) {
//     FILE *file = fopen(outputImagePath, "wb");
//     if (!file) {
//         wprintf(L"无法打开文件进行写入\n");
//         return;
//     }

//     // 写入文件头和信息头
//     fwrite(fileHeader, sizeof(BMPFileHeader), 1, file);
//     fwrite(infoHeader, sizeof(BMPInfoHeader), 1, file);

//     // 写入图像数据，考虑到 padding
//     for (int y = 0; y < infoHeader->biHeight; y++) {
//         fwrite(data + y * infoHeader->biWidth, sizeof(uint8_t), infoHeader->biWidth, file);
//         if (padding > 0) {
//             uint8_t paddingData[3] = {0, 0, 0};
//             fwrite(paddingData, 1, padding, file);
//         }
//     }

//     fclose(file);
// }

// void save_histogram_as_image(const char *outputImagePath, int histogram[256], int maxHeight) {
//     int width = 600;  // 直方图的宽度
//     int height = maxHeight > 0 ? maxHeight : 256;  // 确保 height 不为零

//     // 为直方图图像分配内存
//     uint8_t *histogramImage = (uint8_t *)malloc(width * height);
//     if (!histogramImage) {
//         wprintf(L"无法分配内存来保存直方图图像\n");
//         return;
//     }

//     // 创建一个黑色背景的直方图
//     memset(histogramImage, 0, width * height);

//     // 根据直方图绘制图像
//     for (int i = 0; i < 256; i++) {
//         int barHeight = (int)((histogram[i] * (float)height) / maxHeight);
//         for (int j = 0; j < barHeight; j++) {
//             histogramImage[(height - 1 - j) * width + i] = 255;  // 使用白色绘制柱形图
//         }
//     }

//     // 创建 BMP 文件头和信息头
//     BMPFileHeader fileHeader;
//     BMPInfoHeader infoHeader;

//     fileHeader.bfType = 0x4D42;  // BMP 文件标识符
//     fileHeader.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + width * height;
//     fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
//     fileHeader.bfReserved1 = 0;
//     fileHeader.bfReserved2 = 0;

//     infoHeader.biSize = sizeof(BMPInfoHeader);
//     infoHeader.biWidth = width;
//     infoHeader.biHeight = height;
//     infoHeader.biPlanes = 1;
//     infoHeader.biBitCount = 8;
//     infoHeader.biCompression = 0;
//     infoHeader.biSizeImage = width * height;
//     infoHeader.biClrUsed = 256;
//     infoHeader.biClrImportant = 0;

//     // 保存 BMP 文件
//     int padding = (4 - (width % 4)) % 4;
//     write_bmp(outputImagePath, &fileHeader, &infoHeader, histogramImage, padding);

//     free(histogramImage);
// }

// // 计算直方图
// void calculateHistogram(const unsigned char* image, int width, int height, int* histogram) {
//     for (int i = 0; i <= MAX_COLOR_VALUE; i++) {
//         histogram[i] = 0;
//     }

//     for (int i = 0; i < width * height; i++) {
//         histogram[image[i]]++;
//     }
// }

// // 直方图均衡化
// void equalizeHistogram(unsigned char* image, int width, int height) {
//     int histogram[256] = {0};
//     int cumulativeHistogram[256] = {0};

//     for (int i = 0; i < width * height; i++) {
//         histogram[image[i]]++;
//     }

//     cumulativeHistogram[0] = histogram[0];
//     for (int i = 1; i <= MAX_COLOR_VALUE; i++) {
//         cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
//     }

//     int totalPixels = width * height;
//     for (int i = 0; i <= MAX_COLOR_VALUE; i++) {
//         cumulativeHistogram[i] = (cumulativeHistogram[i] * MAX_COLOR_VALUE) / totalPixels;
//     }

//     for (int i = 0; i < width * height; i++) {
//         image[i] = cumulativeHistogram[image[i]];
//     }
// }

// int main(int argc, char** argv) {
//     if (argc != 2) {
//         printf("Usage: %s <image_path>\n", argv[0]);
//         return -1;
//     }

//     int width, height;
//     unsigned char header[BMP_HEADER_SIZE];
//     unsigned char* imageData = readBMP(argv[1], &width, &height, header);
//     if (!imageData) {
//         return -1;
//     }

//     int histogram[256];
//     calculateHistogram(imageData, width, height, histogram);
//     // saveHistogramImage(histogram, "original_histogram.bmp");
//     save_histogram_as_image("original_histogram.bmp",histogram,256);

//     equalizeHistogram(imageData, width, height);
//     saveBMP("equalized_image.bmp", header, imageData, width, height);
//     // save_histogram_as_image("original_histogram.bmp",histogram,256);

//     calculateHistogram(imageData, width, height, histogram);
//     // saveHistogramImage(histogram, "equalized_histogram.bmp");
//     save_histogram_as_image("equalized_histogram.bmp",histogram,256);


//     free(imageData);
//     printf("Original and equalized histograms saved as BMP images.\n");
//     printf("Equalized image saved as equalized_image.bmp.\n");
//     return 0;
// }
#include <stdio.h>      // 用于标准输入输出函数，如 wprintf
#include <stdlib.h>     // 用于内存分配函数，如 malloc 和 free
#include <string.h>     // 用于 memset 函数
#include <stdint.h>     // 用于 uint8_t 类型
#include <math.h>

#define BMP_HEADER_SIZE 54
#define MAX_COLOR_VALUE 255
#define HISTOGRAM_WIDTH 256
#define HISTOGRAM_HEIGHT 256
typedef struct {
    uint16_t bfType;        // 文件类型，必须是 'BM'
    uint32_t bfSize;        // 文件大小
    uint16_t bfReserved1;   // 保留字段，通常为 0
    uint16_t bfReserved2;   // 保留字段，通常为 0
    uint32_t bfOffBits;     // 从文件头到数据部分的偏移量
} BMPFileHeader;

// BMP 信息头
typedef struct {
    uint32_t biSize;        // 信息头的大小
    int32_t biWidth;        // 图像宽度
    int32_t biHeight;       // 图像高度
    uint16_t biPlanes;      // 色彩平面数，必须为 1
    uint16_t biBitCount;    // 每个像素的位数，8 位（256 色）
    uint32_t biCompression; // 压缩方式（0 表示不压缩）
    uint32_t biSizeImage;   // 图像数据的大小
    uint32_t biClrUsed;     // 使用的颜色数
    uint32_t biClrImportant; // 重要的颜色数
} BMPInfoHeader;

typedef union {
    uint8_t bytes[4];
    uint32_t value;
} LITTLE;
void write_bmp(const char *outputImagePath, BMPFileHeader *fileHeader, BMPInfoHeader *infoHeader, uint8_t *data, int padding) {
    FILE *file = fopen(outputImagePath, "wb");
    if (!file) {
        wprintf(L"无法打开文件进行写入\n");
        return;
    }

    // 写入文件头和信息头
    fwrite(fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(infoHeader, sizeof(BMPInfoHeader), 1, file);

    // 写入图像数据，考虑到 padding
    for (int y = 0; y < infoHeader->biHeight; y++) {
        fwrite(data + y * infoHeader->biWidth, sizeof(uint8_t), infoHeader->biWidth, file);
        if (padding > 0) {
            uint8_t paddingData[3] = {0, 0, 0};
            fwrite(paddingData, 1, padding, file);
        }
    }

    fclose(file);
}
// 读取 BMP 图像数据
unsigned char* readBMP(const char* filename, int* width, int* height, unsigned char* header) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open file %s.\n", filename);
        return NULL;
    }

    fread(header, sizeof(unsigned char), BMP_HEADER_SIZE, file);

    *width = *(int*)&header[18];
    *height = *(int*)&header[22];

    int imageSize = (*width) * (*height);
    unsigned char* imageData = (unsigned char*)malloc(imageSize);
    fread(imageData, sizeof(unsigned char), imageSize, file);

    fclose(file);
    return imageData;
}

// 保存 BMP 图像
void saveBMP(const char* filename, const unsigned char* header, const unsigned char* image, int width, int height) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Cannot write to file %s.\n", filename);
        return;
    }

    fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, file);
    fwrite(image, sizeof(unsigned char), width * height, file);
    fclose(file);
}

// 计算直方图并生成 BMP 图像
int save_histogram_as_image(const char *fileName, int histogram[256], uint32_t width, uint32_t height) {
    FILE *fp;
    uint32_t i, j;
    LITTLE l_width, l_height, l_bfSize, l_biSizeImage;

    // 计算宽度应为4字节对齐
    uint32_t width_r = (width * 24 / 8 + 3) / 4 * 4;
    uint32_t bfSize = width_r * height + 54 + 2;
    uint32_t biSizeImage = width_r * height;

    l_width.value = width;
    l_height.value = height;
    l_bfSize.value = bfSize;
    l_biSizeImage.value = biSizeImage;

    // BMP 文件头和信息头
    uint8_t bmp_head_map[54] = {
        /* BMP 文件头 */
        0x42, 0x4d,
        l_bfSize.bytes[0], l_bfSize.bytes[1], l_bfSize.bytes[2], l_bfSize.bytes[3],
        0, 0, 0, 0,
        54, 0, 0, 0,

        /* BMP 信息头 */
        40, 0, 0, 0,
        l_width.bytes[0], l_width.bytes[1], l_width.bytes[2], l_width.bytes[3],
        l_height.bytes[0], l_height.bytes[1], l_height.bytes[2], l_height.bytes[3],
        1, 0,
        24, 0,  // 24位色深

        0, 0, 0, 0,
        l_biSizeImage.bytes[0], l_biSizeImage.bytes[1], l_biSizeImage.bytes[2], l_biSizeImage.bytes[3],
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    fp = fopen(fileName, "wb+");
    if (fp == NULL) {
        printf("%s: 文件创建失败！\n", fileName);
        return -1;
    }

    fwrite(bmp_head_map, sizeof(bmp_head_map), 1, fp);

    // 图像数据生成
    uint8_t *imageData = (uint8_t *)malloc(width * height * 3);
    memset(imageData, 0, width * height * 3); // 黑色背景

    // 绘制直方图
    for (i = 0; i < 256; i++) {
        int barHeight = histogram[i]; // 每个条的高度
        if (barHeight > height) {
            barHeight = height; // 防止高度超过图像高度
        }
        for (j = 0; j < barHeight; j++) {
            // 设置图像数据，行是倒序的
            imageData[(height - 1 - j) * width * 3 + i * 3 + 0] = 255; // 蓝色通道
            imageData[(height - 1 - j) * width * 3 + i * 3 + 1] = 255; // 绿色通道
            imageData[(height - 1 - j) * width * 3 + i * 3 + 2] = 255; // 红色通道
        }
    }

    // 将图像数据写入文件，逐行写入并补齐对齐字节
    for (i = 0; i < height; i++) {
        fwrite(imageData + (height - 1 - i) * width * 3, 1, width * 3, fp);
        for (j = 0; j < (width_r - width * 3); j++) {
            fputc(0, fp); // 填充字节
        }
    }

    free(imageData);
    fclose(fp);
    printf("%s: 文件已成功生成！\n", fileName);

    return 0;
}

// 计算直方图
void calculateHistogram(const unsigned char* image, int width, int height, int* histogram) {
    for (int i = 0; i <= MAX_COLOR_VALUE; i++) {
        histogram[i] = 0;
    }

    for (int i = 0; i < width * height; i++) {
        histogram[image[i]]++;
    }
}

// 直方图均衡化
void equalizeHistogram(unsigned char* image, int width, int height) {
    int histogram[256] = {0};
    int cumulativeHistogram[256] = {0};

    for (int i = 0; i < width * height; i++) {
        histogram[image[i]]++;
    }

    cumulativeHistogram[0] = histogram[0];
    for (int i = 1; i <= MAX_COLOR_VALUE; i++) {
        cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
    }

    int totalPixels = width * height;
    for (int i = 0; i <= MAX_COLOR_VALUE; i++) {
        cumulativeHistogram[i] = (cumulativeHistogram[i] * MAX_COLOR_VALUE) / totalPixels;
    }

    for (int i = 0; i < width * height; i++) {
        image[i] = cumulativeHistogram[image[i]];
    }
}
int average(int* histogram)
{
    int max_high = 0;
    for(int i=0; i<256;i++)
    {
        if(histogram[i]>max_high) 
        max_high=histogram[i];
    }
    for(int i=0; i<256;i++)
    {
        // printf("%d\n",histogram[i]);
            // 对数缩放（+1 避免对数0的情况）
        if (histogram[i] > 0) {
            histogram[i] = (int)(log(histogram[i] + 1) / log(max_high + 1) * 100);
        }
        // 设置最小高度阈值，确保可见
        if (histogram[i] > 0 && histogram[i] < 2) {
            histogram[i] = 2; // 设置最小高度为2像素
        }
    }
    return max_high;
    // printf("\n");
}
int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <image_path>\n", argv[0]);
        return -1;
    }

    int width, height;
    unsigned char header[BMP_HEADER_SIZE];
    unsigned char* imageData = readBMP(argv[1], &width, &height, header);
    if (!imageData) {
        return -1;
    }

    int histogram[256];
    int max_high;
    calculateHistogram(imageData, width, height, histogram);
    max_high = average(histogram);
    save_histogram_as_image("original_histogram.bmp", histogram, 256,100);

    equalizeHistogram(imageData, width, height);
    saveBMP("equalized_image.bmp", header, imageData, width, height);

    calculateHistogram(imageData, width, height, histogram);
    max_high = average(histogram);
    save_histogram_as_image("equalized_histogram.bmp", histogram, 256,100);

    free(imageData);
    printf("Original and equalized histograms saved as BMP images.\n");
    printf("Equalized image saved as equalized_image.bmp.\n");
    return 0;
}
