#ifndef _UE_NGAP_CONTEXT_H_
#define _UE_NGAP_CONTEXT_H_

#include <stdint.h>
#include <map>
#include "gNB_context.hpp"
#include <shared_mutex>

using namespace sctp;
typedef enum{
  NGAP_UE_INVALID_STATE,
  NGAP_UE_WAITING_CSR,//Context Setup Response(CSR)
  NGAP_UE_HANDOVER,
  NGAP_UE_CONNECTED,
  NGAP_UE_WAITING_CRR
}ng_ue_state_t;

class ue_ngap_context{
public:
  uint32_t         ran_ue_ngap_id;//32bits
  long             amf_ue_ngap_id:40;//40bits

  sctp_stream_id_t sctp_stream_recv;// used to decide which ue in gNB
  sctp_stream_id_t sctp_stream_send;// used to decide which ue in gNB
  
  sctp_assoc_id_t  gnb_assoc_id;//to find which gnb this UE belongs to

  bool             ueContextRequest;
  uint32_t         s_tmsi_5g;

  //state management, ue status over the air
  ng_ue_state_t    ng_ue_state; 
};











#endif
