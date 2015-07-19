/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <vector>
#include <iomanip>

#include "support/helper/hexdump.h"

supportlib::string::string_t toHexArray(std::vector<char> const &oBuffer)
{
	supportlib::string::string_t s;
	supportlib::string::char_t const *sep = ", ";
	supportlib::string::char_t str[12];
	str[0] = 0;

	for(unsigned char const &c : oBuffer)
	{
		sprintf_t(&str[supportlib::string::strlen(str)], "0x%02X", c);
		s += str;
		strcpy_t(str, sep);
	}

	return s;
}

bool hexdumpToFile(supportlib::string::string_t const &oInput, supportlib::string::string_t const &oOutput)
{
	std::fstream fl(oInput, std::ios::in | std::ios::binary);
	std::fstream txt(oOutput, std::ios::out);
	char fbuffer[8];
	while(!fl.eof())
	{
		std::vector<char> buffer;
		supportlib::string::string_t s;
		fl.read(fbuffer, sizeof(fbuffer));
		int n = fl.gcount();
		for(int i = 0; i < n; i++)
			buffer.push_back(fbuffer[i]);
		s = toHexArray(buffer) + ",";
		txt << s << std::endl;
	}
	fl.close();
	txt.close();

	return true;
}
