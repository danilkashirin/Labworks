#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include "header.h"

MainHeader MainHeaderParse(std::ifstream& fin, MainHeader& ID3_header) {
  unsigned char c;
  std::string header_size_str;
  std::string format;
  for (int i = 0; i < 10; i++) {

    if (0 <= i and i <= 2) {
      c = fin.get();
      format.push_back((char) c);
    }
    ID3_header.SetFormat(format);

    if (i == 3) {
      c = fin.get();
      ID3_header.SetVersion(c);
    }

    if (i == 4) {
      c = fin.get();
      ID3_header.SetSubversion(c);
    }

    if (i == 5) {
      c = fin.get();
      ID3_header.SetFlags((std::bitset<8>) c);
    }

    if (6 <= i) {
      c = fin.get();
      header_size_str += (static_cast<std::bitset<7>> (c)).to_string();
    }
    ID3_header.SetSize((static_cast<std::bitset<7 * 4>> (header_size_str)).to_ullong());
  }
  return ID3_header;
}

ExtendedHeader ExtendedHeaderParse(std::ifstream& fin, ExtendedHeader& extended_header) { ///////
  unsigned char c;
  std::string ex_header_size_str;
  for (int i = 0; i < 6; i++) {
    if (0 <= i and i < 4) {
      c = fin.get();
      ex_header_size_str += (static_cast<std::bitset<7>> (c)).to_string();
    }
    extended_header.SetSize((static_cast<std::bitset<7 * 4>> (ex_header_size_str)).to_ullong());

    if (i == 4) {
      c = fin.get();
      extended_header.SetFlagsBytes((int) c);
    }

    if (i == 5) {
      c = fin.get();
      extended_header.SetFlags((std::bitset<8>) c);
    }
  }

  for (int i = 0; i < extended_header.GetSize() - 6; i++) {
    c = fin.get();
    extended_header.value.push_back((char) c);
  }

  return extended_header;
}

void PrintHeaderInfo(MainHeader &header) {
  std::cout << "Header\nFormat: " << header.GetFormat() << '\n' <<
            "Version: " << header.GetVersion() << '.' << header.GetSubversion() << '\n' <<
            "Size: " << header.GetSize() << '\n' <<
            "---------------------------\n";
}

void MainHeader::SetFormat(std::string name) {
  ID3_format = std::move(name);
}

void MainHeader::SetVersion(int c) {
  version = c;
}

void MainHeader::SetSubversion(int c) {
  subversion = c;
}

void HeaderBase::SetFlags(std::bitset<8> c) {
  flags = c;
}

void HeaderBase::SetSize(uint64_t c) {
  size = c;
}

std::string MainHeader::GetFormat() const {
  return ID3_format;
}

int MainHeader::GetVersion() const {
  return version;
}

int MainHeader::GetSubversion() const {
  return subversion;
}

uint64_t HeaderBase::GetSize() const {
  return size;
}

std::bitset<8> HeaderBase::GetFlags() const {
  return flags;
}

void ExtendedHeader::SetFlagsBytes(int c) {
  flags_bytes = c;
}

unsigned char ExtendedHeader::GetFlagsBytes() const {
  return flags_bytes;
}
