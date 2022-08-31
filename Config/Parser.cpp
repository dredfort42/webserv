#include "Parser.hpp"

ws::Parser::Parser(): _path("default.conf") {};

ws::Parser::Parser(const std::string &p): _path(p) {};

ws::Parser::~Parser() {};

std::string	ws::Parser::getPath() { return _path; }

ws::Parser& ws::Parser::operator=(const Parser &other) {
	this->_fd = other._fd;
	this->_path = other._path;
	this->_cfg = other._cfg;
	return *this;
};
	


