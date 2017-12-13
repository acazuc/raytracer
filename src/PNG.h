#ifndef PPNG_H
# define PPNG_H

# include <cstdint>
# include <string>

class PNG
{

	public:
		static bool read(std::string file, uint8_t *&data, uint32_t &width, uint32_t &height);
		static bool write(std::string file, uint8_t *data, uint32_t width, uint32_t height);

};

#endif
