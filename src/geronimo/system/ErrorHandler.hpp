
#pragma once

#include "TraceLogger.hpp"

// one line exception macro
#define D_THROW(exceptionType, exceptionMsg)                                                                           \
  {                                                                                                                    \
    gero::TraceLogger logger;                                                                                          \
    logger.getStream() << D_MYLOG_PREFIX << "[EXCEPTION] type: " #exceptionType << ", msg: " << exceptionMsg;          \
    logger.dump();                                                                                                     \
    throw exceptionType(logger.getStream().getData());                                                                 \
  }
