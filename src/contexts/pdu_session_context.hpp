#ifndef _PDU_SESSION_CONTEXT_H_
#define _PDU_SESSION_CONTEXT_H_

#include <string>
#include "bstrlib.h"
using namespace std;

class pdu_session_context{
public:
  pdu_session_context();
  ~pdu_session_context();
public:
  uint32_t ran_ue_ngap_id;
  long     amf_ue_ngap_id;
  uint8_t req_type;
  uint8_t pdu_session_id;
  bstring n2sm;
  bool isn2sm_avaliable;
  bstring n1sm;
  bool isn1sm_avaliable;
  string dnn;
  string remote_smf_addr[0];//"192.168.12.10:8080"
  bool smf_avaliable;
  
  string location;
};













#endif
