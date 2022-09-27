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

//void	ws::HTTPparser::parseVars() {
	
//};

void	ws::HTTPparser::checkStatusLine(std::string& line) {
	// According to standart Request must be formated like :
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	this->_req.method = Split(line, " ");
	if (this->_req.method.empty())
		throw parseHTTPexception("Invalid Method in Request");
	this->_req.path = Split(line, " "); 
	if (this->_req.path.empty())
		throw parseHTTPexception("Empty location path in Request");
//	if (this->_req.path.find(".php?") != std::string::npos)
//		parseVars();
	if (line.find("HTTP/1.1") == std::string::npos)
		throw parseHTTPexception("Wrong ver. of HTTP protocol in Request");
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
	std::cout << "REQUEST \n" << this->_raw;
	line = Split(this->_raw, "\n");
	checkStatusLine(line);
	while (this->_raw.empty() == false) {
		line = Split(this->_raw, "\n");
		checkHeaderLine(line);
	}
//	if (_req.connect == EMPTY)
//		_req.connect = CLOSE;
};
//Fill functions

void	ws::HTTPparser::fillHost(std::string& line)
{
	if (this->_req.host.empty() == false)
		throw parseHTTPexception("Double host init!\n");
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
	if (this->_req.connect != EMPTY)
		throw parseHTTPexception("Double connection init!\n");
	prepareLine(line, "Connection field is empty!\n");
	if (line.find("keep-alive") != std::string::npos)
		this->_req.connect = KEEP_ALIVE;
	else
		this->_req.connect = CLOSE;
};

void	ws::HTTPparser::fillAccept(std::string &line) {
	if (this->_req.accept.empty() == false)
	{
		std::cout << line;
		throw parseHTTPexception("Double accept init!\n");
	}
	prepareLine(line, "Accept field is empty!\n");
	this->_req.accept = line;
};

void	ws::HTTPparser::fillAcceptEnc(std::string &line) {
	if (this->_req.acceptEnc.empty() == false)
		throw parseHTTPexception("Double accept-encinit!\n");
	prepareLine(line, "Accept-Encoding field is empty!\n");
	this->_req.acceptEnc = line;
};
void	ws::HTTPparser::fillAcceptLang(std::string &line) {
	if (this->_req.acceptLang.empty() == false)
		throw parseHTTPexception("Double accept-lang init!\n");
	prepareLine(line, "Accept-Language field is empty!\n");
	this->_req.acceptLang = line;
};
void	ws::HTTPparser::fillUsr(std::string &line) {
	if (this->_req.User_Agent.empty() == false)
		throw parseHTTPexception("Double User-Agent init!\n");
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
		return (this->trim(token, " \t"));
	}

    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());
	line.append("\0");
	return (trim(token, " \t"));
};
