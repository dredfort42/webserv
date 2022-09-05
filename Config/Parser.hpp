#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "ConfigStruct.hpp"
#define errFillName "Not Valid Server Name"
#define errFillListen "Not Valid Listen"
#define errFillBodySize "Not Valid Body Size"
#define errFillAutoInd "Not Valid Auto Index"
#define errFillRoot "Not Valid Root"
#define errFillMethods "Not Valid Methods"
#define errFillError "Not Valid Error line"
#define errFillIndex "Not Valid Index"
#define errFillUpPath "Not Valid Upload Path"
#define errLocationParse "Not Valid location Syntax"
namespace ws {
	class Parser {
		public:

			typedef  std::string::iterator	iterator;
			
			Parser();
			Parser(const std::string &_path);
			Parser(const Parser &other);
			~Parser();

			Parser& operator=(const Parser &other);
			void		openFile();
			void		readFile();
			void		parseFile();
			std::vector<Config> getStruct();
			void  		prepareLine(std::string& line, const char *err);
			bool		checkLine(std::string &line, Config &cnf);
			void		fillStruct(std::string &buf, Config &cnf);
			bool		fillListen(std::string &line, Config &cnf);
			bool		fillName(std::string &line, Config &cnf);
			bool		fillBodySize(std::string &line, Config &cnf);
			bool		fillAutoInd(std::string &line, Config &cnf);
			bool		fillRoot(std::string &line, Config &cnf);
			bool		fillMethods(std::string &line, Config &cnf);
			bool		fillError(std::string &line, Config &cnf);
			bool		fillIndex(std::string &line, Config &cnf);
			void		fillLocation(std::string &line, Config &cnf);
			bool		fillUploadPath(std::string &line, Config &cnf);
			std::string Split(std::string &line, std::string delimiter);
			std::string takeBlock(std::string &config, size_t pos, size_t *end);
			std::string getPath() const;
			std::string getRawText() const;
			inline std::string &trim( std::string &line, const std::string &trimmer);
			void		resetConfig(Config &cfg);
			
			void parseServerBlock(Config &cfg, const size_t &pos);
			void checkBrackets();
			
			
			int		stoi(std::string line);
			size_t checkBracketsByPos(std::string::iterator pos);

			class parseException: public std::exception
		{
			private:
				const char* _err;
				parseException() {};

			public:
				parseException(const char *str): _err(str) {};
				parseException(const std::string &str): _err(str.c_str()) {};
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
			std::vector<Config>	_cfg;
	};

}
