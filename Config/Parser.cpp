#include "Parser.hpp"

ws::Parser::Parser(): _path("default.conf") {};

ws::Parser::Parser(const std::string &p): _path(p) {};

ws::Parser::~Parser() {
	if (this->_cfg.empty() == false)
		this->_cfg.clear();
};

std::string	ws::Parser::getPath() const { return _path; };

std::string ws::Parser::getRawText() const { return _rawFile; };

ws::Parser& ws::Parser::operator=(const Parser &other) {
//	this->_fd = other._fd;
	this->_path = other._path;
	this->_cfg = other._cfg;
	return *this;
};

inline std::string& ws::Parser::trim( std::string &line, const std::string &trimmer)
{
	line.erase(line.find_last_not_of(trimmer)+1);         //suffixing spaces
    line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
	return line;
};
	
std::string& ws::Parser::Split(std::string &line, std::string delimiter)
{
	size_t pos = 0;
	std::string token;
	pos = line.find(delimiter);
	if (pos == std::string::npos)
	{
		token = line;
		line.erase();
		return (this->trim(token, " \t"));
	}

    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());
	return (trim(token, " \t"));
};

void ws::Parser::checkBrackets()
{
	std::string::iterator begin = _rawFile.begin();
	size_t bracket = 0;
	while (begin != _rawFile.end())
	{
		if (*begin == '{')
			bracket += 1;
		else if (!bracket && *begin == '}')
			throw(parseException("Bracket have closed before open\n"));//todo  make parse exception
		else if (*begin == '}')
			bracket -= 1;
		begin++;
	}
	if (bracket != 0)
		throw(parseException("Brackets still opened\n"));//todo  make parse exception
}

void  ws::Parser::openFile() {
	this->_fd.open(this->_path.c_str(), std::ios::in);
	if (this->_fd.is_open() == false)
		throw parseException("Can't open File\n");
};

void  ws::Parser::readFile() {
	std::string buf;
	while (std::getline(this->_fd, buf))
	{
		if ((this->_fd).fail())
		{
			this->_fd.close();
			throw parseException("Can't readFile\n");
		}
		buf = trim(buf, " \t");
		if (buf.empty() || buf[0] == '#')
			continue;
		this->_rawFile += buf;
		this->_rawFile += "\n";
	}
	this->_fd.close();

};

void	ws::Parser::parseFile() {
	Config *cfg = new(Config);
	
	checkBrackets();

//	while (_rawFile.empty() == false){
//		parseServerBlock(cfg);
//	}

	
	this->_cfg.push_back(cfg);
}

