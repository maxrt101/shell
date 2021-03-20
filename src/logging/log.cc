#include "logging/log.h"

#include <mutex>
#include <cstdio>
#include <cstdarg>

static std::mutex log_mutex;


static void flog(logging::LogLevel level, const char* fmt, va_list args) {
  log_mutex.lock();
  FILE* file = stdout;
  if (level > logging::LogLevel::INFO) file = stderr;

  switch (level) {
    case logging::LogLevel::DEBUG:
      fprintf(file, "\x1b[34mDEBUG: ");
      break;
    case logging::LogLevel::INFO:
      fprintf(file, "INFO: ");
      break;
    case logging::LogLevel::WARN:
      fprintf(file, "\x1b[1;33mWARN: ");
      break;
    case logging::LogLevel::ERROR:
      fprintf(file, "\x1b[31mERROR: ");
      break;
    case logging::LogLevel::FATAL:
      fprintf(file, "\x1b[1;31mFATAL: ");
      break;
  }

  fprintf(file, "\x1b[0m");
  vfprintf(file, fmt, args);
  fprintf(file, "\n");
  fflush(file);
  log_mutex.unlock();
}

void logging::log(logging::LogLevel level, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flog(level, fmt, args);
  va_end(args);
}

void logging::debug(const char* fmt, ...) {
#ifdef _DEBUG
  va_list args;
  va_start(args, fmt);
  flog(LogLevel::DEBUG, fmt, args);
  va_end(args);
#endif
}

void logging::info(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flog(LogLevel::INFO, fmt, args);
  va_end(args);
}

void logging::warn(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flog(LogLevel::WARN, fmt, args);
  va_end(args);
}

void logging::error(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flog(LogLevel::ERROR, fmt, args);
  va_end(args);
}

void logging::fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flog(LogLevel::FATAL, fmt, args);
  va_end(args);
}

