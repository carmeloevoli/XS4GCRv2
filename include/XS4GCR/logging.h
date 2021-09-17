#ifndef INCLUDE_XS4GCR_LOGGING_H
#define INCLUDE_XS4GCR_LOGGING_H

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include "XS4GCR/git_revision.h"

namespace XS4GCR {
namespace LOG {

void startup_information() {
  static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("log.csv", 8000, 3);
  static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
#ifdef DEBUG
  plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);
#else
  plog::init(plog::info, &fileAppender).addAppender(&consoleAppender);
#endif
  LOGI << "Welcome to XS4GCR version " << get_version();
  LOGI << "was built on " << __DATE__ << " at " << __TIME__;
  LOGI << "git version is " << git_sha1();
  LOGW << "has local changes " << git_has_local_changes();
}

}  // namespace LOG
}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_LOGGING_H