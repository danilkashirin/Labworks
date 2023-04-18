#include "lib\image.h"
#include <iostream>

int main() {
  const int width = 1000;
  const int hight = 1000;

  Image image(width, hight);

  for (int y = 0; y < hight; y++) {
	for (int x = 0; x < width; x++) {
	  image.SetColor(Color(128, rand()%255, rand()%255), x, y);
	}
  }
  image.Export(R"(C:\projects_C\lab_3_BoP\labwork-3-danilkashirin\pictures\image.bmp)");
}