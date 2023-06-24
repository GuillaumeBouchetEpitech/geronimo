
#pragma once

#include "string-utils/stream-formatter.hpp"

// #include "geronimo/helpers/GLMath.hpp"

#include <cstring> // <= strrchr()
#include <sstream> // <= std::stringstream

#include <cstdint>
#include <mutex>
#include <ostream>
#include <vector>

namespace gero {

class TraceLogger {
public:
  static std::string getTime();

public:
  static void log(const std::string& msg);

private:
  std::mutex _mutex;

  StreamFormatter _formatter;

public:
  TraceLogger() = default;

public:
  StreamFormatter& getStream();

public:
  void dump();
  void dumpErr();
};

} // namespace gero

// this will reduce the "__FILE__" macro to it's filename -> friendlier to read
#define D_MYLOG_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// this is just to make the "D_MYLOG" macro source code easier to read
#define D_MYLOG_STACK D_MYLOG_FILENAME << "|" << __func__ << "|" << __LINE__

#define D_MYLOG_PREFIX "[" << gero::TraceLogger::getTime() << "] (" << D_MYLOG_STACK << ") -> "

// one line logging macro
#define D_MYLOG(streamMsg)                                                                                             \
  {                                                                                                                    \
    gero::TraceLogger logger;                                                                                          \
    logger.getStream() << "MYLOG " << D_MYLOG_PREFIX << streamMsg;                                                     \
    logger.dump();                                                                                                     \
  }

// one line logging macro
#define D_MYERR(streamMsg)                                                                                             \
  {                                                                                                                    \
    gero::TraceLogger logger;                                                                                          \
    logger.getStream() << "MYERR " << D_MYLOG_PREFIX << streamMsg;                                                     \
    logger.dumpErr();                                                                                                  \
  }
