#include <iostream>
#include <utility>
#include <vector>
#include "argument_parser.h"
#include "frame_base.h"

///Frame Base
Frame::Frame(const std::string& name_,
             uint64_t size_,
             std::bitset<8> flags_1_,
             std::bitset<8> flags_2_) {
  name = name_;
  size = size_;
  flags_1 = flags_1_;
  flags_2 = flags_2_;
}
void Frame::SetSize(uint64_t c) {
  size = c;
}
void Frame::SetValue(const std::string& c) {
  value = c;
}

std::string Frame::GetName() const {
  return name;
}
uint64_t Frame::GetSize() const {
  return size;
}
std::string Frame::GetValue() const {
  return value;
}
void Frame::PrintFrame(std::ostream& stream) const {}

std::ostream& operator<<(std::ostream& stream, const Frame& frame) {
  frame.PrintFrame(stream);
  return stream;
}

///TEXT Frame
std::string TextFrame::GetInfo() const {
  return value;
}
void TextFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Information: " << GetInfo() << '\n'
            << "---------------------------\n";
}

///TXXX Frame
TxxxFrame::TxxxFrame(const Frame& frame_,
                     const std::string& value_,
                     const std::string& description_,
                     unsigned char& encoding_byte_) : Frame(frame_) {
  value = value_;
  description = description_;
  encoding_byte = encoding_byte_;
}
std::string TxxxFrame::GetInfo() const {
  return value;
}
std::string TxxxFrame::GetDescription() const {
  return description;
}
void TxxxFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Description: " << GetDescription() << '\n'
            << "Value: " << GetInfo() << '\n'
            << "---------------------------\n";
}

///UFID Frame
UfidFrame::UfidFrame(const Frame& frame_, const std::string& owner_, const std::string& identifier_) : Frame(frame_) {
  owner = owner_;
  identifier = identifier_;
}
std::string UfidFrame::GetOwner() const {
  return owner;
}
std::string UfidFrame::GetIdentifier() const {
  return identifier;
}
void UfidFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Owner identifier: " << GetOwner() << '\n'
            << "Identifier " << GetIdentifier() << '\n'
            << "---------------------------\n";
}

///URL Frame
UrlFrame::UrlFrame(const Frame& frame_, const std::string& url_) : Frame(frame_) {
  url = url_;
}
std::string UrlFrame::GetUrl() const {
  return url;
}
void UrlFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "URL: " << GetUrl() << '\n'
            << "---------------------------\n";
}

///WXXX Frame
WxxxFrame::WxxxFrame(const Frame& frame_,
                     const std::string& url_,
                     const std::string& description_,
                     unsigned char& encoding_byte_) : Frame(frame_) {
  url = url_;
  description = description_;
  encoding_byte = encoding_byte_;
}
std::string WxxxFrame::GetUrl() const {
  return url;
}
std::string WxxxFrame::GetDescription() const {
  return description;
}
void WxxxFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Description: " << GetDescription() << '\n'
            << "Value: " << GetUrl() << '\n'
            << "---------------------------\n";
}

///ETCO Frame
EtcoFrame::EtcoFrame(const Frame& frame_, unsigned char time_stamp_format_) : Frame(frame_) {
  time_stamp_format = time_stamp_format_;
}
unsigned char EtcoFrame::GetStamp() const {
  return time_stamp_format;
}
void EtcoFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Time stamp format: " << GetStamp() << '\n'
            << "---------------------------\n";
}

///USLT Frame
UsltFrame::UsltFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& language_,
                     std::string& content_descriptor_,
                     std::string& text_) : Frame(frame_) {
  encoding_byte = encoding_byte_;
  language = language_;
  content_descriptor = content_descriptor_;
  text = text_;
}
int UsltFrame::GetEncodingByte() const {
  return (int) encoding_byte;
}
std::string UsltFrame::GetLanguage() const {
  return language;
}
std::string UsltFrame::GetDescriptor() const {
  return content_descriptor;
}
std::string UsltFrame::GetText() const {
  return text;
}
void UsltFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Text Encoding " << GetEncodingByte() << '\n'
            << "Language: " << GetLanguage() << '\n'
            << "Content descriptor: " << GetDescriptor() << '\n'
            << "Lyrics/text: " << GetText() << '\n'
            << "---------------------------\n";
}

///SYLT Frame
SyltFrame::SyltFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& language_,
                     unsigned char time_stamp_format_,
                     unsigned char content_type_,
                     std::string& content_descriptor_) : Frame(frame_) {
  encoding_byte = encoding_byte_;
  language = language_;
  time_stamp_format = time_stamp_format_;
  content_type = content_type_;
  content_descriptor = content_descriptor_;
}
int SyltFrame::GetEncodingByte() const {
  return (int) encoding_byte;
}
std::string SyltFrame::GetLanguage() const {
  return language;
}
unsigned char SyltFrame::GetStamp() const {
  return time_stamp_format;
}
unsigned char SyltFrame::GetContentType() const {
  return content_type;
}
std::string SyltFrame::GetDescriptor() const {
  return content_descriptor;
}
void SyltFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Text Encoding " << GetEncodingByte() << '\n'
            << "Language: " << GetLanguage() << '\n'
            << "Time stamp format: " << GetStamp() << '\n'
            << "Content type: " << GetContentType() << '\n'
            << "Content descriptor: " << GetDescriptor() << '\n'
            << "---------------------------\n";
}

///COMM Frame
CommFrame::CommFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& language_,
                     std::string& short_description_,
                     std::string& text_) : Frame(frame_) {
  encoding_byte = encoding_byte_;
  language = language_;
  short_description = short_description_;
  text = text_;
}
std::string CommFrame::GetLanguage() const {
  return language;
}
std::string CommFrame::GetDescription() const {
  return short_description;
}
std::string CommFrame::GetText() const {
  return text;
}
void CommFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Language: " << GetLanguage() << '\n'
            << "Short content description: " << GetDescription() << '\n'
            << "The actual text: " << GetText() << '\n'
            << "---------------------------\n";
}

///RVA2 Frame
Rva2Frame::Rva2Frame(const Frame& frame_, const std::string& identification_) : Frame(frame_) {
  identification = identification_;
}
std::string Rva2Frame::GetIdentification() const {
  return identification;
}
void Rva2Frame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Identification: " << GetIdentification() << '\n'
            << "---------------------------\n";
}

///EQU2 Frame
Equ2Frame::Equ2Frame(const Frame& frame_, unsigned char method_, const std::string& identification_) : Frame(frame_) {
  method = method_;
  identification = identification_;
}
unsigned char Equ2Frame::GetMethod() const {
  return method;
}
std::string Equ2Frame::GetIdentification() const {
  return identification;
}
void Equ2Frame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Interpolation method: " << (int) GetMethod() << '\n'
            << "Identification: " << GetIdentification() << '\n'
            << "---------------------------\n";
}

///PCNT Frame
PcntFrame::PcntFrame(const Frame& frame_, uint32_t counter_) : Frame(frame_) {
  counter = counter_;
}
uint32_t PcntFrame::GetCounter() const {
  return counter;
}
void PcntFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Counter: " << GetCounter() << '\n'
            << "---------------------------\n";
}

///POPM Frame
PopmFrame::PopmFrame(const Frame& frame_, const std::string& email_, unsigned char rating_, uint32_t counter_) : Frame(
    frame_) {
  email = email_;
  rating = rating_;
  counter = counter_;
}
std::string PopmFrame::GetEmail() const {
  return email;
}
unsigned char PopmFrame::GetRating() const {
  return rating;
}
uint32_t PopmFrame::GetCounter() const {
  return counter;
}
void PopmFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Email to user: " << GetEmail() << '\n'
            << "Rating: " << (int) GetRating() << '\n'
            << "Counter: " << GetCounter() << '\n'
            << "---------------------------\n";
}

///RBUF Frame
RbufFrame::RbufFrame(const Frame& frame_, uint32_t buffer_size_, bool flag_, const std::string& offset_)
    : Frame(frame_) {
  buffer_size = buffer_size_;
  flag = flag_;
  offset = offset_;
}
uint32_t RbufFrame::GetBufferSize() const {
  return buffer_size;
}
bool RbufFrame::GetFlag() const {
  return flag;
}
std::string RbufFrame::GetOffset() const {
  return offset;
}
void RbufFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Buffer size: " << GetBufferSize() << '\n'
            << "Flag: " << (int) GetFlag() << '\n'
            << "Offset to next tag: " << GetOffset() << '\n'
            << "---------------------------\n";
}

///LINK Frame
LinkFrame::LinkFrame(const Frame& frame_,
                     uint& id_,
                     const std::string& url_,
                     const std::string& additional_) : Frame(frame_) {
  url = url_;
  id = id_;
  additional = additional_;
}
uint LinkFrame::GetIdentifier() const {
  return id;
}
std::string LinkFrame::GetUrl() const {
  return url;
}
std::string LinkFrame::GetAdditional() const {
  return additional;
}
void LinkFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Identifier: " << GetIdentifier() << '\n'
            << "URL: " << GetUrl() << '\n'
            << "ID and additional data: " << GetAdditional() << '\n'
            << "---------------------------\n";
}

///POSS Frame
PossFrame::PossFrame(const Frame& frame_, unsigned char stamp_, unsigned char position_) : Frame(frame_) {
  time_stamp_format = stamp_;
  position = position_;
}
unsigned char PossFrame::GetStamp() const {
  return time_stamp_format;

}
unsigned char PossFrame::GetPosition() const {
  return position;
}
void PossFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Time stamp format: " << GetStamp() << '\n'
            << "PositionL: " << GetPosition() << '\n'
            << "---------------------------\n";
}

///USER Frame
UserFrame::UserFrame(const Frame& frame_, unsigned char encoding_byte_, std::string& language_, std::string& text_)
    : Frame(frame_) {
  encoding_byte = encoding_byte_;
  language = language_;
  text = text_;
}
std::string UserFrame::GetLanguage() const {
  return language;
}
std::string UserFrame::GetText() const {
  return text;
}
void UserFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Language: " << GetLanguage() << '\n'
            << "The actual text: " << GetText() << '\n'
            << "---------------------------\n";
}

///OWNE Frame
OwneFrame::OwneFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     std::string& price_,
                     std::string& date_,
                     std::string& seller_) : Frame(frame_) {
  encoding_byte = encoding_byte_;
  price = price_;
  date = date_;
  seller = seller_;
}
std::string OwneFrame::GetPrice() const {
  return price;
}
std::string OwneFrame::GetDate() const {
  return date;
}
std::string OwneFrame::GetSeller() const {
  return seller;
}
void OwneFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Price paid: " << GetPrice() << '\n'
            << "Date of purch: " << GetDate() << '\n'
            << "Seller: " << GetSeller() << '\n'
            << "---------------------------\n";
}

///COMR Frame
ComrFrame::ComrFrame(const Frame& frame_,
                     unsigned char encoding_byte_,
                     const std::string& price_,
                     const std::string& valid_,
                     const std::string& url_,
                     unsigned char received_byte_,
                     const std::string& seller_,
                     const std::string& description_,
                     const std::string& picture_type_) : Frame(frame_) {
  encoding_byte = encoding_byte_;
  price = price_;
  valid = valid_;
  url = url_;
  received_byte = received_byte_;
  seller = seller_;
  description = description_;
  picture_type = picture_type_;
}
std::string ComrFrame::GetPrice() const {
  return price;
}
std::string ComrFrame::GetValid() const {
  return valid;
}
std::string ComrFrame::GetSeller() const {
  return seller;
}
std::string ComrFrame::GetDescription() const {
  return description;
}
std::string ComrFrame::GetPictureType() const {
  return picture_type;
}
std::string ComrFrame::GetUrl() const {
  return url;
}
void ComrFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Price string: " << GetPrice() << '\n'
            << "Valid until: " << GetValid() << '\n'
            << "Contact URL: " << GetUrl() << '\n'
            << "Name of seller: " << GetSeller() << '\n'
            << "Description: " << GetDescription() << '\n'
            << "Picture MIME type: " << GetPictureType() << '\n'
            << "---------------------------\n";
}

///ENCR Frame
EncrFrame::EncrFrame(const Frame& frame_, const std::string& id_, unsigned char method_, const std::string& data_)
    : Frame(frame_) {
  id = id_;
  method = method_;
  data = data_;
}
std::string EncrFrame::GetIdentifier() const {
  return id;
}
unsigned char EncrFrame::GetMethod() const {
  return method;
}
std::string EncrFrame::GetData() const {
  return data;
}
void EncrFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Owner identifier: " << GetIdentifier() << '\n'
            << "Method symbol: " << GetMethod() << '\n'
            << "Encryption data: " << GetData() << '\n'
            << "---------------------------\n";
}

///GRID Frame
GridFrame::GridFrame(const Frame& frame_, const std::string& id_, unsigned char group_, const std::string& data_)
    : Frame(frame_) {
  id = id_;
  group = group_;
  data = data_;
}
std::string GridFrame::GetIdentifier() const {
  return id;
}
unsigned char GridFrame::GetGroup() const {
  return group;
}
std::string GridFrame::GetData() const {
  return data;
}
void GridFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Owner identifier: " << GetIdentifier() << '\n'
            << "Group symbol: " << GetGroup() << '\n'
            << "Group dependent data: " << GetData() << '\n'
            << "---------------------------\n";
}

///Priv Frame
PrivFrame::PrivFrame(const Frame& frame_, const std::string& id_, const std::string& data_) : Frame(frame_) {
  id = id_;
  data = data_;
}
std::string PrivFrame::GetIdentifier() const {
  return id;
}
std::string PrivFrame::GetData() const {
  return data;
}
void PrivFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Owner identifier: " << GetIdentifier() << '\n'
            << "The private data: " << GetData() << '\n'
            << "---------------------------\n";
}

///SIGN Frame
SignFrame::SignFrame(const Frame& frame_, unsigned char group_, const std::string& signature_) : Frame(frame_) {
  group = group_;
  signature = signature_;
}
std::string SignFrame::GetSignature() const {
  return signature;
}
unsigned char SignFrame::GetGroup() const {
  return group;
}
void SignFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Group symbol: " << GetGroup() << '\n'
            << "Signature: " << GetSignature() << '\n'
            << "---------------------------\n";
}

///SEEK Frame
SeekFrame::SeekFrame(const Frame& frame_, const std::string& min_offset_) : Frame(frame_) {
  min_offset = min_offset_;

}
std::string SeekFrame::GetOffset() const {
  return min_offset;
}
void SeekFrame::PrintFrame(std::ostream& stream) const {
  stream << "Frame name: " << GetName() << '\n'
            << "Minimum offset to next tag: " << GetOffset() << '\n'
            << "---------------------------\n";
}