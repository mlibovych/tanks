#include "my_framework.h"

std::optional<std::pair<int, int>> getSize(int argc, char *argv[]) {
	if (argc == 1) {
		return std::nullopt;
	}

	std::string flag, size; 
	if (argc != 1 && argc != 3) {
		throw(std::invalid_argument("usage: ./tanks [-window WxH]"));
	}
	flag = argv[1];
	size = argv[2];
	if (flag != "-window") {
		throw(std::invalid_argument("unknown flag: " + flag));
	}
	size_t x = size.find('x');
	if (x == std::string::npos) {
		throw(std::invalid_argument("invalid size"));
	}
	std::string width = size.substr(0, x);
	std::string height = size.substr(x + 1);
	for (char c : width) {
		if (!isnumber(c)) {
			throw(std::invalid_argument("invalid size"));
		}
	}
	for (char c : height) {
		if (!isnumber(c)) {
			throw(std::invalid_argument("invalid size"));
		}
	}
	return std::make_pair(atoi(width.c_str()), atoi(height.c_str()));
}

int main(int argc, char *argv[])
{	
	try {
		auto size = getSize(argc, argv);
		
		if (!size) {
			return run(new MyFramework);
		}
		else {
			int width = (*size).first;
			int height = (*size).second;

			return run(new MyFramework(width, height));
		}
	}
	catch (std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		exit(1);
	}
}
