#ifndef PARSER_H
# define PARSER_H

# include "Objects/Object.h"
# include "Lights/Light.h"
# include <fstream>
# include <vector>

class Parser
{

	private:
		std::vector<Object*> objects;
		std::vector<Light*> lights;
		std::ifstream ifs;
		std::string trim(std::string);
		void parseline(std::string line);
		void parsesphere(std::string line);
		void parsecylinder(std::string line);
		void parsecone(std::string line);
		void parseplane(std::string line);
		void parseponctuallight(std::string line);
		void parsedirectionallight(std::string line);

	public:
		Parser(std::string filename);
		void parse();

};

#endif
