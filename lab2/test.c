#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef union {
    uint8_t bytes[4];
    uint32_t value;
} LITTLE;

int bmp_gen_histogram(const char *fileName, int histogram[256], uint32_t width, uint32_t height) {
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
        fwrite(imageData + i * width * 3, 1, width * 3, fp);
        for (j = 0; j < (width_r - width * 3); j++) {
            fputc(0, fp); // 填充字节
        }
    }

    free(imageData);
    fclose(fp);
    printf("%s: 文件已成功生成！\n", fileName);

    return 0;
}

int main() {
    char bmpName[200] = "histogram.bmp";
    int histogram[256];
    uint32_t width = 256;  // 直方图的宽度
    uint32_t height = 100; // 图像高度

    // 示例直方图数据：随机生成或手动设置
    for (int i = 0; i < 256; i++) {
        histogram[i] = rand() % height;
    }

    int ret = bmp_gen_histogram(bmpName, histogram, width, height);
    if (ret == 0) {
        printf("直方图已保存到 %s\n", bmpName);
    }

    return 0;
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdint.h>

// #define BMP_HEADER_SIZE 54
// #define MAX_COLOR_VALUE 255
// #define HISTOGRAM_WIDTH 256
// #define HISTOGRAM_HEIGHT 256

// typedef struct {
//     uint16_t bfType;
//     uint32_t bfSize;
//     uint16_t bfReserved1;
//     uint16_t bfReserved2;
//     uint32_t bfOffBits;
// } BMPFileHeader;

// typedef struct {
//     uint32_t biSize;
//     int32_t biWidth;
//     int32_t biHeight;
//     uint16_t biPlanes;
//     uint16_t biBitCount;
//     uint32_t biCompression;
//     uint32_t biSizeImage;
//     uint32_t biClrUsed;
//     uint32_t biClrImportant;
// } BMPInfoHeader;

// void write_bmp(const char *outputImagePath, BMPFileHeader *fileHeader, BMPInfoHeader *infoHeader, uint8_t *data, int padding) {
//     FILE *file = fopen(outputImagePath, "wb");
//     if (!file) {
//         printf("Error: Unable to open file for writing\n");
//         return;
//     }
//     fwrite(fileHeader, sizeof(BMPFileHeader), 1, file);
//     fwrite(infoHeader, sizeof(BMPInfoHeader), 1, file);
//     for (int y = 0; y < infoHeader->biHeight; y++) {
//         fwrite(data + y * infoHeader->biWidth, sizeof(uint8_t), infoHeader->biWidth, file);
//         if (padding > 0) {
//             uint8_t paddingData[3] = {0, 0, 0};
//             fwrite(paddingData, 1, padding, file);
//         }
//     }
//     fclose(file);
// }

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

// void save_histogram_as_image(const char *outputImagePath, int histogram[256], int maxHeight) {
//     int width = HISTOGRAM_WIDTH;
//     int height = maxHeight > 0 ? maxHeight : HISTOGRAM_HEIGHT;
//     uint8_t *histogramImage = (uint8_t *)malloc(width * height);
//     if (!histogramImage) {
//         printf("Error: Unable to allocate memory for histogram image.\n");
//         return;
//     }
//     memset(histogramImage, 0, width * height);
//     for (int i = 0; i < 256; i++) {
//         int barHeight = (int)((histogram[i] * (float)height) / maxHeight);
//         for (int j = 0; j < barHeight; j++) {
//             histogramImage[(height - 1 - j) * width + i] = 255;
//         }
//     }
//     BMPFileHeader fileHeader = {0x4D42, BMP_HEADER_SIZE + width * height, 0, 0, BMP_HEADER_SIZE};
//     BMPInfoHeader infoHeader = {40, width, height, 1, 8, 0, width * height, 256, 0};
//     write_bmp(outputImagePath, &fileHeader, &infoHeader, histogramImage, 0);
//     free(histogramImage);
// }

// void calculateHistogram(const unsigned char* image, int width, int height, int* histogram) {
//     for (int i = 0; i <= MAX_COLOR_VALUE; i++) {
//         histogram[i] = 0;
//     }
//     for (int i = 0; i < width * height; i++) {
//         histogram[image[i]]++;
//     }
// }

// void equalizeHistogram(unsigned char* image, int width, int height) {
//     int histogram[256] = {0};
//     int cumulativeHistogram[256] = {0};
//     calculateHistogram(image, width, height, histogram);
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
//     save_histogram_as_image("original_histogram.bmp", histogram, HISTOGRAM_HEIGHT);
//     equalizeHistogram(imageData, width, height);
//     saveBMP("equalized_image.bmp", header, imageData, width, height);
//     calculateHistogram(imageData, width, height, histogram);
//     save_histogram_as_image("equalized_histogram.bmp", histogram, HISTOGRAM_HEIGHT);
//     free(imageData);
//     printf("Original and equalized histograms saved as BMP images.\n");
//     printf("Equalized image saved as equalized_image.bmp.\n");
//     return 0;
// }
