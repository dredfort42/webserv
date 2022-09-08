#include "Parser.hpp"

//Constructors - Destructors

ws::Parser::Parser(): _path("default.conf") {};

ws::Parser::Parser(const std::string &p): _path(p) {};

ws::Parser::~Parser() {
	if (this->_cfg.empty() == false)
		this->_cfg.clear();
};

ws::Parser& ws::Parser::operator=(const Parser &other) {
//	this->_fd = other._fd;
	this->_path = other._path;
	this->_cfg = other._cfg;
	this->_rawFile = other._rawFile;
	return *this;
};

//Getters

std::string	ws::Parser::getPath() const { return _path; };

std::string ws::Parser::getRawText() const { return _rawFile; };

std::vector<ws::Config> ws::Parser::getStruct() {

	openFile();
	readFile();
	parseFile();
	std::cout << "----------------------------------------\n\n";
	return (this->_cfg);
}

// File Routine

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
	Config cfg;

	checkBrackets();//проверка валидности скобочек

	size_t pos = this->_rawFile.find("SERVER");

	while (pos != std::string::npos) {
		resetConfig(cfg);
		parseServerBlock(cfg, pos);
		if (cfg.port.empty())
			cfg.port = "8080";
		if (cfg.ip.empty())
			cfg.ip = "0.0.0.0";
		pos = this->_rawFile.find("SERVER");
		this->_cfg.push_back(cfg);
	}
	
}

//Utils

void	ws::Parser::resetConfig(Config &cfg) {

		cfg.ip.clear();
		cfg.port.clear();
		cfg.bodySize = 0;
		cfg.serverName.clear();
		cfg.autoindex = false;
		cfg.root.clear();
		cfg.method.clear();
		cfg.index.clear();
		cfg.uploadPath.clear();
		cfg.errorPage.clear();
		cfg.Locations.clear();
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

//FILLER blocks

void	ws::Parser::prepareLine(std::string &line, const char *err) {

	Split(line, " ");
	if (line.empty())
		throw parseException(err);
	line = trim(line, "\t \n;");
}

bool	ws::Parser::fillListen(std::string &line, Config &cnf)
{
	prepareLine(line, errFillListen);
	std::string split = Split(line, ":");
	if (line.empty())
	{
		cnf.ip = "127.0.0.1";
		cnf.port = trim(split, ";");
	}
	else
	{
		cnf.ip = split;
		cnf.port = trim(line, ";");
	}
	return true;
}

bool	ws::Parser::fillBodySize(std::string &line, Config &cnf)
{
	prepareLine(line, errFillBodySize);
	cnf.bodySize = stoi(line);
	return true;
}

bool	ws::Parser::fillName(std::string &line, Config &cnf)
{
	prepareLine(line, errFillName);
	cnf.serverName = line;
	return true;
}

template <class T>
bool	ws::Parser::fillAutoInd(std::string &line, T &cnf)
{
	prepareLine(line, errFillAutoInd);
	if  (line == "off")
		cnf.autoindex = false;
	else if (line == "on")
		cnf.autoindex = true;
	else
		throw parseException("Wrong format of autoindex\n");
	return true;
}

template <class T>
bool	ws::Parser::fillRoot(std::string &line, T &cnf) {
	prepareLine(line, errFillRoot);
	cnf.root = line;
	return true;
}

template <class T>
bool	ws::Parser::fillMethods(std::string &line, T &cnf) {
	prepareLine(line, errFillMethods);
	cnf.method = line;
	return true;
}

template <class T>
bool	ws::Parser::fillError(std::string &line, T &cnf) {
	prepareLine(line, errFillError);
	cnf.errorPage = line;
	return true;
}

template <class T>
bool	ws::Parser::fillIndex(std::string &line, T &cnf) {
	prepareLine(line, errFillIndex);
	cnf.index = line;
	return true;
}

template <class T>
bool	ws::Parser::fillUploadPath(std::string &line, T &cnf) {
	prepareLine(line, errFillUpPath);
	cnf.uploadPath = line;
	return true;
}

template <class T>
bool	ws::Parser::fillUploadBinPath(std::string &line, T &cnf) {
	prepareLine(line, errFillUpPath);
	cnf.binPath = line;
	return true;
}

template <class T>
bool	ws::Parser::fillRedirect(std::string &line, T &cnf) {
	prepareLine(line, errFillUpPath);
	cnf.redirect = line;
	return true;
}

void	ws::Parser::fillLocation(std::string &line, std::string &buf, Config &cnf) {
	ws::Location loc;
	size_t end = 0;
	//std::string path;

	prepareLine(line, errLocationParse);
	loc.raw = takeBlock(buf, 0, &end, true);
	buf.erase(0, end);
	parseLocationBlock(line, loc);
	cnf.Locations.push_back(loc);
}

bool	ws::Parser::checkLine(std::string &line, Config &cnf)
{
		if ((line[0] == '{' && line.size() <= 2) || line[0] == '}' || line[0] == '\n' || line.empty())
			return true;
		if (*(line.rbegin()) != ';')
			throw parseException("Every parameter should end with ;\n");
		if (cnf.ip.empty() && line.find("listen") != std::string::npos)
			return fillListen(line, cnf);
		if (cnf.serverName.empty() && line.find("server_name") != std::string::npos)
			return fillName(line, cnf);
		if (cnf.bodySize == 0 && line.find("client_max_body_size") != std::string::npos)
			return fillBodySize(line, cnf);
		if (line.find("autoindex") != std::string::npos)
			return fillAutoInd(line, cnf);
		if (cnf.method.empty() && line.find("methods") != std::string::npos)
			return fillMethods(line, cnf);
		if (cnf.root.empty() && line.find("root") != std::string::npos)
			return fillRoot(line, cnf);
		if (cnf.errorPage.empty() && line.find("error_page") != std::string::npos)
			return fillError(line, cnf);
		if (cnf.index.empty() && line.find("index") != std::string::npos)
			return fillIndex(line, cnf);
		if (cnf.uploadPath.empty() && line.find("upload_path") != std::string::npos)
			return fillUploadPath(line, cnf);
		return true;
}

bool	ws::Parser::checkLine(std::string &line, Location &cnf)
{
		if ((line[0] == '{' && line.size() <= 2) || line[0] == '}' || line[0] == '\n' || line.empty())
			return true;
		if (*(line.rbegin()) != ';')
			throw parseException("Every parameter should end with ;\n");
		if (line.find("autoindex") != std::string::npos)
			return fillAutoInd(line, cnf);
		if (cnf.method.empty() && line.find("methods") != std::string::npos)
			return fillMethods(line, cnf);
		if (cnf.root.empty() && line.find("root") != std::string::npos)
			return fillRoot(line, cnf);
		if (cnf.errorPage.empty() && line.find("error_page") != std::string::npos)
			return fillError(line, cnf);
		if (cnf.index.empty() && line.find("index") != std::string::npos)
			return fillIndex(line, cnf);
		if (cnf.uploadPath.empty() && line.find("upload_path") != std::string::npos)
			return fillUploadPath(line, cnf);
		if (cnf.binPath.empty() && line.find("bin_path") != std::string::npos)
			return fillUploadBinPath(line, cnf);
		if (cnf.redirect.empty() && line.find("redirection") != std::string::npos)
			return fillRedirect(line, cnf);
		return true;
}

void	ws::Parser::fillStruct(std::string &buf, Config &cnf)
{
	std::string line;
	while (buf.empty() == false)
	{
		line = Split(buf, "\n");
		//std::cout << line << "\n";
		if (line.find("location") != std::string::npos)
		{
		//	std::cout<<line<<"\n";
			fillLocation(line, buf, cnf);
			continue;
		}
		if (checkLine(line, cnf) == false)
		{
			throw parseException("Wrong parameter");
		}
	}
}

//Main parse block

void	ws::Parser::parseServerBlock(Config &cfg, const size_t &pos){
	size_t end = pos;
	std::string buf = takeBlock(this->_rawFile, pos, &end, false);
	fillStruct(buf, cfg);
	this->_rawFile.erase(pos, end + 1);
	if (cfg.bodySize == 0)
		cfg.bodySize = 1024;
	std::cout << cfg;
//	std::cout << cfg.ip << " IP | " << cfg.port << " PORT\n";
//	std::cout << cfg.serverName << " server_name\n";
//	std::cout << cfg.bodySize << " body size\n";
//	std::cout << "---------------------------------------\n";
}

std::string ws::Parser::takeBlock(std::string &config, size_t pos, size_t *end, bool loc){

	iterator it = config.begin() + pos;
	if (loc == false)
	{
		while (*it++ != '{')
			(*end)++;
	}
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
	(*end)++;
	return std::string(it, last);
}


//Parse Locations

void	ws::Parser::parseLocationBlock(std::string &line, Location &loc) {
	loc.path = trim(line, "{ \t}");
//	std::cout << loc.raw << "\n";
	while (loc.raw.empty() == false) {
		line = Split(loc.raw, "\n");
		checkLine(line, loc);
	}
}


