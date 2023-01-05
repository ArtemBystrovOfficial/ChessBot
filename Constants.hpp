#pragma once

constexpr size_t kSizeMap = 8;
constexpr size_t kCountDirectories = 8;
constexpr int kGrayColor = 0x80;
constexpr int kRedColor = 0x40;
constexpr int kBlueColor = 0x10;
constexpr int kYellowColor = 0xd0;

#define FOR_MAP for(int i=0;i<kSizeMap;i++)for(int j=0;j<kSizeMap;j++)

namespace BasicFunctions {

bool checkArea(int i, int j);
bool checkDirectory(int directory);

}
