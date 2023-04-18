#include "frame_parse.h"
#include "frame_base.h"
#include <vector>
#include "map"

void TerminatedByte(std::string& value, unsigned char encoding_byte) {
  uint n = value.size();
  if (encoding_byte == 1 or encoding_byte == 2) {
    if (value[n - 1] == 0 and value[n - 2] == 0) {
      value = value.substr(0, n - 2);
    }
  } else if (encoding_byte == 0 or encoding_byte == 3) {
    if (value[n - 1] == 0) {
      value = value.substr(0, n - 1);
    }
  }
}

void StringSplit(std::string& str_1, std::string& str_2, std::string& info) {
  int null_char_ind = 0;
  for (int j = 0; j < info.size(); j++) {
    if (info[j] == 0) {
      null_char_ind = j;
      break;
    }
  }
  str_1 = info.substr(0, null_char_ind);
  str_2 = info.substr(null_char_ind + 1, info.size() - 1 - null_char_ind);
}

void FramesParse(std::vector<Frame*>& frame_base, std::ifstream& fin, uint64_t size) {
  for (int i = 0; i < size - 10;) {
    unsigned char c;
    std::string frame_name;
    std::string frame_size_str;
    uint64_t frame_size;
    std::bitset<8> flags_1, flags_2;

    for (int j = 0; j < 4; j++, i++) {
      c = fin.get();
      frame_name.push_back((char) c);
    }

    for (int j = 0; j < 4; j++, i++) {
      c = fin.get();
      frame_size_str += (static_cast<std::bitset<7>> (c)).to_string();
    }
    frame_size = (static_cast<std::bitset<7 * 4>> (frame_size_str)).to_ullong();

    for (int j = 0; j < 2; j++, i++) {
      c = fin.get();
      if (j == 0) flags_1 = (std::bitset<8>) c;
      if (j == 1) flags_2 = (std::bitset<8>) c;
    }
    Frame base_frame(frame_name, frame_size, flags_1, flags_2);

    if (frame_name[0] == 'T' and frame_name != "TXXX") {
      std::string info;
      unsigned char encoding_byte;
      encoding_byte = fin.get();
      i++;
      for (int j = 0; j < frame_size - 1; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      TerminatedByte(info, encoding_byte);
      auto* cur_frame = new TextFrame(base_frame, info, encoding_byte);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "TXXX") {
      std::string value;
      std::string description;
      std::string info;
      unsigned char encoding_byte;
      encoding_byte = fin.get();
      i++;
      for (int j = 0; j < frame_size - 1; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      TerminatedByte(info, encoding_byte);
      StringSplit(description, value, info);
      auto* cur_frame = new TxxxFrame(base_frame, value, description, encoding_byte);
      frame_base.push_back(cur_frame);
    } else if (frame_name == "UFID") {
      std::string owner;
      std::string identifier;
      std::string info;
      for (int j = 0; j < frame_size; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      StringSplit(owner, identifier, info);
      auto* cur_frame = new UfidFrame(base_frame, owner, identifier);
      frame_base.push_back(cur_frame);
    } else if (frame_name[0] == 'W' and frame_name != "WXXX") {
      std::string info;
      for (int j = 0; j < frame_size; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      auto* cur_frame = new UrlFrame(base_frame, info);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "WXXX") {
      std::string url;
      std::string description;
      std::string info;
      unsigned char encoding_byte;
      encoding_byte = fin.get();
      i++;
      for (int j = 0; j < frame_size - 1; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      TerminatedByte(info, encoding_byte);
      StringSplit(description, url, info);
      auto* cur_frame = new WxxxFrame(base_frame, url, description, encoding_byte);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "ETCO") {
      c = fin.get();
      i++;
      auto* cur_frame = new EtcoFrame(base_frame, c);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "USLT") {
      unsigned char encoding_byte = fin.get();
      i++;
      std::string language;
      std::string info;
      std::string content_descriptor;
      std::string text;
      for (int j = 0; j < 3; j++, i++) {
        c = fin.get();
        language.push_back((char) c);
      }
      for (int j = 0; j < frame_size - 4; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      TerminatedByte(info, encoding_byte);
      StringSplit(content_descriptor, text, info);
      auto* cur_frame = new UsltFrame(base_frame, encoding_byte, language, content_descriptor, text);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "SYLT") {
      unsigned char encoding_byte = fin.get();
      i++;
      std::string language;
      for (int j = 0; j < 3; j++, i++) {
        c = fin.get();
        language.push_back((char) c);
      }
      unsigned char time_stamp_format = fin.get();
      unsigned char content_type = fin.get();
      i += 2;
      std::string info;
      std::string content_descriptor;
      for (int j = 0; j < frame_size - 6; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      TerminatedByte(info, encoding_byte);
      content_descriptor = info.substr(0, info.size() - 1);
      auto* cur_frame = new SyltFrame(base_frame,
                                      encoding_byte,
                                      language,
                                      time_stamp_format,
                                      content_type,
                                      content_descriptor);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "COMM") {
      unsigned char encoding_byte = fin.get();
      i++;
      std::string language;
      for (int j = 0; j < 3; j++, i++) {
        c = fin.get();
        language.push_back((char) c);
      }
      std::string info;
      std::string short_description;
      std::string text;
      for (int j = 0; j < frame_size - 4; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      TerminatedByte(info, encoding_byte);
      StringSplit(short_description, text, info);
      auto* cur_frame = new CommFrame(base_frame,
                                      encoding_byte,
                                      language,
                                      short_description,
                                      text);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "RVA2") {
      std::string info;
      std::string identification;
      for (int j = 0; j < frame_size; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      identification = info.substr(0, info.size() - 1);
      auto* cur_frame = new Rva2Frame(base_frame, identification);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "EQU2") {
      unsigned char method = fin.get();
      i++;
      std::string identification;
      for (int j = 0; j < frame_size - 1; j++, i++) {
        c = fin.get();
        identification.push_back((char) c);
      }
      if (identification[identification.size() - 1] == 0) {
        identification = identification.substr(0, identification.size() - 1);
      }
      auto* cur_frame = new Equ2Frame(base_frame, method, identification);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "PCNT") {
      std::string cnt_size_str;
      uint32_t cnt_size;
      for (int j = 0; j < 4; j++, i++) {
        c = fin.get();
        cnt_size_str += ((std::bitset<7>) c).to_string();
      }
      cnt_size = ((std::bitset<7 * 4>) cnt_size_str).to_ullong();
      auto* cur_frame = new PcntFrame(base_frame, cnt_size);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "POPM") {
      std::string user_email;
      while (true) {
        c = fin.get();
        i++;
        if (c == 0) {
          break;
        } else {
          user_email.push_back((char) c);
        }
      }
      unsigned char rating = fin.get();
      i++;
      std::string cnt_size_str;
      uint32_t cnt_size;
      for (int j = 0; j < std::min(4, int(frame_size - 2 - user_email.size())); j++, i++) {
        c = fin.get();
        cnt_size_str += ((std::bitset<7>) c).to_string();
      }
      cnt_size = ((std::bitset<7 * 4>) cnt_size_str).to_ullong();
      auto* cur_frame = new PopmFrame(base_frame, user_email, rating, cnt_size);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "RBUF") {
      std::string buffer_size_str;
      uint32_t buffer_size;
      for (int j = 0; j < 3; j++, i++) {
        c = fin.get();
        buffer_size_str += ((std::bitset<7>) c).to_string();
      }
      buffer_size = ((std::bitset<7 * 3>) buffer_size_str).to_ullong();
      bool flag = ((std::bitset<8>) fin.get())[7];
      i++;
      std::string offset;
      for (int j = 0; j < frame_size; j++, i++) {
        c = fin.get();
        offset.push_back((char) c);
        i++;
      }
      auto* cur_frame = new RbufFrame(base_frame, buffer_size, flag, offset);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "LINK") {
      std::string id_str;
      uint32_t id;
      for (int j = 0; j < 4; j++, i++) {
        c = fin.get();
        id_str += ((std::bitset<7>) c).to_string();
      }
      id = ((std::bitset<7 * 4>) id_str).to_ullong();
      std::string info;
      std::string url;
      std::string data;
      for (int j = 0; j < frame_size - 4; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      StringSplit(url, data, info);
      auto* cur_frame = new LinkFrame(base_frame, id, url, data);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "POSS") {
      unsigned char stamp = fin.get();
      unsigned char position = fin.get();
      i += 2;
      auto* cur_frame = new PossFrame(base_frame, stamp, position);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "USER") {
      unsigned char encoding_byte = fin.get();
      i++;
      std::string language;
      for (int j = 0; j < 3; j++, i++) {
        c = fin.get();
        language.push_back((char) c);
      }
      std::string info;
      for (int j = 0; j < frame_size - 4; j++, i++) {
        c = fin.get();
        info.push_back((char) c);
      }
      TerminatedByte(info, encoding_byte);
      auto* cur_frame = new UserFrame(base_frame,
                                      encoding_byte,
                                      language,
                                      info);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "OWNE") {
      unsigned char encoding_byte = fin.get();
      i++;
      std::string price;
      std::string date;
      std::string seller;
      while (true) {
        c = fin.get();
        i++;
        if (c == 0) {
          break;
        } else {
          price.push_back((char) c);
        }
      }
      for (int j = 0; j < 8; j++, i++) {
        if (j == 4 or j == 6) {
          date.push_back('.');
        }
        date.push_back((char) fin.get());
      }
      for (int j = 0; j < frame_size; j++, i++) {
        seller.push_back((char) fin.get());
      }
      TerminatedByte(seller, encoding_byte);
      TerminatedByte(price, encoding_byte);
      auto* cur_frame = new OwneFrame(base_frame, encoding_byte, price, date, seller);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "COMR") {
      int byte_cnt = 0;
      unsigned char encoding_byte = fin.get();
      byte_cnt++;
      std::string price;
      while (true) {
        c = fin.get();
        byte_cnt++;
        if (c == 0) {
          break;
        } else {
          price.push_back((char) c);
        }
      }
      std::string valid;
      for (int j = 0; j < 8; j++, byte_cnt++) {
        if (j == 4 or j == 6) {
          valid.push_back('.');
        }
        valid.push_back((char) fin.get());
      }
      std::string url;
      while (true) {
        c = fin.get();
        byte_cnt++;
        if (c == 0) {
          break;
        } else {
          url.push_back((char) c);
        }
      }
      unsigned char received = fin.get();
      byte_cnt++;
      std::string seller;
      while (true) {
        c = fin.get();
        byte_cnt++;
        if (c == 0) {
          break;
        } else {
          seller.push_back((char) c);
        }
      }
      std::string description;
      while (true) {
        c = fin.get();
        byte_cnt++;
        if (c == 0) {
          break;
        } else {
          description.push_back((char) c);
        }
      }
      std::string type;
      while (true) {
        c = fin.get();
        byte_cnt++;
        if (c == 0) {
          break;
        } else {
          type.push_back((char) c);
        }
      }
      for (int j = 0; j < size - byte_cnt; j++, i++) fin.get();
      i += byte_cnt;
      auto cur_frame = new ComrFrame(base_frame, encoding_byte, price, valid,
                                     url, received, seller, description, type);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "ENCR") {
      std::string id;
      while (true) {
        c = fin.get();
        i++;
        if (c == 0) {
          break;
        } else {
          id.push_back((char) c);
        }
      }
      unsigned char method = fin.get();
      i++;
      std::string data;
      while (true) {
        c = fin.get();
        i++;
        if (c == 0) {
          break;
        } else {
          data.push_back((char) c);
        }
      }
      auto* cur_frame = new EncrFrame(base_frame, id, method, data);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "GRID") {
      std::string id;
      while (true) {
        c = fin.get();
        i++;
        if (c == 0) {
          break;
        } else {
          id.push_back((char) c);
        }
      }
      unsigned char group = fin.get();
      i++;
      std::string data;
      while (true) {
        c = fin.get();
        i++;
        if (c == 0) {
          break;
        } else {
          data.push_back((char) c);
        }
      }
      auto* cur_frame = new GridFrame(base_frame, id, group, data);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "PRIV") {
      std::string id;
      std::string data;
      std::string info;
      for (int j = 0; j < frame_size; j++, i++) {
        info.push_back((char) fin.get());
      }
      StringSplit(id, data, info);
      auto* cur_frame = new PrivFrame(base_frame, id, data);
      frame_base.push_back(cur_frame);

    } else if (frame_name == "SIGN") {
      unsigned char group = fin.get();
      i++;
      std::string signature;
      for (int j = 0; j < frame_size - 1; j++, i++) {
        signature.push_back((char) fin.get());
      }
      auto* cur_frame = new SignFrame(base_frame, group, signature);
      frame_base.push_back(cur_frame);
    } else if (frame_name == "SEEK") {
      std::string offset;
      for (int j = 0; j < frame_size; j++, i++) {
        offset.push_back((char) fin.get());
      }
      auto* cur_frame = new SeekFrame(base_frame, offset);
      frame_base.push_back(cur_frame);
    } else {
      for (int j = 0; j < frame_size; j++, i++) {
        fin.get();
      }
    }
  }
}

void Print(std::vector<Frame*>& frame_base, std::string& mode) {
  if (mode == "list") {
    std::cout << "Frames count: " << frame_base.size() << '\n'
              << "---------------------------\n";
    for (auto elem : frame_base) {
      std::cout << *elem;
    }
  } else if (mode != "list") {
    for (auto elem : frame_base) {
      if (mode == elem->GetName()) {
        std::cout << *elem;
      }
    }
  }
}

void HelpFrame(std::map<std::string, std::string>& help_base, const std::string& cur_frame_name) {
  std::ifstream fin;
  fin.open("./resources/FrameHelp.txt");
  std::string frame;
  std::string help;
  while (fin >> frame, getline(fin, help)) {
    if (frame == cur_frame_name or cur_frame_name == "list") {
      help_base.insert({frame, help});
      if (frame == cur_frame_name) break;
    }
  }
  fin.close();
}

void Delete(std::vector<Frame*>& frame_base) {
  for (auto* elem : frame_base) delete elem;
}
