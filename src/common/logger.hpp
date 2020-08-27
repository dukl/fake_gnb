#ifndef __LOGGER_H
#define __LOGGER_H

#include <cstdarg>
#include <stdexcept>
#include <vector>

#define SPDLOG_LEVEL_NAMES { "trace", "debug", "info ",  "start", "warn ", "error", "off  " };

#define SPDLOG_ENABLE_SYSLOG
#include "spdlog/spdlog.h"

class LoggerException : public std::runtime_error
{
public:
  explicit LoggerException(const char *m) : std::runtime_error(m) {}
  explicit LoggerException(const std::string &m) : std::runtime_error(m) {}
};

class _Logger
{
public:
  _Logger(const char * category, std::vector<spdlog::sink_ptr> &sinks, const char *pattern);

  void trace(const char *format, ...);
  void trace(const std::string &format, ...);
  void debug(const char *format, ...);
  void debug(const std::string &format, ...);
  void info(const char *format, ...);
  void info(const std::string &format, ...);
  void startup(const char *format, ...);
  void startup(const std::string &format, ...);
  void warn(const char *format, ...);
  void warn(const std::string &format, ...);
  void error(const char *format, ...);
  void error(const std::string &format, ...);

private:
  _Logger();
  enum _LogType
  {
    _ltTrace,
    _ltDebug,
    _ltInfo,
    _ltStartup,
    _ltWarn,
    _ltError
  };

  void log(_LogType lt, const char *format, va_list &args);
  spdlog::logger m_log;
};

class Logger
{
public:

  static void init(const char *app, const bool log_stdout, const bool log_rot_file){singleton()._init(app, log_stdout, log_rot_file);}
  static void init(const std::string &app, const bool log_stdout, const bool log_rot_file){init(app.c_str(), log_stdout, log_rot_file);}

  static _Logger &async_cmd(){return *singleton().m_async_cmd;}
  static _Logger &amf_app(){return *singleton().m_amf_app;}
  static _Logger &config(){return *singleton().m_config;}
  static _Logger &system(){return *singleton().m_system;}
  static _Logger &sctp(){return *singleton().m_sctp;}
  static _Logger &nas_mm(){return *singleton().m_nas_mm;}
  static _Logger &ngap(){return *singleton().m_ngap;}
  static _Logger &itti(){return *singleton().m_itti;}
  static _Logger &amf_n2(){return *singleton().m_amf_n2;}
  static _Logger &task_amf_n2(){return *singleton().m_task_amf_n2;}
  static _Logger &amf_n1(){return *singleton().m_amf_n1;}
  static _Logger &task_amf_n1(){return *singleton().m_task_amf_n1;}
  static _Logger &amf_n11(){return *singleton().m_amf_n11;}
  static _Logger &task_amf_n11(){return *singleton().m_task_amf_n11;}
  static _Logger &amf_server(){return *singleton().m_amf_server;}

private:
  static Logger *m_singleton;
  static Logger &singleton(){if(!m_singleton) m_singleton = new Logger(); return *m_singleton;}

  Logger(){}
  ~Logger(){}

  void _init(const char *app, const bool log_stdout, const bool log_rot_file);

  std::vector<spdlog::sink_ptr> m_sinks;

  std::string m_pattern;

  _Logger *m_async_cmd;
  _Logger *m_amf_app;
  _Logger *m_config;
  _Logger *m_system;
  _Logger *m_sctp;
  _Logger *m_nas_mm;
  _Logger *m_ngap;
  _Logger *m_itti;
  _Logger *m_amf_n2;
  _Logger *m_task_amf_n2;
  _Logger *m_amf_n1;
  _Logger *m_task_amf_n1;
  _Logger *m_amf_n11;
  _Logger *m_task_amf_n11;
  _Logger *m_amf_server;

};

#endif






