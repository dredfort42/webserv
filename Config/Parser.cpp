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
//	std::cout << this->_rawFile << std::endl;
};

void	ws::Parser::checkConfigs(Config cfg, siteConf setup)
{
	if (_cfg.size() > 0)
	{
		for (std::vector<Config>::iterator it = _cfg.begin(); it != _cfg.end(); it++)
		{
			//std::cout << it->ip << " = " << cfg.ip <<"\n";
			//std::cout << it->port << " = " << cfg.port <<"\n";
			if (it->ip == cfg.ip && it->port == cfg.port)
			{
				it->setup[setup.serverName] = setup;
				return;
			}
		}
	}
	cfg.setup[setup.serverName] = setup;
	this->_cfg.push_back(cfg);
}

void	ws::Parser::parseFile() {
	Config 		cfg;
	siteConf	setup;

	checkBrackets();//проверка валидности скобочек

	size_t pos = this->_rawFile.find("SERVER");

	while (pos != std::string::npos) {
		resetConfig(cfg, setup);
		parseServerBlock(cfg, setup, pos);
		if (cfg.port.empty())
			cfg.port = "8080";
		if (cfg.ip.empty())
			cfg.ip = "0.0.0.0";
		pos = this->_rawFile.find("SERVER");
		checkConfigs(cfg, setup);
	}
	for (std::vector<Config>::iterator it = _cfg.begin() ; it != _cfg.end(); it++)
	{
		std::cout << *it;
	}
}

//Utils

void	ws::Parser::resetConfig(Config &cfg, siteConf &setup) {

		cfg.ip.clear();
		cfg.port.clear();
		cfg.setup.clear();
		setup.bodySize = 0;
		setup.serverName.clear();
		setup.autoindex = false;
		setup.root.clear();
		setup.method.clear();
		setup.index.clear();
		setup.uploadPath.clear();
		setup.errorPage.clear();
		setup.Locations.clear();
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

bool	ws::Parser::fillBodySize(std::string &line, siteConf &cnf)
{
	prepareLine(line, errFillBodySize);
	cnf.bodySize = stoi(line);
	return true;
}

bool	ws::Parser::fillName(std::string &line, siteConf &cnf)
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
	cnf.errorPage[Split(line, " ")] = line;
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
	cnf.redirect[Split(line, " ")] = line;
	return true;
}

void	ws::Parser::fillLocation(std::string &line, std::string &buf, siteConf &cnf) {
	ws::Location loc;
	size_t end = 0;
	//std::string path;

	prepareLine(line, errLocationParse);
	loc.raw = takeBlock(buf, 0, &end, true);
	buf.erase(0, end);
	parseLocationBlock(line, loc);
	cnf.Locations.push_back(loc);
}

bool	ws::Parser::checkLine(std::string &line, Config &cnf, siteConf &setup)
{
		if ((line[0] == '{' && line.size() <= 2) || line[0] == '}' || line[0] == '\n' || line.empty())
			return true;
		if (*(line.rbegin()) != ';')
			throw parseException("Every parameter should end with ;\n");
		if (cnf.ip.empty() && line.find("listen") != std::string::npos)
			return fillListen(line, cnf);
		if (setup.serverName.empty() && line.find("server_name") != std::string::npos)
			return fillName(line, setup);
		if (setup.bodySize == 0 && line.find("client_max_body_size") != std::string::npos)
			return fillBodySize(line, setup);
		if (line.find("autoindex") != std::string::npos)
			return fillAutoInd(line, setup);
		if (setup.method.empty() && line.find("methods") != std::string::npos)
			return fillMethods(line, setup);
		if (setup.root.empty() && line.find("root") != std::string::npos)
			return fillRoot(line, setup);
		if (line.find("error_page") != std::string::npos)
			return fillError(line, setup);
		if (setup.index.empty() && line.find("index") != std::string::npos)
			return fillIndex(line, setup);
		if (setup.uploadPath.empty() && line.find("upload_path") != std::string::npos)
			return fillUploadPath(line, setup);
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
		if (line.find("error_page") != std::string::npos)
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

void	ws::Parser::fillStruct(std::string &buf, Config &cnf, siteConf &setup)
{
	std::string line;
//	std::cout  << buf;
	while (buf.empty() == false)
	{
		line = Split(buf, "\n");
		if (line.find("location") != std::string::npos)
		{
			fillLocation(line, buf, setup);
			continue;
		}
		if (checkLine(line, cnf, setup) == false)
		{
			throw parseException("Wrong parameter");
		}
	}
}

//Main parse block

void	ws::Parser::parseServerBlock(Config &cfg, siteConf &setup, const size_t &pos){
	size_t end = pos;
	std::string buf = takeBlock(this->_rawFile, pos, &end, false);
	fillStruct(buf, cfg, setup);
	this->_rawFile.erase(pos, end + 1);
	if (setup.bodySize == 0)
		setup.bodySize = 1024;
//	std::cout << cfg;


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
