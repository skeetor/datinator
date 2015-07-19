/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "support/helper/string.h"

namespace supportlib
{
	namespace string
	{
		SUPPORT_DLL_EXPORT std::vector<supportlib::string::string_t> &split(supportlib::string::string_t const &s, supportlib::string::char_t delim, std::vector<supportlib::string::string_t> &elems)
		{
			std::stringstream ss(s);
			supportlib::string::string_t item;
			while (std::getline(ss, item, delim))
				elems.push_back(item);

			return elems;
		}

		SUPPORT_DLL_EXPORT std::vector<supportlib::string::string_t> split(supportlib::string::string_t const &s, supportlib::string::char_t delim)
		{
			std::vector<supportlib::string::string_t> elems;
			split(s, delim, elems);
			return elems;
		}


		SUPPORT_DLL_EXPORT supportlib::string::string_t maskString(supportlib::string::string_t const &oInput)
		{
			supportlib::string::string_t chars = "\\\t\n\r;";
			supportlib::string::string_t s;
			supportlib::string::string_t v = oInput;
			int pos = 0;

			while((pos = v.find_first_of(chars)) != -1)
			{
				s += v.substr(0, pos);
				v = v.substr(pos);
				char c = v[0];

				switch(c)
				{
					case '\\':
						s += "\\\\";
					break;

					case '\r':
						s += "\\r";
					break;

					case '\n':
						s += "\\n";
					break;

					case '\t':
						s += "\\t";
					break;

					case ';':			// Needed for CSV style output.
						s += "\\;";
					break;
				}

				v = v.substr(1);
			}

			s += v;

			return s;
		}

		SUPPORT_DLL_EXPORT supportlib::string::string_t unmaskString(supportlib::string::string_t const &oInput)
		{
			supportlib::string::string_t s;
			supportlib::string::string_t v = oInput;
			int pos = 0;

			while((pos = v.find('\\')) != -1)
			{
				s += v.substr(0, pos);
				v = v.substr(pos);
				char c = v[0];

				int copy = 2;
				if(v.length() > 1)
					c = v[1];
				else
					break;

				switch(c)
				{
					case '\\':
						s += '\\';
					break;

					case 'r':
						s += '\r';
					break;

					case 'n':
						s += '\n';
					break;

					case 't':
						s += '\t';
					break;

					case ';':
						s += ';';
					break;

					default:
						copy = 1;
					break;
				}

				v = v.substr(copy);
			}

			s += v;

			return s;
		}

	}
}
