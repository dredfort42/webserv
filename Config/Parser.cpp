#include "Parser.hpp"

ws::Parser::Parser(): _path("default.conf") {};

ws::Parser::Parser(const std::string &p): _path(p) {};

ws::Parser::~Parser() {};

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

void  ws::Parser::openFile() {
	this->_fd.open(this->_path.c_str(), std::ios::in);
	if (this->_fd.is_open() == false)
		throw OpenFileException();
};

void  ws::Parser::readFile() {
	std::string buf;
	while (std::getline(this->_fd, buf))
	{
		if ((this->_fd).fail())
		{
			this->_fd.close();
			throw ReadFileException();
		}
		buf = trim(buf, " \t");
		if (buf.empty() || buf[0] == '#')
			continue;
		this->_rawFile += buf;
		this->_rawFile += "\n";
	}
	this->_fd.close();

};

