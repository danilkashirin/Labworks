#include <vector>

struct Color {
  float r, g, b;

  Color();
  Color(float r, float g, float b);
};

class Image {
 public:
  Image(unsigned int width, unsigned int height);
  Color GetColor(int x, int y) const;
  void SetColor(const Color &color, int x, int y);
  void Export(const char *path) const;
 private:
  unsigned int m_width;
  unsigned int m_hight;
  std::vector<Color> m_colors;
};