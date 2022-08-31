#pragma once
#include <iostream>
#include <vector>
#include "ConfigStruct.hpp"
#include <fstream>

namespace ws {


	class Parser {
		public:
			Parser();
			Parser(const std::string &_path);
			Parser(const Parser &other);
			~Parser();

			Parser& operator=(const Parser &other);
			void		openFile();
			void		readFile();
			std::string& Split(std::string &line, std::string delimiter);
			std::string getPath() const;
			std::string getRawText() const;
			inline std::string &trim( std::string &line, const std::string &trimmer);



			class OpenFileException: public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Can't open file\n");
				}
		};

			class ReadFileException: public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Can't read file\n");
				}
		};

		private:
			std::ifstream		_fd;
			std::string			_path;
			std::string			_rawFile;
			std::vector<Config>	_cfg;
	};
}
