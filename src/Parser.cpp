#include "Parser.h"

Parser::Parser(std::string filename)
: ifs(filename)
{
	if (!ifs.is_open())
		throw std::exception();
}

std::string Parser::trim(std::string str)
{
	int32_t start = 0;
	int32_t end = str.length();
	while (start < static_cast<int32_t>(str.length()) && std::isspace(str[start]))
		start++;
	while (end >= 0 && std::isspace(str[end - 1]))
		end--;
	if (start >= end)
		return ("");
	str = str.substr(start, end - start);
	for (start = 0; start < static_cast<int32_t>(str.length()) - 1; ++start)
	{
		if (std::isspace(str[start]) && std::isspace(str[start + 1]))
		{
			str = str.substr(0, start) + " " + str.substr(start + 2, str.length() - (start + 2));
			if (start >= static_cast<int32_t>(str.length()) - 1)
				break;
			start--;
		}
		if (std::isspace(str[start]))
			str[start] = ' ';
	}
	return str;
}

void Parser::parse()
{
	std::string line;
	while (getline(this->ifs, line))
	{
		line = trim(line);
		if (line.length() == 0 || line[0] == '#')
			continue;
		parseline(line);
	}
}

void Parser::parseline(std::string line)
{
	(void)line;
	/*if (line.compare(0, 6, "sphere"))
		parsesphere(line);
	else if (line.compare(0, 8, "cylinder"))
		parsecylinder(line);
	else if (line.compare(0, 4, "cone"))
		parsecone(line);
	else if (line.compare(0, 5, "plane"))
		parseplane(line);
	else if (line.compare(0, 14, "ponctual_light"))
		parseponctuallight(line);
	else if (line.compare(0, 17, "directional_light"))
		parsedirectionallight(line);*/
}
