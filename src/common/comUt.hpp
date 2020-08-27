#ifndef _PRINT_BUFFER_H
#define _PRINT_BUFFER_H

#include "logger.hpp"
#include <string>
using namespace std;

void print_buffer(const string app, const string commit, uint8_t *buf, int len){
  if(!app.compare("amf_app"))
    Logger::amf_app().debug(commit.c_str());
  if(!app.compare("amf_n1"))
    Logger::amf_n1().debug(commit.c_str());
  if(!app.compare("amf_server"))
    Logger::amf_server().debug(commit.c_str());
  if(!app.compare("amf_n11"))
    Logger::amf_n11().debug(commit.c_str());
  for(int i=0; i<len; i++)
    printf("%x ",buf[i]);
  printf("\n");   
}

void hexStr2Byte(const char* src, unsigned char *dest, int len){
  short i;
  unsigned char hBy, lBy;
  for(i=0; i<len; i+=2){
    hBy = toupper(src[i]);
    lBy = toupper(src[i+1]);
    if(hBy > 0x39)
      hBy -= 0x37;
    else
      hBy -= 0x30;
    if(lBy > 0x39)
      lBy -= 0x37;
    else
      lBy -= 0x30;
    dest[i/2] = (hBy<<4) | lBy;
  }
}








#endif
