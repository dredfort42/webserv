#include "HTTPparser.hpp"

// Canonical form
ws::HTTPparser::HTTPparser() {};
ws::HTTPparser::HTTPparser(const std::string &request): _raw(request),  processed(false) {};
ws::HTTPparser::HTTPparser(const HTTPparser &other): _raw(other._raw), _req(other._req), processed(other.processed){};
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
};

// Processing Request

void	ws::HTTPparser::checkStatusLine(std::string& line) {
	this->_req.method = Split(line, " ");
	this->_req.path = Split(line, " "); 
};

void	ws::HTTPparser::checkHeaderLine(std::string& line) {
	if (line.find("Host") != std::string::npos)
		fillHost(line);
	else if (line.find("Connection:") != std::string::npos)
		fillConnection(line);
	else if (line.find("Accept: ") != std::string::npos)
		fillAccept(line);
	else if (line.find("Accept-Language:") != std::string::npos)
		fillAcceptLang(line);
	else if (line.find("Accept-Encoding:") != std::string::npos)
		fillAcceptEnc(line);
	else if (line.find("User-Agent:") != std::string::npos)
		fillUsr(line);
};


void	ws::HTTPparser::decode() {
	std::string line;
//	std::cout << "REQUEST \n" << this->_raw;
	line = Split(this->_raw, "\n");
	checkStatusLine(line);
	while (this->_raw.empty() == false) {
		line = Split(this->_raw, "\n");
		checkHeaderLine(line);
	}
};
//Fill functions

void	ws::HTTPparser::fillHost(std::string& line)
{
	prepareLine(line, "Host field is empty!\n");
	if (line.find(":") == std::string::npos)
	{
		this->_req.host = line;
		this->_req.port = "80";
	}
	else
	{
		this->_req.host = Split(line, ":");
		line.pop_back();
		this->_req.port = line;
	}
};

void	ws::HTTPparser::fillConnection(std::string &line) {
	prepareLine(line, "Connection field is empty!\n");
	if (line.find("keep-alive") != std::string::npos)
		this->_req.connect = KEEP_ALIVE;
	else
		this->_req.connect = CLOSE;
};

void	ws::HTTPparser::fillAccept(std::string &line) {
	prepareLine(line, "Accept field is empty!\n");
	this->_req.accept = line;
};

void	ws::HTTPparser::fillAcceptEnc(std::string &line) {
	prepareLine(line, "Accept-Encoding field is empty!\n");
	this->_req.acceptEnc = line;
};
void	ws::HTTPparser::fillAcceptLang(std::string &line) {
	prepareLine(line, "Accept-Language field is empty!\n");
	this->_req.acceptLang = line;
};
void	ws::HTTPparser::fillUsr(std::string &line) {
	prepareLine(line, "User-Agent field is empty!\n");
	this->_req.User_Agent = line;
};
//Utils


void	ws::HTTPparser::prepareLine(std::string &line, const char *err) {
	Split(line, " ");
	if (line.empty())
		throw parseHTTPexception(err);
	line = trim(line, "\t \n;");
};

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
		return (this->trim(token, " \t\r"));
	}

    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());
	line.append("\0");
	return (trim(token, " \t\r"));
};
