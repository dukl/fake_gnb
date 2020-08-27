#include "amf_module_from_config.hpp"
#include "logger.hpp"
#include <iostream>
#include <string>
using namespace std;


namespace config{

int amf_modules::load(const std::string &config_file){
  cout<<endl;
  Logger::amf_app().debug("Load amf  module configuration file(%s)",config_file.c_str());
  Config                 cfg;
  try{
    cfg.readFile(config_file.c_str());
  }catch(const FileIOException &fioex){
    Logger::amf_app().error("I/O error while reading file %s - %s", config_file.c_str(), fioex.what());
    throw;
  }catch(const ParseException &pex){
    Logger::amf_app().error("Parse error at %s:%d - %s", pex.getFile(), pex.getLine(), pex.getError());
    throw;
  }
  const Setting &root = cfg.getRoot();
  try{
    const Setting& modules = root[MODULES_CONFIG_STRING_AMF_MODULES];
  }catch(const SettingNotFoundException &nfex){
    Logger::amf_app().error("%s : %s", nfex.what(), nfex.getPath());
    return -1;
  }
  const Setting &modules = root[MODULES_CONFIG_STRING_AMF_MODULES];
  const Setting &msg = modules[MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE];
  int count = msg.getLength();
  for(int i=0; i< count; i++){
    const Setting & item = msg[i];
    std::string typeOfMessage;
    int procedure_code;
    item.lookupValue(MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_NAME, msgName);
    item.lookupValue(MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_PROCEDURECODE, procedure_code);
    item.lookupValue(MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_TYPEOFMSG, typeOfMessage);
    procedureCode = (Ngap_ProcedureCode_t)procedure_code;
    if(!(typeOfMessage.compare("initialMessage"))){
      typeOfMsg = Ngap_NGAP_PDU_PR_initiatingMessage;
    }else if(!(typeOfMessage.compare("successfuloutcome"))){
      typeOfMsg = Ngap_NGAP_PDU_PR_successfulOutcome;
    }else if(!(typeOfMessage.compare("unsuccessfuloutcome"))){
      typeOfMsg = Ngap_NGAP_PDU_PR_unsuccessfulOutcome;
    }else{
      Logger::config().error("wrong NGAP message configuration");
    }
  }
}

void amf_modules::display(){
  Logger::config().info( "=======    AMF Registred Modules   =======");
  Logger::config().info( "NGAP Message Modules Repository(SourceCode) Path( ~/oai-5g-amf/src/ngap/ngapMsgs )");
  Logger::config().info( "NGAP Message Modules:");
  Logger::config().info( "- %s([%d,%d])\n", msgName.c_str(), procedureCode, typeOfMsg);
}







}
