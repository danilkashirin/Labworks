#pragma once
#include <iostream>
#include <fstream>
#include <utility>

class Frame {
 public:
  std::string value;
 protected:
  std::string name;
  uint64_t size = 0;
  std::bitset<8> flags_1;
  std::bitset<8> flags_2;
 public:
  Frame(const std::string& name_,
        uint64_t size_,
        std::bitset<8> flags_1_,
        std::bitset<8> flags_2_);
  Frame() = default;
  ~Frame() = default;
  void SetSize(uint64_t c);
  void SetValue(const std::string& c);
  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] uint64_t GetSize() const;
  [[nodiscard]] std::string GetValue() const;
  virtual void PrintFrame(std::ostream& stream) const;
  friend std::ostream& operator<<(std::ostream& stream, const Frame& frame);
};

class TextFrame : public Frame {
 private:
  std::string value;
  [[maybe_unused]] unsigned char encoding_byte{};
 public:
  explicit TextFrame(const Frame& frame_, const std::string& value_, unsigned char& encoding_byte_) : Frame(frame_) {
    value = value_;
    encoding_byte = encoding_byte_;
  };
  [[nodiscard]] std::string GetInfo() const;
  void PrintFrame(std::ostream& stream) const override;
};

class TxxxFrame : public Frame {
 private:
  std::string value;
  std::string description;
  [[maybe_unused]] unsigned char encoding_byte{};
 public:
  explicit TxxxFrame(const Frame& frame_,
                     const std::string& value_,
                     const std::string& description_,
                     unsigned char& encoding_byte_);
  TxxxFrame() = default;
  [[nodiscard]] std::string GetInfo() const;
  [[nodiscard]] std::string GetDescription() const;
  void PrintFrame(std::ostream& stream) const override;
};

class UfidFrame : public Frame {
 private:
  std::string owner;
  std::string identifier;
 public:
  UfidFrame() = default;
  explicit UfidFrame(const Frame& frame_, const std::string& owner_, const std::string& identifier_);
  [[nodiscard]] std::string GetOwner() const;
  [[nodiscard]] std::string GetIdentifier() const;
  void PrintFrame(std::ostream& stream) const override;
};

class UrlFrame : public Frame {
 private:
  std::string url;
 public:
  UrlFrame() = default;
  explicit UrlFrame(const Frame& frame_, const std::string& url_);
  [[nodiscard]] std::string GetUrl() const;
  void PrintFrame(std::ostream& stream) const override;
};

class WxxxFrame : public Frame {
 private:
  std::string url;
  std::string description;
  [[maybe_unused]] unsigned char encoding_byte{};
 public:
  explicit WxxxFrame(const Frame& frame_,
                     const std::string& url_,
                     const std::string& description_,
                     unsigned char& encoding_byte_);
  WxxxFrame() = default;
  [[nodiscard]] std::string GetUrl() const;
  [[nodiscard]] std::string GetDescription() const;
  void PrintFrame(std::ostream& stream) const override;
};

class EtcoFrame : public Frame {
 private:
  unsigned char time_stamp_format{};
 public:
  explicit EtcoFrame(const Frame& frame_, unsigned char time_stamp_format_);
  EtcoFrame() = default;
  [[nodiscard]] unsigned char GetStamp() const;
  void PrintFrame(std::ostream& stream) const override;
};

class UsltFrame : public Frame {
 private:
  unsigned char encoding_byte{};
  std::string language;
  std::string content_descriptor;
  std::string text;
 public:
  UsltFrame() = default;
  explicit UsltFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& language_,
                     std::string& content_descriptor_,
                     std::string& text_);
  [[nodiscard]] int GetEncodingByte() const;
  [[nodiscard]] std::string GetLanguage() const;
  [[nodiscard]] std::string GetDescriptor() const;
  [[nodiscard]] std::string GetText() const;
  void PrintFrame(std::ostream& stream) const override;
};

class SyltFrame : public Frame {
 private:
  unsigned char encoding_byte{};
  std::string language;
  unsigned char time_stamp_format{};
  unsigned char content_type{};
  std::string content_descriptor;
 public:
  explicit SyltFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& language_,
                     unsigned char time_stamp_format_,
                     unsigned char content_type_,
                     std::string& content_descriptor_);
  SyltFrame() = default;
  [[nodiscard]] int GetEncodingByte() const;
  [[nodiscard]] std::string GetLanguage() const;
  [[nodiscard]] unsigned char GetStamp() const;
  [[nodiscard]] unsigned char GetContentType() const;
  [[nodiscard]] std::string GetDescriptor() const;
  void PrintFrame(std::ostream& stream) const override;
};

class CommFrame : public Frame {
 private:
  [[maybe_unused]] unsigned char encoding_byte{};
  std::string language;
  std::string short_description;
  std::string text;
 public:
  explicit CommFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& language_,
                     std::string& short_description_,
                     std::string& text_);
  CommFrame() = default;
  [[nodiscard]] std::string GetLanguage() const;
  [[nodiscard]] std::string GetDescription() const;
  [[nodiscard]] std::string GetText() const;
  void PrintFrame(std::ostream& stream) const override;
};

class Rva2Frame : public Frame {
 private:
  std::string identification;
 public:
  explicit Rva2Frame(const Frame& frame_, const std::string& identification_);
  Rva2Frame() = default;
  [[nodiscard]] std::string GetIdentification() const;
  void PrintFrame(std::ostream& stream) const override;
};

class Equ2Frame : public Frame {
 private:
  unsigned char method{};
  std::string identification;
 public:
  explicit Equ2Frame(const Frame& frame_, unsigned char method_, const std::string& identification_);
  Equ2Frame() = default;
  [[nodiscard]] unsigned char GetMethod() const;
  [[nodiscard]] std::string GetIdentification() const;
  void PrintFrame(std::ostream& stream) const override;
};

class PcntFrame : public Frame {
 private:
  uint32_t counter = 0;
 public:
  explicit PcntFrame(const Frame& frame_, uint32_t counter_);
  PcntFrame() = default;
  [[nodiscard]] uint32_t GetCounter() const;
  void PrintFrame(std::ostream& stream) const override;
};

class PopmFrame : public Frame {
 private:
  std::string email;
  unsigned char rating{};
  uint32_t counter = 0;
 public:
  explicit PopmFrame(const Frame& frame_, const std::string& email_, unsigned char rating_, uint32_t counter_);
  PopmFrame() = default;
  [[nodiscard]] std::string GetEmail() const;
  [[nodiscard]] unsigned char GetRating() const;
  [[nodiscard]] uint32_t GetCounter() const;
  void PrintFrame(std::ostream& stream) const override;
};

class RbufFrame : public Frame {
 private:
  uint32_t buffer_size{};
  bool flag{};
  std::string offset;
 public:
  explicit RbufFrame(const Frame& frame_, uint32_t buffer_size_, bool flag_, const std::string& offset_);
  RbufFrame() = default;
  [[nodiscard]] uint32_t GetBufferSize() const;
  [[nodiscard]] bool GetFlag() const;
  [[nodiscard]] std::string GetOffset() const;
  void PrintFrame(std::ostream& stream) const override;
};

class LinkFrame : public Frame {
 private:
  uint id = 0;
  std::string url;
  std::string additional;
 public:
  LinkFrame() = default;
  explicit LinkFrame(const Frame& frame_, uint& id_, const std::string& url_, const std::string& additional_);
  [[nodiscard]] uint GetIdentifier() const;
  [[nodiscard]] std::string GetUrl() const;
  [[nodiscard]] std::string GetAdditional() const;
  void PrintFrame(std::ostream& stream) const override;
};

class PossFrame : public Frame {
 private:
  unsigned char time_stamp_format{};
  unsigned char position{};
 public:
  PossFrame() = default;
  explicit PossFrame(const Frame& frame_, unsigned char stamp_, unsigned char position_);
  [[nodiscard]] unsigned char GetStamp() const;
  [[nodiscard]] unsigned char GetPosition() const;
  void PrintFrame(std::ostream& stream) const override;
};

class UserFrame : public Frame {
 private:
  [[maybe_unused]] unsigned char encoding_byte{};
  std::string language;
  std::string text;
 public:
  explicit UserFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& language_,
                     std::string& text_);
  UserFrame() = default;
  [[nodiscard]] std::string GetLanguage() const;
  [[nodiscard]] std::string GetText() const;
  void PrintFrame(std::ostream& stream) const override;
};

class OwneFrame : public Frame {
 private:
  [[maybe_unused]] unsigned char encoding_byte{};
  std::string price;
  std::string date;
  std::string seller;
 public:
  explicit OwneFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& price_,
                     std::string& date_,
                     std::string& seller_);
  OwneFrame() = default;
  [[nodiscard]] std::string GetPrice() const;
  [[nodiscard]] std::string GetDate() const;
  [[nodiscard]] std::string GetSeller() const;
  void PrintFrame(std::ostream& stream) const override;
};

class ComrFrame : public Frame {
 private:
  [[maybe_unused]] unsigned char encoding_byte{};
  std::string price;
  std::string valid;
  std::string url;
  [[maybe_unused]] unsigned char received_byte{};
  std::string seller;
  std::string description;
  std::string picture_type;
 public:
  ComrFrame() = default;
  explicit ComrFrame(const Frame& frame_, unsigned char encoding_byte_,
                     const std::string& price_,
                     const std::string& valid_,
                     const std::string& url_,
                     unsigned char received_byte_,
                     const std::string& seller_,
                     const std::string& description_,
                     const std::string& picture_type_);
  [[nodiscard]] std::string GetPrice() const;
  [[nodiscard]] std::string GetValid() const;
  [[nodiscard]] std::string GetSeller() const;
  [[nodiscard]] std::string GetDescription() const;
  [[nodiscard]] std::string GetPictureType() const;
  [[nodiscard]] std::string GetUrl() const;
  void PrintFrame(std::ostream& stream) const override;
};

class EncrFrame : public Frame {
 private:
  std::string id;
  unsigned char method{};
  std::string data;
 public:
  EncrFrame() = default;
  explicit EncrFrame(const Frame& frame_, const std::string& id_, unsigned char method_, const std::string& data_);
  [[nodiscard]] std::string GetIdentifier() const;
  [[nodiscard]] unsigned char GetMethod() const;
  [[nodiscard]] std::string GetData() const;
  void PrintFrame(std::ostream& stream) const override;
};

class GridFrame : public Frame {
 private:
  std::string id;
  unsigned char group{};
  std::string data;
 public:
  GridFrame() = default;
  explicit GridFrame(const Frame& frame_, const std::string& id_, unsigned char group_, const std::string& data_);
  [[nodiscard]] std::string GetIdentifier() const;
  [[nodiscard]] unsigned char GetGroup() const;
  [[nodiscard]] std::string GetData() const;
  void PrintFrame(std::ostream& stream) const override;
};

class PrivFrame : public Frame {
 private:
  std::string id;
  std::string data;
 public:
  PrivFrame() = default;
  explicit PrivFrame(const Frame& frame_, const std::string& id_, const std::string& data_);
  [[nodiscard]] std::string GetIdentifier() const;
  [[nodiscard]] std::string GetData() const;
  void PrintFrame(std::ostream& stream) const override;
};

class SignFrame : public Frame {
 private:
  unsigned char group{};
  std::string signature;
 public:
  SignFrame() = default;
  explicit SignFrame(const Frame& frame_, unsigned char group_, const std::string& signature_);
  [[nodiscard]] unsigned char GetGroup() const;
  [[nodiscard]] std::string GetSignature() const;
  void PrintFrame(std::ostream& stream) const override;
};

class SeekFrame : public Frame {
 private:
  std::string min_offset;
 public:
  SeekFrame() = default;
  explicit SeekFrame(const Frame& frame_, const std::string& min_offset_);
  [[nodiscard]] std::string GetOffset() const;
  void PrintFrame(std::ostream& stream) const override;
};