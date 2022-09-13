//
// Created by Dmitry Novikov on 11.09.2022.
//

#include "CGI.hpp"

namespace ws
{

	CGI::CGI(std::string path)
	{
		File file = File(path, OPEN_FILE);
		_response.clear();
		
		if (file._fileType == ".php")
			_executableFile = "/usr/local/bin/php-cgi";
		else if (file._fileType == ".py")
			_executableFile = "/Library/Frameworks/Python.framework/Versions/3"
							 ".10/bin/python3";
		std::cout << _executableFile << std::endl;

		_response = "<html>\n"
					"    <head>\n"
					"        <link rel='stylesheet' href='style.css' />\n"
					"        <script>\n"
					"            function getFile() {\n"
					"                document.getElementById(\"upfile\").click();\n"
					"            }\n"
					"\n"
					"            function sub(obj) {\n"
					"                var file = obj.value;\n"
					"                var fileName = file.split(\"\\\\\");\n"
					"                document.getElementById(\"uploadButton\").innerHTML = fileName[fileName.length - 1];\n"
					"                document.getElementById(\"uploadfile\").click();\n"
					"                event.preventDefault();\n"
					"            }\n"
					"        </script>\n"
					"    </head>\n"
					"    <body>\n"
					"\n"
					"        <div class=\"heading\">\n"
					"            <h1 style=\"text-align:center\">Memes gallery</h1>\n"
					"        </div>\n"
					"        <div class=\"tile-layout\">\n"
					"\n"
					"            \n"
					"        </div>\n"
					"\n"
					"        <div class=\"center heading\" id=\"wrapper\">\n"
					"            <form method=\"POST\" action=\"\" enctype=\"multipart/form-data\" >\n"
					"                <div id=\"uploadButton\" onclick=\"getFile()\" class=\"input-button\" name=\"uploadfile\" >UPLOAD FILE</div>\n"
					"                <div class=\"hidden\">\n"
					"                    <input id=\"upfile\" type=\"file\" value=\"upload\" onchange=\"sub(this)\" name=\"choosefile\" />\n"
					"                    <button hidden id=\"uploadfile\" type=\"submit\" name=\"uploadfile\" >UPLOAD</button>\n"
					"                </div>\n"
					"            </form>\n"
					"\n"
					"            \n"
					"        </div>\n"
					"    </body>\n"
					"</html>";
	}

	std::string CGI::getResponse()
	{
		return std::string();
	}

} // ws
