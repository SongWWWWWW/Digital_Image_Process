
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846

#pragma pack(push, 1)
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

typedef struct {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} ClRgbQuad;

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char *imageData;
} ClImage;

typedef struct {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} ClBitMapFileHeader;

typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} ClBitMapInfoHeader;
#pragma pack(pop)

typedef struct {
    int x, y, width, height;
} Rect;


ClImage *loadImage(char *path) {
    ClImage *bmpImg = (ClImage *)malloc(sizeof(ClImage));
    FILE *pFile;
    ClBitMapFileHeader bmpFileHeader;
    ClBitMapInfoHeader bmpInfoHeader;
    ClRgbQuad *quad;
    unsigned char pixVal;
    int channels = 1, width = 0, height = 0, step = 0, offset = 0;
    int i, j, k;

    pFile = fopen(path, "rb");
    if (!pFile) {
        free(bmpImg);
        return NULL;
    }

    fread(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
    fread(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

    width = bmpInfoHeader.biWidth;
    height = bmpInfoHeader.biHeight;

    if (bmpInfoHeader.biBitCount == 8) {
        channels = 1;
        offset = (channels * width) % 4 ? 4 - (channels * width) % 4 : 0;

        bmpImg->width = width;
        bmpImg->height = height;
        bmpImg->channels = channels;
        bmpImg->imageData = (unsigned char *)malloc(sizeof(unsigned char) * width * height);
        step = channels * width;

        quad = (ClRgbQuad *)malloc(sizeof(ClRgbQuad) * 256);
        fread(quad, sizeof(ClRgbQuad), 256, pFile);
        free(quad);

        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                fread(&pixVal, sizeof(unsigned char), 1, pFile);
                bmpImg->imageData[(height - 1 - i) * step + j] = pixVal;
            }
            if (offset != 0) {
                for (j = 0; j < offset; j++) {
                    fread(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }
        }
    }

    fclose(pFile);
    return bmpImg;
}

void moveImage(ClImage *image, unsigned char *result, int xOffset, int yOffset) {
    int width = image->width;
    int height = image->height;
    int channels = image->channels;
    int lineBytes = width * channels;

    //偏移超出图像范围
    if (width < abs(xOffset) || height < abs(yOffset)) {
        fprintf(stderr, "Offset exceeds image dimensions.\n");
        return;
    }

    // 画布
    memset(result, 0, width * height * channels);

    // 移动图像
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < lineBytes; j++) {
            int newX = j + xOffset * channels;
            int newY = i + yOffset;

            // 确保目标位置在范围内
            if (newY >= 0 && newY < height && newX >= 0 && newX < lineBytes) {
                result[newY * lineBytes + newX] = image->imageData[i * lineBytes + j];
            }
        }
    }
}
void rotateImage(ClImage *image, unsigned char **rotatedImageData, int *newWidth, int *newHeight, double angle) {
    int width = image->width;
    int height = image->height;
    int channels = image->channels;
    int lineBytes = width * channels;

    double half_w = width / 2.0, half_h = height / 2.0;
    double cosa = cos(angle), sina = sin(angle);

    // 计算旋转后图像的四个顶点坐标
    double x1 = -half_w * cosa + half_h * sina, y1 = half_w * sina + half_h * cosa;
    double x2 = half_w * cosa + half_h * sina, y2 = -half_w * sina + half_h * cosa;
    double x3 = -half_w * cosa - half_h * sina, y3 = half_w * sina - half_h * cosa;
    double x4 = half_w * cosa - half_h * sina, y4 = -half_w * sina - half_h * cosa;

    // 新的宽度和高度
    *newWidth = (int)(fmax(fabs(x4 - x1), fabs(x3 - x2)) + 0.5);
    *newHeight = (int)(fmax(fabs(y4 - y1), fabs(y3 - y2)) + 0.5);

    int lineBytes_new = (*newWidth) * channels;
    *rotatedImageData = (unsigned char *)malloc((*newWidth) * (*newHeight) * channels);
    memset(*rotatedImageData, 0, (*newWidth) * (*newHeight) * channels);

    // 偏移常数
    double const1 = -(*newWidth) * cosa / 2.0 + (*newHeight) * sina / 2.0 + width / 2.0;
    double const2 = -(*newWidth) * sina / 2.0 - (*newHeight) * cosa / 2.0 + height / 2.0;

    // 遍历新图像，计算旋转后的位置
    for (int i = 0; i < *newHeight; i++) {
        for (int j = 0; j < *newWidth; j++) {
            int x0 = (int)(j * cosa - i * sina + const1 + 0.5);
            int y0 = (int)(j * sina + i * cosa + const2 + 0.5);

            if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
                (*rotatedImageData)[lineBytes_new * i + j * channels] = image->imageData[lineBytes * y0 + x0 * channels];
                (*rotatedImageData)[lineBytes_new * i + j * channels + 1] = image->imageData[lineBytes * y0 + x0 * channels + 1];
                (*rotatedImageData)[lineBytes_new * i + j * channels + 2] = image->imageData[lineBytes * y0 + x0 * channels + 2];
            }
        }
    }
}

// 水平镜像函数
void horizontalMirror(ClImage *image, unsigned char *result) {
    int width = image->width;
    int height = image->height;
    int channels = image->channels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                result[(y * width + x) * channels + c] =
                    image->imageData[(y * width + (width - 1 - x)) * channels + c];
            }
        }
    }
}

// 垂直镜像函数
void verticalMirror(ClImage *image, unsigned char *result) {
    int width = image->width;
    int height = image->height;
    int channels = image->channels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                result[(y * width + x) * channels + c] =
                    image->imageData[((height - 1 - y) * width + x) * channels + c];
            }
        }
    }
}

// 保存图像
void save_image(char *outputPath, ClImage *image, unsigned char *data) {
    int rowSize = (image->width + 3) & (~3);
    int padding = rowSize - image->width;
    ClBitMapFileHeader fileheader = {0};
    ClBitMapInfoHeader infoheader = {0};

    fileheader.bfType = 0x4d42;
    fileheader.bfSize = 54 + 256 * 4 + image->height * rowSize;
    fileheader.bfOffBits = 54 + 256 * 4;

    infoheader.biSize = 40;
    infoheader.biWidth = image->width;
    infoheader.biHeight = image->height;
    infoheader.biPlanes = 1;
    infoheader.biBitCount = 8;
    infoheader.biSizeImage = rowSize * image->height;

    FILE *file = fopen(outputPath, "wb");
    fwrite(&fileheader, sizeof(fileheader), 1, file);
    fwrite(&infoheader, sizeof(infoheader), 1, file);

    unsigned char palette[1024];
    for (int i = 0; i < 256; ++i) {
        palette[i * 4] = (unsigned char)i;
        palette[i * 4 + 1] = (unsigned char)i;
        palette[i * 4 + 2] = (unsigned char)i;
        palette[i * 4 + 3] = 0;
    }
    fwrite(palette, sizeof(unsigned char), 1024, file);

    for (int y = image->height - 1; y >= 0; --y) {
        fwrite(&data[y * image->width], 1, image->width, file);
        unsigned char paddingBytes[3] = {0};
        fwrite(paddingBytes, 1, padding, file);
    }

    fclose(file);
}

void scaleImage(ClImage *image, unsigned char **scaledImageData, int *newWidth, int *newHeight, double ratio) {
    *newWidth = (int)(image->width * ratio + 0.5);
    *newHeight = (int)(image->height * ratio + 0.5);

    int lineBytesNew = (*newWidth) * image->channels;
    *scaledImageData = (unsigned char *)malloc((*newWidth) * (*newHeight) * image->channels);
    memset(*scaledImageData, 0, (*newWidth) * (*newHeight) * image->channels);

    for (int i = 0; i < *newHeight; i++) {
        for (int j = 0; j < *newWidth; j++) {
            int x0 = (int)(j / ratio);
            int y0 = (int)(i / ratio);

            if (x0 >= 0 && x0 < image->width && y0 >= 0 && y0 < image->height) {
                for (int c = 0; c < image->channels; c++) {
                    (*scaledImageData)[i * lineBytesNew + j * image->channels + c] =
                        image->imageData[y0 * image->width * image->channels + x0 * image->channels + c];
                }
            }
        }
    }
}
int main() {
    ClImage *image = loadImage("../assets/lena.bmp");
    if (!image) {
        fprintf(stderr, "Failed to load image.\n");
        return 1;
    }

    unsigned char *movedResult = (unsigned char *)malloc(image->width * image->height * image->channels);
    if (!movedResult) {
        fprintf(stderr, "Memory allocation for moved result failed.\n");
        free(image->imageData);
        free(image);
        return 1;
    }

    int xOffset = 100; // 水平偏移
    int yOffset = 100; // 垂直偏移

    // 移动图像
    moveImage(image, movedResult, xOffset, yOffset);

    save_image("./moved_image.bmp", image, movedResult);
    printf("Image moved and saved successfully.\n");

    double angle = M_PI/ 3; 

    unsigned char *rotatedImageData = NULL;
    int newWidth, newHeight;

    // 旋转
    rotateImage(image, &rotatedImageData, &newWidth, &newHeight, angle);

    ClImage rotatedImage;
    rotatedImage.width = newWidth;
    rotatedImage.height = newHeight;
    rotatedImage.channels = image->channels;
    rotatedImage.imageData = rotatedImageData;

    save_image("./rotated_image.bmp", &rotatedImage, rotatedImageData);
    printf("Image rotated and saved successfully.\n");

    unsigned char *horizontalResult = (unsigned char *)malloc(image->width * image->height * image->channels);
    unsigned char *verticalResult = (unsigned char *)malloc(image->width * image->height * image->channels);

    if (!horizontalResult || !verticalResult) {
        fprintf(stderr, "Memory allocation for mirror results failed.\n");
        free(image->imageData);
        free(image);
        return 1;
    }

    // 水平镜像
    horizontalMirror(image, horizontalResult);
    save_image("./horizontal_mirror.bmp", image, horizontalResult);
    printf("Horizontal mirror image saved successfully.\n");

    // 垂直镜像
    verticalMirror(image, verticalResult);
    save_image("./vertical_mirror.bmp", image, verticalResult);
    printf("Vertical mirror image saved successfully.\n");

     double scaleRatio = 2.0;

    unsigned char *scaledImageData = NULL;

    scaleImage(image, &scaledImageData, &newWidth, &newHeight, scaleRatio);

    ClImage scaledImage;
    scaledImage.width = newWidth;
    scaledImage.height = newHeight;
    scaledImage.channels = image->channels;
    scaledImage.imageData = scaledImageData;

    save_image("./scaled_image.bmp", &scaledImage, scaledImageData);
    printf("Image scaled and saved successfully.\n");

    free(movedResult);
    free(image->imageData);
    free(image);

    return 0;
}

