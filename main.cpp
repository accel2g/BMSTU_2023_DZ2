#include <iostream>
#include "hw2.h"
#include <cstring>

int main(){
  ImageProcess img(10,10);
  int maskData[] = {0,1,0,1,1,1,0,1,0};
  Img mask(maskData,3,3);
  img.loadImgFromFile("test.txt", 0);
  img.updateMask(mask);
  img.erosion();
  img.dilatation();
  img.saveImgToFile("test1.txt", 1);
  return 0;
}