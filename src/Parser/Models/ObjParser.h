#ifndef OBJ_PARSER_H
# define OBJ_PARSER_H

# include "./ModelParser.h"

class ObjParser : public ModelParser
{

	private:

	public:
		ObjParser(std::string filename);
		void parse();

};

#endif
