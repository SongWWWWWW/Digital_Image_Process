#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1) // 防止编译器对结构体进行字节对�?

// BMP文件头结�?
typedef struct
{
    unsigned short bfType;      // 文件类型，必须为"BM"�?0x4D42�?
    unsigned int bfSize;        // 文件大小
    unsigned short bfReserved1; // 保留，必须设置为0
    unsigned short bfReserved2; // 保留，必须设置为0
    unsigned int bfOffBits;     // 图像数据的偏移量
} BMPFileHeader;

// BMP信息头结�?
typedef struct
{
    unsigned int biSize;         // 信息头大�?
    int biWidth;                 // 图像宽度
    int biHeight;                // 图像高度
    unsigned short biPlanes;     // 颜色平面数，必须�?1
    unsigned short biBitCount;   // 每像素位数（24位代表RGB�?
    unsigned int biCompression;  // 压缩类型�?0表示不压缩）
    unsigned int biSizeImage;    // 图像数据大小
    int biXPelsPerMeter;         // 水平分辨�?
    int biYPelsPerMeter;         // 垂直分辨�?
    unsigned int biClrUsed;      // 实际使用的颜色数
    unsigned int biClrImportant; // 重要颜色�?
} BMPInfoHeader;

#pragma pack(pop)

// �?24位RGB转换�?8位灰�?
void rgb_to_gray(unsigned char *input, unsigned char *output, int width, int height)
{
    for (int i = 0; i < width * height; i++)
    {
        unsigned char b = input[i * 3];
        unsigned char g = input[i * 3 + 1];
        unsigned char r = input[i * 3 + 2];
        unsigned char gray = (unsigned char)((299 * r + 587 * g + 114 * b) / 1000);
        output[i] = gray;
    }
}

// 24位转8位灰度图
int transfer24BitFile(char *inputPath, char *outputPath)
{
    // 读取文件，写入文件的路径
    FILE *inputFile = fopen(inputPath, "rb");
    FILE *outputFile = fopen(outputPath, "wb");

    // 如果inputFile或outputFile为空，则返回错误
    if (inputPath == NULL || inputFile == NULL || outputFile == NULL)
    {
        printf("无法打开文件\n");
        return 1;
    }

    // 读取BMP文件头和信息�?
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    fread(&fileHeader, sizeof(BMPFileHeader), 1, inputFile);
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, inputFile);

    // 获取图像的宽度和高度，并计算每行字节�?
    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    int inputPadding = (4 - (width * 3) % 4) % 4;
    int inputRowSize = width * 3 + inputPadding;

    unsigned char *inputImage = (unsigned char *)malloc(inputRowSize * height);
    unsigned char *grayImage = (unsigned char *)malloc(width * height);

    fseek(inputFile, fileHeader.bfOffBits, SEEK_SET);
    fread(inputImage, sizeof(unsigned char), inputRowSize * height, inputFile);

    rgb_to_gray(inputImage, grayImage, width, height);

    fclose(inputFile);

    BMPFileHeader outputFileHeader = fileHeader;
    BMPInfoHeader outputInfoHeader = infoHeader;

    outputInfoHeader.biBitCount = 8;
    outputInfoHeader.biCompression = 0;
    int outputPadding = (4 - (width * 1) % 4) % 4;
    outputInfoHeader.biSizeImage = (width + outputPadding) * height;
    outputInfoHeader.biClrUsed = 256;
    outputInfoHeader.biClrImportant = 256;

    outputFileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 256 * 4;
    outputFileHeader.bfSize = outputFileHeader.bfOffBits + outputInfoHeader.biSizeImage;

    // 写入 BMP 文件头和信息�?
    fwrite(&outputFileHeader, sizeof(BMPFileHeader), 1, outputFile);
    fwrite(&outputInfoHeader, sizeof(BMPInfoHeader), 1, outputFile);

    // 写入灰度调色�?
    unsigned char colorTable[256 * 4];
    for (int i = 0; i < 256; i++)
    {
        colorTable[i * 4] = i;     // 蓝色
        colorTable[i * 4 + 1] = i; // 绿色
        colorTable[i * 4 + 2] = i; // 红色
        colorTable[i * 4 + 3] = 0; // 保留字节
    }
    fwrite(colorTable, sizeof(unsigned char), 256 * 4, outputFile);

    // 写入灰度图像数据
    for (int i = 0; i < height; i++)
    {
        int rowIndex;
        if (infoHeader.biHeight > 0)
        { // 从下到上
            rowIndex = height - 1 - i;
        }
        else
        { // 从上到下
            rowIndex = i;
        }
        fwrite(grayImage + rowIndex * width, sizeof(unsigned char), width, outputFile);
        unsigned char paddingBytes[3] = {0, 0, 0};
        fwrite(paddingBytes, sizeof(unsigned char), outputPadding, outputFile);
    }

    free(inputImage);
    free(grayImage);
    fclose(outputFile);

    printf("ͼ��ɹ����浽 output.bmp\n");
    return 0;
}

// 

int main()
{
    transfer24BitFile("../assets/rgb.bmp", "./result/output.bmp");
}
