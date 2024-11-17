#include <stdio.h>      // ���ڱ�׼��������������� wprintf
#include <stdlib.h>     // �����ڴ���亯������ malloc �� free
#include <string.h>     // ���� memset ����
#include <stdint.h>     // ���� uint8_t ����


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


// // ����ֱ��ͼͼ��BMP��ʽ��
// void saveHistogramImage(const char* path, int histogram[256]) {
//     // ����һ��256x256��ͼ�񣬴洢ֱ��ͼ
//     unsigned char* histImage = (unsigned char*)malloc(256 * 256);  // ͼ���С256x256

//     // ���ñ���Ϊ��ɫ
//     for (int i = 0; i < 256 * 256; i++) {
//         histImage[i] = 255;  // ����Ϊ��ɫ
//     }

//     // ��ֱ��ͼ��ʾΪ��ɫ����
//     for (int i = 0; i < 256; i++) {
//         int height = histogram[i] > 255 ? 255 : histogram[i];  // ����ֱ��ͼ�߶���255����
//         for (int y = 255; y > 255 - height; y--) {
//             histImage[y * 256 + i] = 0;  // ��������Ϊ��ɫ
//         }
//     }

//     // ����BMP�ļ�ͷ
//     FILE* file = fopen(path, "wb");
//     if (!file) {
//         perror("Cannot open file to save histogram");
//         free(histImage);
//         return;
//     }

//     unsigned char bmpHeader[54] = {
//         0x42, 0x4D, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, // �ļ�ͷ
//         0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, // ��Ϣͷ
//         0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ��Ϣͷ
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // ��Ϣͷ
//     };

//     // д���ļ�ͷ
//     fwrite(bmpHeader, sizeof(unsigned char), 54, file);

//     // д��ͼ�����ݣ�256x256���Ҷ�ͼ��1ͨ����
//     fwrite(histImage, sizeof(unsigned char), 256 * 256, file);

//     fclose(file);
//     free(histImage);  // �ͷ��ڴ�
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
//     printf("����ͼ��Ϊ %s\n", histogramPath);
//     printf("Histogram image saved as %s\n", histogramPath);

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 256
#define HEIGHT 256
// BMP �ļ�ͷ
typedef struct {
    uint16_t bfType;        // �ļ����ͣ������� 'BM'
    uint32_t bfSize;        // �ļ���С
    uint16_t bfReserved1;   // �����ֶΣ�ͨ��Ϊ 0
    uint16_t bfReserved2;   // �����ֶΣ�ͨ��Ϊ 0
    uint32_t bfOffBits;     // ���ļ�ͷ�����ݲ��ֵ�ƫ����
} BMPFileHeader;

// BMP ��Ϣͷ
typedef struct {
    uint32_t biSize;        // ��Ϣͷ�Ĵ�С
    int32_t biWidth;        // ͼ����
    int32_t biHeight;       // ͼ��߶�
    uint16_t biPlanes;      // ɫ��ƽ����������Ϊ 1
    uint16_t biBitCount;    // ÿ�����ص�λ����8 λ��256 ɫ��
    uint32_t biCompression; // ѹ����ʽ��0 ��ʾ��ѹ����
    uint32_t biSizeImage;   // ͼ�����ݵĴ�С
    uint32_t biClrUsed;     // ʹ�õ���ɫ��
    uint32_t biClrImportant; // ��Ҫ����ɫ��
} BMPInfoHeader;
void write_bmp(const char *outputImagePath, BMPFileHeader *fileHeader, BMPInfoHeader *infoHeader, uint8_t *data, int padding) {
    FILE *file = fopen(outputImagePath, "wb");
    if (!file) {
        printf("�޷����ļ�����д��\n");
        return;
    }

    // д���ļ�ͷ����Ϣͷ
    fwrite(fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(infoHeader, sizeof(BMPInfoHeader), 1, file);

    // д��ͼ�����ݣ����ǵ� padding
    for (int y = 0; y < infoHeader->biHeight; y++) {
        fwrite(data + y * infoHeader->biWidth, sizeof(uint8_t), infoHeader->biWidth, file);
        if (padding > 0) {
            uint8_t paddingData[3] = {0, 0, 0};
            fwrite(paddingData, 1, padding, file);
        }
    }

    fclose(file);
};
// ��ȡͼ������
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

// ����Ҷ�ֱ��ͼ
void computeHistogram(unsigned char* image, int width, int height, int histogram[256]) {
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }
    for (int i = 0; i < width * height; i++) {
        histogram[image[i]]++;
    }
}
void save_histogram_as_image(const char *outputImagePath, int histogram[256], int maxHeight) {
    int width = 600;  // ֱ��ͼ�Ŀ��
    int height = maxHeight > 0 ? maxHeight : 256;  // ȷ�� height ��Ϊ��

    // Ϊֱ��ͼͼ������ڴ�
    uint8_t *histogramImage = (uint8_t *)malloc(width * height);
    if (!histogramImage) {
        printf("�޷������ڴ�������ֱ��ͼͼ��\n");
        return;
    }

    // ����һ����ɫ������ֱ��ͼ
    memset(histogramImage, 0, width * height);

    // ����ֱ��ͼ����ͼ��
    for (int i = 0; i < 256; i++) {
        int barHeight = (int)((histogram[i] * (float)height) / maxHeight);
        for (int j = 0; j < barHeight; j++) {
            histogramImage[(height - 1 - j) * width + i] = 255;  // ʹ�ð�ɫ��������ͼ
        }
    }

    // ���� BMP �ļ�ͷ����Ϣͷ
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    fileHeader.bfType = 0x4D42;  // BMP �ļ���ʶ��
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

    // ���� BMP �ļ�
    int padding = (4 - (width % 4)) % 4;
    write_bmp(outputImagePath, &fileHeader, &infoHeader, histogramImage, padding);

    free(histogramImage);
}
// ����ֱ��ͼͼ��BMP��ʽ��
void saveHistogramImage(const char* path, int histogram[256]) {
    // ����һ��256x256��ͼ�񣬴洢ֱ��ͼ
    unsigned char* histImage = (unsigned char*)malloc(WIDTH * HEIGHT);  // ͼ���С256x256

    // ���ñ���Ϊ��ɫ
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        histImage[i] = 255;  // ����Ϊ��ɫ
    }

    // ��ֱ��ͼ��ʾΪ��ɫ����
    for (int i = 0; i < 256; i++) {
        int height = histogram[i] > 255 ? 255 : histogram[i];  // ����ֱ��ͼ�߶���255����
        for (int y = 255; y > 255 - height; y--) {
            histImage[y * WIDTH + i] = 0;  // ��������Ϊ��ɫ
        }
    }

    // ����BMP�ļ�ͷ
    FILE* file = fopen(path, "wb");
    if (!file) {
        perror("Cannot open file to save histogram");
        free(histImage);
        return;
    }

    // �ļ�ͷ: BMPFileHeader
    unsigned char bmpFileHeader[14] = {
        0x42, 0x4D,           // 'BM'��־
        0x36, 0x00, 0x00, 0x00, // �ļ���С���Ժ���£�
        0x00, 0x00,           // �����ֽ�
        0x00, 0x00,           // �����ֽ�
        0x36, 0x00, 0x00, 0x00  // �ļ�ͷƫ������54�ֽڣ�
    };

    // ��Ϣͷ: BITMAPINFOHEADER
    unsigned char bmpInfoHeader[40] = {
        0x28, 0x00, 0x00, 0x00,  // ��Ϣͷ��С
        0x00, 0x00, 0x00, 0x00,  // ͼ���ȣ��Ժ���£�
        0x00, 0x00, 0x00, 0x00,  // ͼ��߶ȣ��Ժ���£�
        0x01, 0x00,              // ��ɫƽ�������̶�Ϊ1��
        0x08, 0x00,              // ÿ���ر�������8λ�Ҷȣ�
        0x00, 0x00, 0x00, 0x00,  // ѹ����ʽ��0 = ��ѹ����
        0x00, 0x00, 0x00, 0x00,  // ͼ���С���Ժ���£�
        0x00, 0x00, 0x00, 0x00,  // ˮƽ�ֱ���
        0x00, 0x00, 0x00, 0x00,  // ��ֱ�ֱ���
        0x00, 0x00, 0x00, 0x00   // ��ɫ������
    };

    // �����ļ���С���ļ�ͷ��С + ��Ϣͷ��С + ͼ�����ݴ�С��
    int imageSize = WIDTH * HEIGHT;
    int fileSize = 54 + imageSize;  // 54�ֽ�Ϊ�ļ�ͷ����Ϣͷ��С

    // �����ļ�ͷ�е��ļ���С
    bmpFileHeader[2] = (unsigned char)(fileSize & 0xFF);
    bmpFileHeader[3] = (unsigned char)((fileSize >> 8) & 0xFF);
    bmpFileHeader[4] = (unsigned char)((fileSize >> 16) & 0xFF);
    bmpFileHeader[5] = (unsigned char)((fileSize >> 24) & 0xFF);

    // ������Ϣͷ�е�ͼ���Ⱥ͸߶�
    bmpInfoHeader[4] = (unsigned char)(WIDTH & 0xFF);
    bmpInfoHeader[5] = (unsigned char)((WIDTH >> 8) & 0xFF);
    bmpInfoHeader[6] = (unsigned char)((WIDTH >> 16) & 0xFF);
    bmpInfoHeader[7] = (unsigned char)((WIDTH >> 24) & 0xFF);

    bmpInfoHeader[8] = (unsigned char)(HEIGHT & 0xFF);
    bmpInfoHeader[9] = (unsigned char)((HEIGHT >> 8) & 0xFF);
    bmpInfoHeader[10] = (unsigned char)((HEIGHT >> 16) & 0xFF);
    bmpInfoHeader[11] = (unsigned char)((HEIGHT >> 24) & 0xFF);

    // ������Ϣͷ�е�ͼ�����ݴ�С
    bmpInfoHeader[20] = (unsigned char)(imageSize & 0xFF);
    bmpInfoHeader[21] = (unsigned char)((imageSize >> 8) & 0xFF);
    bmpInfoHeader[22] = (unsigned char)((imageSize >> 16) & 0xFF);
    bmpInfoHeader[23] = (unsigned char)((imageSize >> 24) & 0xFF);

    // д���ļ�ͷ����Ϣͷ
    fwrite(bmpFileHeader, sizeof(unsigned char), 14, file);
    fwrite(bmpInfoHeader, sizeof(unsigned char), 40, file);

    // д��ͼ������
    fwrite(histImage, sizeof(unsigned char), imageSize, file);

    fclose(file);
    free(histImage);  // �ͷ��ڴ�
}

int main() {
    const char* imagePath = "..\\assets\\dim.bmp";
    const char* histogramPath = "histogram_image.bmp";

    // ��ȡͼ��
    unsigned char* image = readImage(imagePath, WIDTH, HEIGHT);
    if (!image) {
        return 1;
    }

    // ����Ҷ�ֱ��ͼ
    int histogram[256];
    computeHistogram(image, WIDTH, HEIGHT, histogram);

    // ����ֱ��ͼͼ��
    saveHistogramImage(histogramPath, histogram);

    free(image);

    // ��ӡ������
    printf("ֱ��ͼͼ���ѱ���Ϊ %s\n", histogramPath);
    // printf("Histogram image saved as %s\n", histogramPath);

    return 0;
}
