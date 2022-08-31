#pragma once
#include <iostream>
#include <vector>
#include "ConfigStruct.hpp"


namespace ws {


	class Parser {
		public:
			Parser();
			Parser(const std::string &_path);
			Parser(const Parser &other);
			~Parser();

			Parser& operator=(const Parser &other);

			std::string getPath();
			
		private:
			int					_fd;
			std::string			_path;
			std::vector<Config>	_cfg;
	};
}
