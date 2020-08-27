#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>
#include <string>

using namespace std;

typedef struct{
  uint32_t gnb_id;
  string mcc;
  string mnc;
  string gnb_name;
  uint32_t tac;
  //long nrCellId;
}gnb_infos;

typedef struct{
  string   connStatus;
  string   registerStatus;
  uint32_t ranid;
  long     amfid;
  string   imsi;
  string   guti;
  string   mcc;
  string   mnc;
  uint32_t cellId;
}ue_infos;

class statistics{
public:
  void display();
  statistics();
  ~statistics();
public:
  uint32_t        gNB_connected;
  uint32_t        UE_connected;
  uint32_t        UE_registred;
  //uint32_t        system_pdu_sessions;
  vector<gnb_infos>  gnbs; 
  vector<ue_infos>   ues;

};








#endif
