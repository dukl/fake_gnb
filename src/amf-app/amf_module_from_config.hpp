#ifndef _AMF_MODULE_FROM_CONFIG_H_
#define _AMF_MODULE_FROM_CONFIG_H_

#include <arpa/inet.h>
#include <libconfig.h++>
#include <netinet/in.h>
#include <sys/socket.h>

#include <mutex>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "Ngap_ProcedureCode.h"
#include "Ngap_NGAP-PDU.h"


#define MODULES_CONFIG_STRING_AMF_MODULES                               "MODULES"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE                  "NGAP_MESSAGE"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_NAME             "MSG_NAME"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_PROCEDURECODE    "ProcedureCode"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_TYPEOFMSG        "TypeOfMessage"


using namespace libconfig;

namespace config{

class amf_modules{
public:
  int load(const std::string &config_file);
  void display();
  void makeModulesAlive();  
private:
  std::string          msgName;//vector to store more msgs
  Ngap_NGAP_PDU_PR     typeOfMsg;
  Ngap_ProcedureCode_t procedureCode;
  //  NGSetupRequestMsg *ngSetupRequest;

};








}

#endif
