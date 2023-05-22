#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include "hw2.h"

Img::Img() {
    width = 0;
    height = 0;
    x_c = 0;
    y_c = 0;
    srcImg = new int[width*height];
    for (int i = 0; i < width*height; ++i) {
        srcImg[i] = 0;
    }
}

Img::Img(int w, int h) {
    width = w;
    height = h;
    x_c = w >> 1;
    y_c = h >> 1;
    srcImg = new int[width*height];
    for (int i = 0; i < width*height; ++i) {
        srcImg[i] = 0;
    }
}

Img::Img(const int* src, int w, int h){
    width = w;
    height = h;
    x_c = w >> 1;
    y_c = h >> 1;
    srcImg = new int[width*height];
    for (int i = 0; i < width*height; ++i) {
        srcImg[i] = src[i];
    }
}

Img::~Img() {
    delete[] srcImg;
}

ImageProcess::ImageProcess() {
    srcImg = new Img;
    processedImg = nullptr;
    mask = nullptr;
}

ImageProcess::ImageProcess(int w, int h) {
    srcImg = new Img(w,h);
    processedImg = nullptr;
    mask = new Img;
}

ImageProcess::ImageProcess(const Img *img) {
    srcImg = new Img(img->srcImg, img->width,img->height);
    processedImg = nullptr;
    mask = new Img;
} 

ImageProcess::ImageProcess(const char *fileName) {
    std::ifstream file;
    file.open(fileName, std::ios::in);
    if (!file) {
        std::cerr << "Error: can't open file " << fileName << "\n";
        srcImg = new Img;
        processedImg = nullptr;
        mask = nullptr;
        return;
    }
    int w, h;
    file >> w >> h;
    srcImg = new Img(w, h);
    processedImg = nullptr;
    mask = new Img;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int pixel;
            file >> pixel;
            if (pixel == 1) {
                srcImg->srcImg[i*w+j] = 255;
            } else {
                srcImg->srcImg[i*w+j] = 0;
            }
        }
    }
    file.close();
}

ImageProcess::~ImageProcess() {
    delete srcImg;
    delete processedImg;
    delete mask;
}

int ImageProcess::updateMask(const Img &mask) {
    if (this->mask == nullptr) {
        this->mask = new Img(mask.srcImg, mask.width, mask.height);
    } else {
        delete this->mask;
        this->mask = new Img(mask.srcImg, mask.width, mask.height);
    }
    return 0;
}

int ImageProcess::updateSrcImg() {
    if (processedImg == nullptr) {
        processedImg = new Img(srcImg->srcImg, srcImg->width, srcImg->height);
    } else {
        delete srcImg;
        srcImg = new Img(processedImg->srcImg, processedImg->width, processedImg->height);
    }
    return 0;
}

int ImageProcess::dilatation(int srcImg) {
    if (mask == nullptr) {
        std::cerr << "Error: Mask is not defined!\n";
        return -1;
    }
    Img* img;
    if (srcImg == 1) {
        img = this->srcImg;
    } else {
        img = processedImg;
    }
    Img* result = new Img(img->width, img->height);
    int maskSize = mask->width;
    int shift = maskSize >> 1;
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            int mx = 0;
            for (int k = -shift; k <= shift; ++k) {
                for (int l = -shift; l <= shift; ++l) {
                    int x = j + l;
                    int y = i + k;
                    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
                        continue;
                    }
                    if (mask->srcImg[(k+shift)*maskSize + l+shift] == 1 && img->srcImg[y*img->width + x] > mx) {
                        mx = img->srcImg[y*img->width + x];
                    }
                }
            }
            result->srcImg[i*img->width + j] = mx;
        }
    }
    if (srcImg == 1) {
        delete this->processedImg;
        this->processedImg = result;
    } else {
        delete this->srcImg;
        this->srcImg = result;
    }
    return 0;
}

int ImageProcess::erosion(int srcImg) {
    if (mask == nullptr) {
        std::cerr << "Error: Mask is not defined!\n";
        return -1;
    }
    Img* img;
    if (srcImg == 1) {
        img = this->srcImg;
    } else {
        img = processedImg;
    }
    Img* result = new Img(img->width, img->height);
    int maskSize = mask->width;
    int shift = maskSize >> 1;
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            int mn = 256;
            for (int k = -shift; k <= shift; ++k) {
                for (int l = -shift; l <= shift; ++l) {
                    int x = j + l;
                    int y = i + k;
                    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
                        continue;
                    }
                    if (mask->srcImg[(k+shift)*maskSize + l+shift] == 1 && img->srcImg[y*img->width + x] < mn) {
                        mn = img->srcImg[y*img->width + x];
                    }
                }
            }
            result->srcImg[i*img->width + j] = mn;
        }
    }
    if (srcImg == 1) {
        delete this->processedImg;
        this->processedImg = result;
    } else {
        delete this->srcImg;
        this->srcImg = result;
    }
    return 0;
}

int ImageProcess::loadImgFromFile(const char *fileName, int format) {
    std::ifstream file;
    file.open(fileName, std::ios::in);
    if (!file) {
        std::cerr << "Error: can't open file " << fileName << "\n";
        return -1;
    }
    int w, h;
    file >> w >> h;
    Img* img = new Img(w, h);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int pixel;
            if (format == 1) {
                file >> pixel;
            } else {
                char c;
                file >> c;
                pixel = (int)(c - '0');
            }
            if (pixel == 1) {
                img->srcImg[i*w+j] = 255;
            } else {
                img->srcImg[i*w+j] = 0;
            }
        }
    }
    file.close();
    delete srcImg;
    srcImg = img;
    return 0;
}

int ImageProcess::saveImgToFile(const char *fileName, int format) {
    std::ofstream file;
    file.open(fileName, std::ios::out);
    if (!file) {
        std::cerr << "Error: can't open file " << fileName << "\n";
        return -1;
    }
    Img* img = (processedImg != nullptr) ? processedImg : srcImg;
    file << img->width << " " << img->height << "\n";
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            if (format == 1) {
                file << img->srcImg[i*img->width+j]/255 << " ";
            } else {
                file << (img->srcImg[i*img->width+j]/255);
            }
        }
        if (format == 1) {
            file << "\n";
        }
    }
    file.close();
    return 0;
}