#ifndef SHELL_LOG_LOG_H_
#define SHELL_LOG_LOG_H_

namespace logging {

enum class LogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

void log(LogLevel level, const char* fmt, ...);

void debug(const char* fmt, ...);
void info(const char* fmt, ...);
void warn(const char* fmt, ...);
void error(const char* fmt, ...);
void fatal(const char* fmt, ...);

} // namespace log

#endif

