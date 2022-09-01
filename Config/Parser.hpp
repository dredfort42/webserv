#include <iostream>
#include <vector>
#include "ConfigStruct.hpp"
#include <fstream>

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
			std::vector<Config*> getStruct();
			void		fillStruct(std::string &buf, Config *cnf);
			void		fillListen(std::string &line, Config *cnf);
			void		fillName(std::string &line, Config *cnf);
			void		fillBodySize(std::string &line, Config *cnf);
			std::string Split(std::string &line, std::string delimiter);
			std::string takeBlock(size_t pos, size_t *end);
			std::string getPath() const;
			std::string getRawText() const;
			inline std::string &trim( std::string &line, const std::string &trimmer);
			
			void parseServerBlock(Config *cfg, const size_t &pos);
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
