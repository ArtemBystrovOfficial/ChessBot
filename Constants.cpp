#include "Constants.hpp"

bool BasicFunctions::checkArea(int i, int j) {
		return i >= 0 && i < kSizeMap&& j >= 0 && j < kSizeMap;
}

bool BasicFunctions::checkDirectory(int directory) {
	return directory >= 0 && directory < 8;
}
