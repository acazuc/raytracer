#ifndef VERBOSE_H
# define VERBOSE_H

# include <cstdint>
# include <string>

# ifndef DEBUG_NO_FILE
#  define VERBOSE_FILE_LINE_FUNC " (" << __FILE__ << ":" << __LINE__ << ") (" << __PRETTY_FUNCTION__ << ")"
#  ifdef NDEBUG
#   define VERBOSE_DEBUG(x) {}
#  else
#   define VERBOSE_DEBUG(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_DEBUG << x << VERBOSE_FILE_LINE_FUNC << "\n").flush();}
#  endif
#  define VERBOSE_INFO(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_INFO << x << VERBOSE_FILE_LINE_FUNC << "\n").flush();}
#  define VERBOSE_WARN(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_WARN << x << VERBOSE_FILE_LINE_FUNC << "\n").flush();}
#  define VERBOSE_ERROR(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_ERROR << x << VERBOSE_FILE_LINE_FUNC << "\n").flush();}
# else
#  ifdef NDEBUG
#   define VERBOSE_DEBUG(x) {}
#  else
#   define VERBOSE_DEBUG(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_DEBUG << x << "\n").flush();}
#  endif
#  define VERBOSE_INFO(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_INFO << x << "\n").flush();}
#  define VERBOSE_WARN(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_WARN << x << "\n").flush();}
#  define VERBOSE_ERROR(x) {(Verbose::getVerbose() << VerboseLevel::VERBOSE_ERROR << x << "\n").flush();}
# endif

enum class VerboseLevel
{
	VERBOSE_DEBUG,
	VERBOSE_INFO,
	VERBOSE_WARN,
	VERBOSE_ERROR,
};

class Verbose
{

	private:
		static Verbose *verbose;
		VerboseLevel level;
		bool first;
		void checkFirst();
		Verbose &output(const std::string &str);
		Verbose &output(std::string &&str);

	public:
		static void init();
		static void clear();
		static Verbose &getVerbose() {return *verbose;};
		Verbose();
		~Verbose();
		void flush();
		Verbose &operator << (enum VerboseLevel level);
		Verbose &operator << (const std::string &val);
		Verbose &operator << (std::string &&val);
		Verbose &operator << (const char *val);
		Verbose &operator << (bool val);
		Verbose &operator << (char val);
		Verbose &operator << (signed char val);
		Verbose &operator << (unsigned char val);
		Verbose &operator << (int16_t val);
		Verbose &operator << (uint16_t val);
		Verbose &operator << (int32_t val);
		Verbose &operator << (uint32_t val);
		Verbose &operator << (int64_t val);
		Verbose &operator << (uint64_t val);
		Verbose &operator << (float val);
		Verbose &operator << (double val);
		Verbose &operator << (long double val);
		Verbose &operator << (void *val);
		Verbose &operator << (std::exception &e);

};

#endif
