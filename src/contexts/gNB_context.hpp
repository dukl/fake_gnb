#ifndef _GNB_CONTEXT_H_
#define _GNB_CONTEXT_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "sctp_server.hpp"
#include "NgapIEsStruct.hpp"
extern "C"{
  #include "Ngap_PagingDRX.h"
  #include "bstrlib.h"
}

using namespace sctp;
using namespace std;
using namespace ngap;

enum amf_ng_gnb_state_s{
    NGAP_INIT,
    NGAP_RESETING,
    NGAP_READY,
    NGAP_SHUTDOWN
};
class gnb_context{
public:
  enum amf_ng_gnb_state_s ng_state;
  
  string                  gnb_name;
  long                    globalRanNodeId;
  e_Ngap_PagingDRX        default_paging_drx;  //v32, v64, v128, v256
  vector<SupportedItem_t>   s_ta_list;
  bstring                 ue_radio_cap_ind;
  
  sctp_assoc_id_t         sctp_assoc_id;
  sctp_stream_id_t        next_sctp_stream;
  sctp_stream_id_t        instreams;
  sctp_stream_id_t        outstreams;
  
};















#endif
