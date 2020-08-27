#include "logger.hpp"
#include "spdlog/sinks/syslog_sink.h"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

Logger *Logger::m_singleton = NULL;

void Logger::_init(const char *app, const bool log_stdout, const bool log_rot_file){
  int num_sinks = 0;
  spdlog::set_async_mode(2048);
#if TRACE_IS_ON
  spdlog::level::level_enum llevel = spdlog::level::trace;
#elif DEBUG_IS_ON
  spdlog::level::level_enum llevel = spdlog::level::debug;
#elif INFO_IS_ON
  spdlog::level::level_enum llevel = spdlog::level::info;
#else
  spdlog::level::level_enum llevel = spdlog::level::warn;
#endif

  if(log_stdout){
    std::string filename = fmt::format("./{}.log",app);
    m_sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
    m_sinks[num_sinks++].get()->set_level(llevel);
  }
  if(log_rot_file){
    std::string filename = fmt::format("./{}.log",app);
    m_sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, 5*1024*1024, 3));
  }

  std::stringstream ss;
  ss << "[%Y-%m-%dT%H:%M:%S.%f] [" << app << "] [%n] [%l] %v";

  m_async_cmd = new _Logger("asnyc_c", m_sinks, ss.str().c_str());
  m_amf_app = new _Logger("amf_app", m_sinks, ss.str().c_str());
  m_config = new _Logger("configurations", m_sinks, ss.str().c_str());
  m_system = new _Logger("system", m_sinks, ss.str().c_str());
  m_sctp = new _Logger("sctp", m_sinks, ss.str().c_str());
  m_nas_mm = new _Logger("nas_mm", m_sinks, ss.str().c_str());
  m_ngap = new _Logger("ngap", m_sinks, ss.str().c_str());
  m_itti = new _Logger("itti", m_sinks, ss.str().c_str());
  m_amf_n2 = new _Logger("amf_n2", m_sinks, ss.str().c_str());
  m_task_amf_n2 = new _Logger("TASK_AMF_N2", m_sinks, ss.str().c_str());
  m_amf_n1 = new _Logger("amf_n1", m_sinks, ss.str().c_str());
  m_task_amf_n1 = new _Logger("TASK_AMF_N1", m_sinks, ss.str().c_str());
  m_amf_n11 = new _Logger("amf_n11", m_sinks, ss.str().c_str());
  m_task_amf_n11 = new _Logger("TASK_AMF_N11", m_sinks, ss.str().c_str());
  m_amf_server = new _Logger("amf_server", m_sinks, ss.str().c_str());

}

_Logger::_Logger( const char *category, std::vector<spdlog::sink_ptr> &sinks, const char *pattern )
	   : m_log( category, sinks.begin(), sinks.end() )
{
  m_log.set_pattern( pattern );
#if TRACE_IS_ON
  m_log.set_level( spdlog::level::trace );
#elif DEBUG_IS_ON
  m_log.set_level( spdlog::level::debug );
#elif INFO_IS_ON
  m_log.set_level( spdlog::level::info );
#else
  m_log.set_level( spdlog::level::warn );
#endif
}

void _Logger::trace( const char *format, ... )
{
#if TRACE_IS_ON
  va_list args;
  va_start( args, format );
  log( _ltTrace, format, args );
  va_end( args );
#endif
}

void _Logger::trace( const std::string &format, ... )
{
#if TRACE_IS_ON
  va_list args;
  va_start( args, format );
  log( _ltTrace, format.c_str(), args );
  va_end( args );
#endif
}

void _Logger::debug( const char *format, ... )
{
#if DEBUG_IS_ON
  va_list args;
  va_start( args, format );
  log( _ltDebug, format, args );
  va_end( args );
#endif
}

void _Logger::debug( const std::string &format, ... )
{
#if DEBUG_IS_ON
  va_list args;
  va_start( args, format );
  log( _ltDebug, format.c_str(), args );
  va_end( args );
#endif
}

void _Logger::info( const char *format, ... )
{
#if INFO_IS_ON
  va_list args;
  va_start( args, format );
  log( _ltInfo, format, args );
  va_end( args );
#endif
}

void _Logger::info( const std::string &format, ... )
{
#if INFO_IS_ON
  va_list args;
  va_start( args, format );
  log( _ltInfo, format.c_str(), args );
  va_end( args );
#endif
}

void _Logger::startup( const char *format, ... )
{
  va_list args;
  va_start( args, format );
  log( _ltStartup, format, args );
  va_end( args );
}

void _Logger::startup( const std::string &format, ... )
{
  va_list args;
  va_start( args, format );
  log( _ltStartup, format.c_str(), args );
  va_end( args );
}

void _Logger::warn( const char *format, ... )
{
  va_list args;
  va_start( args, format );
  log( _ltWarn, format, args );
  va_end( args );
}

void _Logger::warn( const std::string &format, ... )
{
  va_list args;
  va_start( args, format );
  log( _ltWarn, format.c_str(), args );
  va_end( args );
}

void _Logger::error( const char *format, ... )
{
  va_list args;
  va_start( args, format );
  log( _ltError, format, args );
  va_end( args );
}

void _Logger::error( const std::string &format, ... )
{
  va_list args;
  va_start( args, format );
  log( _ltError, format.c_str(), args );
  va_end( args );
}

void _Logger::log( _LogType lt, const char *format, va_list &args )
{
  char buffer[ 2048 ];

  vsnprintf( buffer, sizeof(buffer), format, args );

  switch ( lt )
  {
    case _ltTrace: m_log.trace( buffer ); break;
    case _ltDebug: m_log.debug( buffer ); break;
    case _ltInfo: m_log.info( buffer ); break;
    case _ltStartup: m_log.warn( buffer ); break;
    case _ltWarn: m_log.error( buffer ); break;
    case _ltError: m_log.critical( buffer ); break;
  }
}






















