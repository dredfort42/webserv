#include "HTTPparser.hpp"

// Canonical form
ws::HTTPparser::HTTPparser() {};
ws::HTTPparser::HTTPparser(const std::string &request): _raw(request), processed(false) {};
ws::HTTPparser::HTTPparser(const HTTPparser &other): _raw(other._raw), _req(other._req) {};
ws::HTTPparser::~HTTPparser() {};

ws::HTTPparser& ws::HTTPparser::operator=(const HTTPparser &other) {
	this->_raw = other._raw;
	this->_req = other._req;
	return *this;
};
// Getters
std::string&	ws::HTTPparser::getRaw() {
	return this->_raw;
};

ws::HTTPreq&		ws::HTTPparser::getRequest(){
	if (this->processed == false)
	{
		this->decode();
		this->processed = true;
	}
	return this->_req;
}
// Processing Request

void	ws::HTTPparser::checkStatusLine(std::string& line) {
	// According to standart Request must be formated like :
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	this->_req.method = Split(line, " ");
	if (this->_req.method.empty() || (this->_req.method != "GET" && this->_req.method != "DELETE" && this->_req.method != "POST"))
		throw parseHTTPexception("Invalid Method in Request");
	this->_req.path = Split(line, " "); 
	if (this->_req.path.empty())
		throw parseHTTPexception("Empty location path in Request");
	if (line.find("HTTP/1.1") == std::string::npos)
		throw parseHTTPexception("Wrong ver. of HTTP protocol in Request");
}

void	ws::HTTPparser::checkHeaderLine(std::string& line) {
	line.clear();
	return;
}

void	ws::HTTPparser::decode() {
	std::string line;
	line = Split(this->_raw, "\n");
	checkStatusLine(line);
	while (this->_raw.empty() == false) {
		line = Split(this->_raw, "\n");
		checkHeaderLine(line);
	}
};

//Utils


inline std::string& ws::HTTPparser::trim( std::string &line, const std::string &trimmer)
{
	line.erase(line.find_last_not_of(trimmer)+1);         //suffixing spaces
    line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
	return line;
};

std::string	ws::HTTPparser::Split(std::string &line, std::string delimiter)
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
