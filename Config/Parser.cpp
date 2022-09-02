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
	this->_rawFile = other._rawFile;
	return *this;
};

inline std::string& ws::Parser::trim( std::string &line, const std::string &trimmer)
{
	line.erase(line.find_last_not_of(trimmer)+1);         //suffixing spaces
    line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
	return line;
};
	
std::string	ws::Parser::Split(std::string &line, std::string delimiter)
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
	if (checkBracketsByPos(_rawFile.end()) != 0)
		throw(parseException("Brackets still opened\n"));//todo  make parse exception
}

size_t ws::Parser::checkBracketsByPos(iterator pos) {
	iterator begin = _rawFile.begin();
	size_t bracket = 0;
	while (begin != pos)
	{
		if (*begin == '{')
			bracket += 1;
		else if (!bracket && *begin == '}')
			throw(parseException("Bracket have closed before open\n"));//todo  make parse exception
		else if (*begin == '}')
			bracket -= 1;
		begin++;
	}
	return bracket;
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

std::vector<ws::Config> ws::Parser::getStruct() {

	openFile();
	readFile();
	parseFile();
	std::cout << (this->_cfg).size() << "\n";
	return (this->_cfg);
}

void	ws::Parser::parseFile() {
	Config cfg;

	cfg.IP = 0;
	cfg.PORT = 0;
	cfg.bodySize = 0;
	checkBrackets();//проверка валидности скобочек


//	while (_rawFile.empty() == false) {
	size_t pos = this->_rawFile.find("SERVER");
	if (pos != std::string::npos)
		parseServerBlock(cfg, pos);

	this->_cfg.push_back(cfg);
//	}
	
}

std::string ws::Parser::takeBlock(size_t pos, size_t *end){

	iterator it = this->_rawFile.begin() + pos;
	while (*it++ != '{')
		(*end)++;
	iterator last = it;
	size_t count = 1;
	while (count)
	{
		(*end)++;
		last++;
		if (*last == '{')
			count++;
		else if (*last == '}')
			count--;
	}
	return std::string(it, last);
}

void	ws::Parser::fillListen(std::string &line, Config &cnf)
{
	Split(line, " ");
	if (line.empty())
		throw parseException("Not Valid listen\n");
	line = trim(line, "\t \n");
	std::string split = Split(line, ":");
	if (line.empty())
	{
		cnf.IP = inet_addr("127.0.0.1");
		cnf.PORT = stoi(trim(split, ";"));
	}
	else
	{
		cnf.IP = inet_addr(split.c_str());
		cnf.PORT = stoi(trim(line, ";"));
	}
}

int		ws::Parser::stoi(std::string line)
{
	int res = 0;
	for (iterator it = line.begin(); it != line.end(); it++)
	{
		if (std::isdigit(*it) == false)
			throw parseException("ATOI ERROR");
		res *= 10;
		res += *it - '0';
	}
	return res;
}

void	ws::Parser::fillBodySize(std::string &line, Config &cnf)
{
	Split(line, " ");
	if (line.empty())
		throw parseException("Not Valid listen\n");
	line = trim(line, "\t \n;");
	cnf.bodySize = stoi(line);
}

void	ws::Parser::fillName(std::string &line, Config &cnf)
{
	Split(line, " ");
	if (line.empty())
		throw parseException("Not Valid listen\n");
	line = trim(line, "\t \n;");
	cnf.serverName = line;
}

void	ws::Parser::fillStruct(std::string &buf, Config &cnf)
{
	std::string line;
	while (buf.empty() == false)
	{
		line = Split(buf, "\n");
		if ((line[0] == '{' && line.size() <= 2) || line[0] == '}' || line[0] == '\n' || line.empty())
			continue;
		if (line.find("location") != std::string::npos)
			continue;
		if (*(line.rbegin()) != ';')
		{	
		//	std::cout << line << " exception\n";
			throw parseException("Every parameter should end with ;\n");
		}
		if (cnf.IP == 0 && line.find("listen") != std::string::npos)
			fillListen(line, cnf);
		if (cnf.serverName.empty() && line.find("server_name") != std::string::npos)
			fillName(line, cnf);
		if (cnf.bodySize == 0 && line.find("client_max_body_size") != std::string::npos)
			fillBodySize(line, cnf);
	}
}

void	ws::Parser::parseServerBlock(Config &cfg, const size_t &pos){
	size_t end = pos;
	std::string buf = takeBlock(pos, &end);
	fillStruct(buf, cfg);
	this->_rawFile.erase(pos, end);
	if (cfg.bodySize == 0)
		cfg.bodySize = 1024;
	std::cout << cfg.IP << " IP | " << cfg.PORT << " PORT\n";
	std::cout << cfg.serverName << " server_name\n";
	std::cout << cfg.bodySize << " body size\n";
}
