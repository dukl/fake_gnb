#include <iostream>
#include <thread>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h> 
#include <unistd.h>

#include "AMFApiServer.hpp"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

#include "logger.hpp"
#include "options.hpp"
#include "amf_config.hpp"
#include "ngap_app.hpp"
#include "itti.hpp"
#include "amf_app.hpp"
#include "amf_module_from_config.hpp"
#include "amf_statistics.hpp"

#include "SMFClientApi.hpp"
#include "test.hpp"
#include "smf-client.hpp"

#include <string>
#include <cstring>
#include "normalizer.hh"

extern void hexStr2Byte(const char* src, unsigned char *dest, int len);
extern void print_buffer(const std::string app, const std::string commit, uint8_t *buf, int len);
extern void ue_gnb_simulator();

using namespace std;
using namespace config;
//using namespace ngap;
using namespace amf_application;

amf_config amf_cfg;
amf_modules modules;
//ngap_app * ngap_inst = NULL;
itti_mw *itti_inst = nullptr;
amf_app *amf_app_inst = nullptr;
statistics stacs;


int main(int argc, char **argv){
  srand (time(NULL));

  if(!Options::parse(argc, argv)){
    cout<<"Options::parse() failed"<<endl;
    return 1;
  }

  Logger::init( "amf" , Options::getlogStdout() , Options::getlogRotFilelog());
  Logger::amf_app().startup("Options parsed!");

  amf_cfg.load(Options::getlibconfigConfig());
  amf_cfg.display();
  modules.load(Options::getlibconfigConfig());
  modules.display();
  
  itti_inst = new itti_mw();
  itti_inst->start(amf_cfg.itti.itti_timer_sched_params);
  //itti_inst->start();

  amf_app_inst = new amf_app(amf_cfg);
  amf_app_inst->allRegistredModulesInit(modules);

  Logger::amf_app().debug("initiating amf server endpoints");
  Pistache::Address addr(std::string(inet_ntoa (*((struct in_addr *)&amf_cfg.n2.addr4))) , Pistache::Port(8282));  
  AMFApiServer amfApiServer(addr, amf_app_inst);
  amfApiServer.init(2);
  std::thread amf_api_manager(&AMFApiServer::start, amfApiServer);

  //hexTest();
  //send_pdu_session_establishment_request();
  //sleep(5);
  //send_pdu_session_update_sm_context_establishment();
  //send_pdu_session_update_sm_context_establishment();
  //smf_client_test();
  //f1_test();
  //kdf_test();
/*
  string snn = "5G:mnc011.mcc460.3gppnetwork.org";
  UNF::Normalizer norm;
  UNF::Normalizer::Form form = UNF::Normalizer::FORM_NFKC;
  cout <<"unicode normallize: "<<norm.normalize(snn.c_str(), form)<<endl;
*/
  
  //rijndael_test();
  //f1_test();
  //f2345_test();
  //kdf_test();
  //res_test();
  //xresStar_test();
  //nia1_test();
  //nia2_test();
  //unsigned char dest[8];
  //hexStr2Byte("460110123456789", dest, 16);
  //print_buffer("amf_n1", "result", dest, 8);
  //buffer_test();
  //mac_test();
  //new_mac_test();
  //ue_gnb_simulator();
  //istream_test();

  Logger::amf_app().debug("Initiating Done!");
  pause();
  return 0;
}
