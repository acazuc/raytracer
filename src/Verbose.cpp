#include "Verbose.h"
#include <iostream>
#include <ctime>

Verbose *Verbose::verbose(nullptr);

void Verbose::init()
{
	verbose = new Verbose();
}

void Verbose::clear()
{
	delete (verbose);
}

Verbose::Verbose()
: level(VerboseLevel::VERBOSE_DEBUG)
, first(true)
{
}

Verbose::~Verbose()
{
}

void Verbose::checkFirst()
{
	if (!this->first)
		return;
	this->first = false;
	char buf[256];
	struct tm tm;
	time_t t = time(nullptr);
#ifdef __WIN32
	localtime_s(&tm, &t);
#else
	localtime_r(&t, &tm);
#endif
	strftime(buf, sizeof(buf), "[%c] ", &tm);
	output(buf);
	switch (this->level)
	{
		case VerboseLevel::VERBOSE_DEBUG:
			output("[DEBUG] ");
			break;
		case VerboseLevel::VERBOSE_INFO:
			output("[INFO ] ");
			break;
		case VerboseLevel::VERBOSE_WARN:
			output("[WARN ] ");
			break;
		case VerboseLevel::VERBOSE_ERROR:
			output("[ERROR] ");
			break;
	}
}

void Verbose::flush()
{
	std::cout.flush();
	this->first = true;
}

Verbose &Verbose::output(const std::string &str)
{
	checkFirst();
	std::cout << str;
	return *this;
}

Verbose &Verbose::output(std::string &&str)
{
	return output(str);
}

Verbose &Verbose::operator << (enum VerboseLevel level)
{
	this->level = level;
	return *this;
}

Verbose &Verbose::operator << (const std::string &val)
{
	return output(val);
}

Verbose &Verbose::operator << (std::string &&val)
{
	return output(val);
}

Verbose &Verbose::operator << (const char *val)
{
	return output(val);
}

Verbose &Verbose::operator << (bool val)
{
	return output(val ? "true" : "false");
}

Verbose &Verbose::operator << (char val)
{
	return output(std::string(&val, 1));
}

Verbose &Verbose::operator << (signed char val)
{
	return output(std::string((char*)&val, 1));
}

Verbose &Verbose::operator << (unsigned char val)
{
	return output(std::string((char*)&val, 1));
}

Verbose &Verbose::operator << (int16_t val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (uint16_t val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (int32_t val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (uint32_t val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (int64_t val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (uint64_t val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (float val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (double val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (long double val)
{
	return output(std::to_string(val));
}

Verbose &Verbose::operator << (void *val)
{
	char buf[32];
	snprintf(buf, sizeof(buf), "%llx", (unsigned long long)(intptr_t)val);
	return output(buf);
}

Verbose &Verbose::operator << (std::exception &e)
{
#ifdef __GXX_RTTI
	return *this << typeid(e).name() << ":" << e.what();
#else
	return output(e.what());
#endif
}
