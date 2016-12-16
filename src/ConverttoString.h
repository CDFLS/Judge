#ifndef CONVERT_H
#define CONVERT_H
#include <sstream>
#include <string>
namespace CTS {
	static std::string DoubleToString(double x) {
		std::ostringstream oss;
		oss << x;
		return oss.str();
	}
	static std::string IntToString(int x) {
		std::ostringstream oss;
		oss << x;
		return oss.str();
	}
}
#endif
