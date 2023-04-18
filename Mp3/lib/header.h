#pragma once
#include <iostream>

class HeaderBase {
 protected:
  std::bitset<8> flags;
  uint64_t size = 0;
 public:
  void SetFlags(std::bitset<8> c);
  void SetSize(uint64_t c);

  [[nodiscard]] std::bitset<8> GetFlags() const;
  [[nodiscard]] uint64_t GetSize() const;
};

class MainHeader : public HeaderBase {
 private:
  std::string ID3_format;
  int version;
  int subversion;
 public:
  void SetFormat(std::string name);
  void SetVersion(int c);
  void SetSubversion(int c);

  [[nodiscard]] std::string GetFormat() const;
  [[nodiscard]] int GetVersion() const;
  [[nodiscard]] int GetSubversion() const;
};

class ExtendedHeader : public HeaderBase {
 private:
  int flags_bytes{};

 public:
  std::string value;

  void SetFlagsBytes(int c);

  [[nodiscard]] unsigned char GetFlagsBytes() const;

};

MainHeader MainHeaderParse(std::ifstream& fin, MainHeader& ID3_header);

ExtendedHeader ExtendedHeaderParse(std::ifstream& fin, ExtendedHeader& extended_header);

void PrintHeaderInfo(MainHeader& header);
