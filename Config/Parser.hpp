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
			void		parseFile();
			std::string& Split(std::string &line, std::string delimiter);
			std::string getPath() const;
			std::string getRawText() const;
			inline std::string &trim( std::string &line, const std::string &trimmer);

			void checkBrackets();

			class parseException: public std::exception
		{
			private:
				const char* _err;
				parseException() {};

			public:
				parseException(const char *str): _err(str) {};
				~parseException() throw() {};

				virtual const char* what() const throw()
				{
					return (_err);
				}
		};

		private:
			std::ifstream		_fd;
			std::string			_path;
			std::string			_rawFile;
			std::vector<Config*>	_cfg;
	};
}
