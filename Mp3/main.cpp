#include <iostream>
#include <vector>
#include "argument_parser.h"
#include "header.h"
#include "frame_parse.h"

int main(int argc, const char** argv) {
  arguments argument_data = FillArguments(argc, argv);
  if (argument_data.filename.empty() and argument_data.help_frame.empty()) {
    std::cout << "ERROR: Enter file name or frame";
    return 0;
  }

  if (argument_data.list or !argument_data.frame.empty()) {
    std::ifstream fin;
    fin.open(argument_data.filename, std::ios::binary);

    MainHeader my_main_header;
    MainHeaderParse(fin, my_main_header);

    if (my_main_header.GetFlags()[1]) {
      ExtendedHeader my_extended_header;
      ExtendedHeaderParse(fin, my_extended_header);
    }

    std::vector<Frame*> frame_base;
    std::string mode;
    if (argument_data.list) mode = "list";
    FramesParse(frame_base, fin, my_main_header.GetSize());
    fin.close();
    PrintHeaderInfo(my_main_header);
    Print(frame_base, mode);
    Delete(frame_base);

  } else if (!argument_data.help_frame.empty()) {
    std::map<std::string, std::string> help_base;
    HelpFrame(help_base, argument_data.help_frame);
    for (const auto& elem : help_base) {
      std::cout << "Name: " << elem.first << '\n'
                << "Description:" << elem.second << '\n'
                << "---------------------------\n";
    }
  }
}
