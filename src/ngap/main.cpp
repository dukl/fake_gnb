#include <iostream>
#include <thread>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h> 
#include <unistd.h>

#include "logger.hpp"
#include "options.hpp"
#include "amf_config.hpp"
#include "ngap_app.hpp"

using namespace std;
using namespace config;
using namespace ngap;

amf_config amf_cfg;
ngap_app * ngap_inst = NULL;

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

  ngap_inst = new ngap_app("10.103.238.20",38412);

  Logger::amf_app().debug("Initiating Done!");
  pause();
  return 0;
}
