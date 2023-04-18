#include "frame_base.h"
#include "header.h"
#include "map"

void FramesParse(std::vector<Frame*>& frame_base, std::ifstream& fin, uint64_t size);
void Print(std::vector<Frame*>& frame_base, std::string& mode);
void HelpFrame(std::map<std::string, std::string>& help_base, const std::string& cur_frame_name);
void Delete(std::vector<Frame*>& frame_base);