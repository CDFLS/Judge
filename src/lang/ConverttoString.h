#include <sstream>
namespace CTS {
	static string DoubleToString(double x) {
		std::ostringstream oss;
		oss << x;
		return oss.str();
	}
	static string IntToString(int x) {
		std::ostringstream oss;
		oss << x;
		return oss.str();
	}
}
