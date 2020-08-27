#ifndef _UE_CONTEXT_H_
#define _UE_CONTEXT_H_

#include <stdint.h>
#include <map>
#include "NgapIEsStruct.hpp"
#include <shared_mutex>

extern "C"{
  #include "Ngap_RRCEstablishmentCause.h"
}

using namespace ngap;

class ue_context{
public:
  uint32_t         ran_ue_ngap_id;//32bits
  long             amf_ue_ngap_id:40;//40bits

  e_Ngap_RRCEstablishmentCause rrc_estb_cause;
  bool isUeContextRequest;
  NrCgi_t cgi;
  Tai_t   tai;
};









#endif
