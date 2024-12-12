#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma pack(1)

typedef struct
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;

void readBMP(const char *filePath, unsigned char **header, unsigned char **data, int *width, int *height, int *stride)
{
    FILE *file = fopen(filePath, "rb");
    if (!file)
    {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    BITMAPFILEHEADER fileHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    if (fileHeader.bfType != 0x4D42)
    {
        printf("Not a valid BMP file.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    BITMAPINFOHEADER infoHeader;
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    *width = infoHeader.biWidth;
    *height = infoHeader.biHeight;
    // 计算填充
    int padding = (*width * 3) % 4 == 0 ? 0 : 4 - (*width * 3) % 4;
    // 计算行字节数
    *stride = (*width * 3) + padding;

    *header = (unsigned char *)malloc(fileHeader.bfOffBits);
    fseek(file, 0, SEEK_SET);
    fread(*header, 1, fileHeader.bfOffBits, file);

    *data = (unsigned char *)malloc(infoHeader.biSizeImage);
    fread(*data, 1, infoHeader.biSizeImage, file);
    fclose(file);
}

void writeBMP(const char *filePath, unsigned char *header, unsigned char *data, int width, int height, int stride)
{
    FILE *file = fopen(filePath, "wb");
    if (!file)
    {
        perror("File writing failed");
        exit(EXIT_FAILURE);
    }

    BITMAPFILEHEADER *fileHeader = (BITMAPFILEHEADER *)header;
    BITMAPINFOHEADER *infoHeader = (BITMAPINFOHEADER *)(header + sizeof(BITMAPFILEHEADER));

    fileHeader->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + stride * height;
    infoHeader->biWidth = width;
    infoHeader->biHeight = height;
    infoHeader->biSizeImage = stride * height;

    fwrite(header, 1, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), file);
    fwrite(data, 1, stride * height, file);
    fclose(file);
}

unsigned char *scale(unsigned char *data, int width, int height, int stride, float scaleX, float scaleY, int *newWidth, int *newHeight)
{
    *newWidth = width * scaleX;
    *newHeight = height * scaleY;
    int newStride = (*newWidth * 3 + 3) & ~3;

    unsigned char *newData = (unsigned char *)calloc(newStride * (*newHeight), 1);

    for (int y = 0; y < *newHeight; y++)
    {
        for (int x = 0; x < *newWidth; x++)
        {
            int oldX = x / scaleX;
            int oldY = y / scaleY;
            for (int c = 0; c < 3; c++)
            {
                newData[y * newStride + x * 3 + c] = data[oldY * stride + oldX * 3 + c];
            }
        }
    }

    return newData;
}

unsigned char *mirror(unsigned char *data, int width, int height, int stride)
{
    unsigned char *newData = (unsigned char *)malloc(height * width);
    int temp;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            data[x][y] = data[x][width - j - 1];
        }
    }
    return newData;
}

unsigned char *rotate(unsigned char *data, int width, int height, int stride, int *newWidth, int *newHeight)
{
    *newWidth = height;
    *newHeight = width;
    int newStride = (*newWidth * 3 + 3) & ~3;

    unsigned char *newData = (unsigned char *)calloc(newStride * (*newHeight), 1);
    for (int y = 0; y < *newHeight; y++)
    {
        for (int x = 0; x < *newWidth; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                newData[y * newStride + x * 3 + c] = data[(height - x - 1) * stride + y * 3 + c];
            }
        }
    }
    return newData;
}

void processAndSaveImages(unsigned char *header, unsigned char *data, int width, int height, int stride)
{
    // 1. 缩放
    float scaleX = 1.5, scaleY = 1.5;
    int newWidth, newHeight;
    unsigned char *scaledData = scale(data, width, height, stride, scaleX, scaleY, &newWidth, &newHeight);
    int newStride = (newWidth * 3 + 3) & ~3;
    BITMAPINFOHEADER *infoHeader = (BITMAPINFOHEADER *)(header + sizeof(BITMAPFILEHEADER));
    infoHeader->biWidth = newWidth;
    infoHeader->biHeight = newHeight;
    infoHeader->biSizeImage = newStride * newHeight;
    writeBMP("output_scale.bmp", header, scaledData, newWidth, newHeight, newStride);
    free(scaledData);

    // 2. 镜像
    unsigned char *mirroredData = mirror(data, width, height, stride);
    infoHeader->biWidth = width;
    infoHeader->biHeight = height;
    infoHeader->biSizeImage = stride * height;
    writeBMP("output_mirror.bmp", header, mirroredData, width, height, stride);
    free(mirroredData);

    // 3. 旋转
    unsigned char *rotatedData = rotate(data, width, height, stride, &newWidth, &newHeight);
    newStride = (newWidth * 3 + 3) & ~3;
    infoHeader->biWidth = newWidth;
    infoHeader->biHeight = newHeight;
    infoHeader->biSizeImage = newStride * newHeight;
    writeBMP("output_rotate.bmp", header, rotatedData, newWidth, newHeight, newStride);
    free(rotatedData);
}

int main()
{
    unsigned char *header, *data;
    int width, height, stride;
    readBMP("../assets/dim.bmp", &header, &data, &width, &height, &stride);

    // 执行所有图像处理
    processAndSaveImages(header, data, width, height, stride);

    // 释放内存
    free(header);
    free(data);

    printf("All operations completed. Output saved to respective files.\n");
    return 0;
}
