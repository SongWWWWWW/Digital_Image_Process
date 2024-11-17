#include <stdio.h>      // 用于标准输入输出函数，如 wprintf
#include <stdlib.h>     // 用于内存分配函数，如 malloc 和 free
#include <string.h>     // 用于 memset 函数
#include <stdint.h>     // 用于 uint8_t 类型


// #define WIDTH 256
// #define HEIGHT 256


// unsigned char* readImage(const char* path, int width, int height) {
//     FILE* file = fopen(path, "rb");
//     if (!file) {
//         perror("Cannot open file");
//         return NULL;
//     }
//     unsigned char* image = (unsigned char*)malloc(width * height);
//     fread(image, sizeof(unsigned char), width * height, file);
//     fclose(file);
//     return image;
// }


// void computeHistogram(unsigned char* image, int width, int height, int histogram[256]) {
//     for (int i = 0; i < 256; i++) {
//         histogram[i] = 0;
//     }
//     for (int i = 0; i < width * height; i++) {
//         histogram[image[i]]++;
//     }
// }


// // 保存直方图图像（BMP格式）
// void saveHistogramImage(const char* path, int histogram[256]) {
//     // 创建一个256x256的图像，存储直方图
//     unsigned char* histImage = (unsigned char*)malloc(256 * 256);  // 图像大小256x256

//     // 设置背景为白色
//     for (int i = 0; i < 256 * 256; i++) {
//         histImage[i] = 255;  // 背景为白色
//     }

//     // 将直方图显示为黑色条形
//     for (int i = 0; i < 256; i++) {
//         int height = histogram[i] > 255 ? 255 : histogram[i];  // 限制直方图高度在255以下
//         for (int y = 255; y > 255 - height; y--) {
//             histImage[y * 256 + i] = 0;  // 设置条形为黑色
//         }
//     }

//     // 构建BMP文件头
//     FILE* file = fopen(path, "wb");
//     if (!file) {
//         perror("Cannot open file to save histogram");
//         free(histImage);
//         return;
//     }

//     unsigned char bmpHeader[54] = {
//         0x42, 0x4D, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, // 文件头
//         0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, // 信息头
//         0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 信息头
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 信息头
//     };

//     // 写入文件头
//     fwrite(bmpHeader, sizeof(unsigned char), 54, file);

//     // 写入图像数据（256x256，灰度图像，1通道）
//     fwrite(histImage, sizeof(unsigned char), 256 * 256, file);

//     fclose(file);
//     free(histImage);  // 释放内存
// }

// int main() {
//     const char* imagePath = "..\\assets\\dim.bmp";
//     const char* histogramPath = "histogram_image.bmp";
//     printf("1");
    
//     unsigned char* image = readImage(imagePath, WIDTH, HEIGHT);
//     if (!image) {
//         return 1;
//     }
//     printf("2");

    
//     int histogram[256];
//     computeHistogram(image, WIDTH, HEIGHT, histogram);
//     printf("3");

    
//     saveHistogramImage(histogramPath, histogram);
//     printf("4");

//     free(image);
//     printf("保存图像为 %s\n", histogramPath);
//     printf("Histogram image saved as %s\n", histogramPath);

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 256
#define HEIGHT 256
// BMP 文件头
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
void write_bmp(const char *outputImagePath, BMPFileHeader *fileHeader, BMPInfoHeader *infoHeader, uint8_t *data, int padding) {
    FILE *file = fopen(outputImagePath, "wb");
    if (!file) {
        printf("无法打开文件进行写入\n");
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
};
// 读取图像数据
unsigned char* readImage(const char* path, int width, int height) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        perror("Cannot open file");
        return NULL;
    }
    unsigned char* image = (unsigned char*)malloc(width * height);
    fread(image, sizeof(unsigned char), width * height, file);
    fclose(file);
    return image;
}

// 计算灰度直方图
void computeHistogram(unsigned char* image, int width, int height, int histogram[256]) {
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }
    for (int i = 0; i < width * height; i++) {
        histogram[image[i]]++;
    }
}
void save_histogram_as_image(const char *outputImagePath, int histogram[256], int maxHeight) {
    int width = 600;  // 直方图的宽度
    int height = maxHeight > 0 ? maxHeight : 256;  // 确保 height 不为零

    // 为直方图图像分配内存
    uint8_t *histogramImage = (uint8_t *)malloc(width * height);
    if (!histogramImage) {
        printf("无法分配内存来保存直方图图像\n");
        return;
    }

    // 创建一个黑色背景的直方图
    memset(histogramImage, 0, width * height);

    // 根据直方图绘制图像
    for (int i = 0; i < 256; i++) {
        int barHeight = (int)((histogram[i] * (float)height) / maxHeight);
        for (int j = 0; j < barHeight; j++) {
            histogramImage[(height - 1 - j) * width + i] = 255;  // 使用白色绘制柱形图
        }
    }

    // 创建 BMP 文件头和信息头
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    fileHeader.bfType = 0x4D42;  // BMP 文件标识符
    fileHeader.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + width * height;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;

    infoHeader.biSize = sizeof(BMPInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 8;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = width * height;
    infoHeader.biClrUsed = 256;
    infoHeader.biClrImportant = 0;

    // 保存 BMP 文件
    int padding = (4 - (width % 4)) % 4;
    write_bmp(outputImagePath, &fileHeader, &infoHeader, histogramImage, padding);

    free(histogramImage);
}
// 保存直方图图像（BMP格式）
void saveHistogramImage(const char* path, int histogram[256]) {
    // 创建一个256x256的图像，存储直方图
    unsigned char* histImage = (unsigned char*)malloc(WIDTH * HEIGHT);  // 图像大小256x256

    // 设置背景为白色
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        histImage[i] = 255;  // 背景为白色
    }

    // 将直方图显示为黑色条形
    for (int i = 0; i < 256; i++) {
        int height = histogram[i] > 255 ? 255 : histogram[i];  // 限制直方图高度在255以下
        for (int y = 255; y > 255 - height; y--) {
            histImage[y * WIDTH + i] = 0;  // 设置条形为黑色
        }
    }

    // 构建BMP文件头
    FILE* file = fopen(path, "wb");
    if (!file) {
        perror("Cannot open file to save histogram");
        free(histImage);
        return;
    }

    // 文件头: BMPFileHeader
    unsigned char bmpFileHeader[14] = {
        0x42, 0x4D,           // 'BM'标志
        0x36, 0x00, 0x00, 0x00, // 文件大小（稍后更新）
        0x00, 0x00,           // 保留字节
        0x00, 0x00,           // 保留字节
        0x36, 0x00, 0x00, 0x00  // 文件头偏移量（54字节）
    };

    // 信息头: BITMAPINFOHEADER
    unsigned char bmpInfoHeader[40] = {
        0x28, 0x00, 0x00, 0x00,  // 信息头大小
        0x00, 0x00, 0x00, 0x00,  // 图像宽度（稍后更新）
        0x00, 0x00, 0x00, 0x00,  // 图像高度（稍后更新）
        0x01, 0x00,              // 颜色平面数（固定为1）
        0x08, 0x00,              // 每像素比特数（8位灰度）
        0x00, 0x00, 0x00, 0x00,  // 压缩方式（0 = 无压缩）
        0x00, 0x00, 0x00, 0x00,  // 图像大小（稍后更新）
        0x00, 0x00, 0x00, 0x00,  // 水平分辨率
        0x00, 0x00, 0x00, 0x00,  // 垂直分辨率
        0x00, 0x00, 0x00, 0x00   // 颜色索引数
    };

    // 计算文件大小（文件头大小 + 信息头大小 + 图像数据大小）
    int imageSize = WIDTH * HEIGHT;
    int fileSize = 54 + imageSize;  // 54字节为文件头和信息头大小

    // 更新文件头中的文件大小
    bmpFileHeader[2] = (unsigned char)(fileSize & 0xFF);
    bmpFileHeader[3] = (unsigned char)((fileSize >> 8) & 0xFF);
    bmpFileHeader[4] = (unsigned char)((fileSize >> 16) & 0xFF);
    bmpFileHeader[5] = (unsigned char)((fileSize >> 24) & 0xFF);

    // 更新信息头中的图像宽度和高度
    bmpInfoHeader[4] = (unsigned char)(WIDTH & 0xFF);
    bmpInfoHeader[5] = (unsigned char)((WIDTH >> 8) & 0xFF);
    bmpInfoHeader[6] = (unsigned char)((WIDTH >> 16) & 0xFF);
    bmpInfoHeader[7] = (unsigned char)((WIDTH >> 24) & 0xFF);

    bmpInfoHeader[8] = (unsigned char)(HEIGHT & 0xFF);
    bmpInfoHeader[9] = (unsigned char)((HEIGHT >> 8) & 0xFF);
    bmpInfoHeader[10] = (unsigned char)((HEIGHT >> 16) & 0xFF);
    bmpInfoHeader[11] = (unsigned char)((HEIGHT >> 24) & 0xFF);

    // 更新信息头中的图像数据大小
    bmpInfoHeader[20] = (unsigned char)(imageSize & 0xFF);
    bmpInfoHeader[21] = (unsigned char)((imageSize >> 8) & 0xFF);
    bmpInfoHeader[22] = (unsigned char)((imageSize >> 16) & 0xFF);
    bmpInfoHeader[23] = (unsigned char)((imageSize >> 24) & 0xFF);

    // 写入文件头和信息头
    fwrite(bmpFileHeader, sizeof(unsigned char), 14, file);
    fwrite(bmpInfoHeader, sizeof(unsigned char), 40, file);

    // 写入图像数据
    fwrite(histImage, sizeof(unsigned char), imageSize, file);

    fclose(file);
    free(histImage);  // 释放内存
}

int main() {
    const char* imagePath = "..\\assets\\dim.bmp";
    const char* histogramPath = "histogram_image.bmp";

    // 读取图像
    unsigned char* image = readImage(imagePath, WIDTH, HEIGHT);
    if (!image) {
        return 1;
    }

    // 计算灰度直方图
    int histogram[256];
    computeHistogram(image, WIDTH, HEIGHT, histogram);

    // 保存直方图图像
    saveHistogramImage(histogramPath, histogram);

    free(image);

    // 打印保存结果
    printf("直方图图像已保存为 %s\n", histogramPath);
    // printf("Histogram image saved as %s\n", histogramPath);

    return 0;
}
