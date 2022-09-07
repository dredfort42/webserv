#pragma once
#include <iostream>
#include "HTTPstruct.hpp"

namespace ws {
	class HTTPparser {
		private:
			std::string _raw;
			HTTPreq		_req;
			bool		processed;

			HTTPparser();

		public:
			//Canonical form
			HTTPparser(const std::string &request);
			HTTPparser(const HTTPparser &other);
			~HTTPparser();

			HTTPparser& operator=(const HTTPparser &other);

			//Processing Request
			std::string&	getRaw();
			HTTPreq&		getRequest();
			void			decode();
			void			checkStatusLine(std::string &line);
			void			checkHeaderLine(std::string &line);
			void			parseRaw();

			//Utils

			std::string				Split(std::string &line, std::string delimiter);
			inline std::string&		trim( std::string &line, const std::string &trimmer);

			//Exception

			class parseHTTPexception: public std::exception
		{
			private:
				const char* _err;
				parseHTTPexception() {};

			public:
				parseHTTPexception(const char *str): _err(str) {};
				parseHTTPexception(const std::string &str): _err(str.c_str()) {};
				~parseHTTPexception() throw() {};

				virtual const char* what() const throw()
				{
					return (_err);
				}
		};

	};
}
