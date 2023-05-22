#include <list>

#ifndef HW2_H
#define HW2_H

struct Img{
  int* srcImg;
  int width;
  int height;
  int x_c;
  int y_c;
  Img();
  Img(int w, int h);
  Img(const int* src, int w, int h);
  ~Img();
};

class ImageProcess{
  private:
      Img* srcImg;
      Img* processedImg;
      Img* mask;
  public:
      ImageProcess();
      ImageProcess(int w, int h);
      ImageProcess(const Img* img);
      ImageProcess(const char* fileName);
      ~ImageProcess();
      int updateMask(const Img& mask);
      int updateSrcImg();
      int dilatation(int srcImg = 1);
      int erosion(int srcImg = 1);
      int loadImgFromFile(const char* fileName, int format = 1);
      int saveImgToFile(const char* fileName, int format = 1);
      std::list<std::list<std::pair<int, int>>> getListContours();
};

#endif