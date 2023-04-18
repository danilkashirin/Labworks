#include "image.h"
#include <iostream>
#include <fstream>

Color::Color()
	: r(0), g(0), b(0) {
}

Color::Color(float r, float g, float b)
	: r(r), g(g), b(b) {
}

Image::Image(unsigned int width, unsigned int height)
	: m_width(width), m_hight(height), m_colors(std::vector<Color>(width * height)) {
}

Color Image::GetColor(int x, int y) const {
  return m_colors[y * m_width + x];
}

void Image::SetColor(const Color &color, int x, int y) {
  m_colors[y * m_width + x].r = 256 - color.r;
  m_colors[y * m_width + x].g = 256 - color.g;
  m_colors[y * m_width + x].b = 256 - color.b;
}

void Image::Export(const char *path) const {
  std::ofstream f;
  f.open(path, std::ios::out | std::ios::binary);

  if (!f.is_open()) {
	std::cout << "File not open.\n";
	return;
  }
  unsigned char bmp_pad[3] = {0, 0, 0};
  const unsigned int padding_amount = (4 - (m_width * 3) % 4) % 4;
  const int file_header_size = 14;
  const int information_header_size = 40;
  const int file_size = file_header_size + information_header_size + m_width * m_hight * 3 + padding_amount * m_hight;

  unsigned char file_header[file_header_size];
  //file type
  file_header[0] = 'B';
  file_header[1] = 'M';

  //file size
  file_header[2] = file_size;
  file_header[3] = file_size >> 8;
  file_header[4] = file_size >> 16;
  file_header[5] = file_size >> 24;

  //reserved 1 and reserved 2 (not used)
  file_header[6] = 0; //res 1
  file_header[7] = 0; //res 1
  file_header[8] = 0; //res 2
  file_header[9] = 0; //res 2

  //pixel data offset
  file_header[10] = file_header_size + information_header_size;
  file_header[11] = 0;
  file_header[12] = 0;
  file_header[13] = 0;

  unsigned char information_header[information_header_size] = {
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  //header size
  information_header[0] = information_header_size;

  //image width
  information_header[4] = m_width;
  information_header[5] = m_width >> 8;
  information_header[6] = m_width >> 16;
  information_header[7] = m_width >> 24;

  //image height
  information_header[8] = m_hight;
  information_header[9] = m_hight >> 8;
  information_header[10] = m_hight >> 16;
  information_header[11] = m_hight >> 24;

  //planes
  information_header[12] = 1;

  //bits per pixel (RGB)
  information_header[14] = 8 * 3;

  /* compression (no compression) i: 16 - 19
   * image size (no compression) i: 20 - 23
   * x pixels per meter (not specified) i: 24 - 27
   * y pixels per meter (not specified) i: 28 - 31
   * total colors (palette not used) i: 32 - 35
   * important colors (generally ignored) i: 36 - 39
   */


  f.write(reinterpret_cast<char *>(file_header), file_header_size);
  f.write(reinterpret_cast<char *>(information_header), information_header_size);

  for (int y = 0; y < m_hight; y++) {
	for (int x = 0; x < m_width; x++) {
	  unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
	  unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
	  unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

	  unsigned char color[] = {b, g, r};
	  f.write(reinterpret_cast<char *>(color), 3);
	}
	f.write(reinterpret_cast<char *>(bmp_pad), padding_amount);
  }

  f.close();
}
