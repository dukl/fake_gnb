#include "amf_statistics.hpp"
#include "logger.hpp"
#include <iostream>
using namespace std;


void statistics::display(){
  cout<<endl;
  //Logger::amf_app().info("--------------------------------------------------");
  //Logger::amf_app().info("| connected gNBs | connected UEs | registred UEs |");
  //Logger::amf_app().info("--------------------------------------------------");
  //Logger::amf_app().info("|       %d       |       %d      |        %d     |",gNB_connected,UE_connected,UE_registred);
  //Logger::amf_app().info("--------------------------------------------------");
  Logger::amf_app().info("|--------------------------------------------------------------------------------------------------------------------|");
  Logger::amf_app().info("-----------------------------------------------------------------------------------------------------------------");
  Logger::amf_app().info("|----------------------------------------------------gNBs' information--------------------------------------------|");
  for(int i=0; i<gnbs.size(); i++){
    Logger::amf_app().info("[index %d][connected][GlobalID: 0x%x][gnb name: %s][Tracking Area: plmn(%s), tac(%d)]", i+1, gnbs[i].gnb_id, gnbs[i].gnb_name.c_str(), (gnbs[i].mcc+gnbs[i].mnc).c_str(), gnbs[i].tac);
  }
  Logger::amf_app().info("-----------------------------------------------------------------------------------------------------------------"); cout<<endl;
  Logger::amf_app().info("-----------------------------------------------------------------------------------------------------------------");
  Logger::amf_app().info("|----------------------------------------------------UEs' information--------------------------------------------|");
  for(int i=0; i<ues.size();i++){
    Logger::amf_app().info("[index %d][%s][%s][imsi %s][guti %s]", i+1, ues[i].connStatus.c_str(), ues[i].registerStatus.c_str(), ues[i].imsi.c_str(), ues[i].guti.c_str());
    Logger::amf_app().info("Current ran_ue_ngap_id[%d]; Current amf_ue_ngap_id[%d]", ues[i].ranid, ues[i].amfid);
    Logger::amf_app().info("Location[NrCgi][PLMN(%s), cellID(%d)]", (ues[i].mcc+ues[i].mnc).c_str(), ues[i].cellId); cout<<endl;
  }
  Logger::amf_app().info("-----------------------------------------------------------------------------------------------------------------"); 
  Logger::amf_app().info("|--------------------------------------------------------------------------------------------------------------------|"); cout<<endl;
}

statistics::statistics(){
  gNB_connected = 0;
  UE_connected = 0;
  UE_registred = 0;
}

statistics::~statistics(){}
