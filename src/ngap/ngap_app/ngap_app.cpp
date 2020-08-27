#include "sctp_server.hpp"
#include "ngap_app.hpp"
#include "logger.hpp"
#include "amf_module_from_config.hpp"
#include "ngap_message_callback.hpp"

extern "C"{
  #include "Ngap_NGAP-PDU.h"
  #include "Ngap_InitiatingMessage.h"
}

using namespace sctp;
using namespace config;
using namespace ngap;

ngap_app::ngap_app(const string & address, const uint16_t port_num):ppid_(60),sctp_s_38412(address.c_str(),port_num){
  sctp_s_38412.start_receive(this);
  Logger::ngap().info("set n2 amf ipv4_address:port (%s:%d)", address.c_str(), port_num);
}
ngap_app::~ngap_app(){}

/**************************************** received sctp paylaod and decode it to NGAP message and send itti message to TASK_AMF_N2 ***************************************/

void ngap_app::handle_receive(bstring payload, sctp_assoc_id_t assoc_id, sctp_stream_id_t stream, sctp_stream_id_t instreams, sctp_stream_id_t outstreams){
  Logger::ngap().debug("ngap handle sctp payload from sctp_server on assoc_id(%d), stream_id(%d), instreams(%d), outstreams(%d)", assoc_id,stream,instreams,outstreams);
  Ngap_NGAP_PDU_t *ngap_msg_pdu = (Ngap_NGAP_PDU_t*)calloc(1,sizeof(Ngap_NGAP_PDU_t));
  asn_dec_rval_t rc = asn_decode(NULL,ATS_ALIGNED_CANONICAL_PER,&asn_DEF_Ngap_NGAP_PDU,(void**)&ngap_msg_pdu,bdata(payload),blength(payload));
  Logger::ngap().debug("decoded ngap message[%d,%d]",ngap_msg_pdu->choice.initiatingMessage->procedureCode, ngap_msg_pdu->present); 
  (*messages_callback[ngap_msg_pdu->choice.initiatingMessage->procedureCode][ngap_msg_pdu->present - 1]) (assoc_id, stream, ngap_msg_pdu);
}

/***************************************** handle new sctp association *************************************/
// TNL association(clause 8.7.1.1, 3gpp ts38.413)

void ngap_app::handle_sctp_new_association(sctp_assoc_id_t assoc_id, sctp_stream_id_t instreams, sctp_stream_id_t outstreams){
  Logger::ngap().debug("ready to handle new ngap sctp association(id:%d) request",assoc_id);
  std::shared_ptr<gnb_context> gc;
  if(!is_assoc_id_2_gnb_context(assoc_id)) {
    Logger::ngap().debug("Create a new gNB context with assoc_id(%d)",assoc_id);
    gc = std::shared_ptr<gnb_context>(new gnb_context());
    set_assoc_id_2_gnb_context(assoc_id, gc);
  }else{
    gc = assoc_id_2_gnb_context(assoc_id);
    if(gc.get()->ng_state == NGAP_RESETING || gc.get()->ng_state == NGAP_SHUTDOWN){
      Logger::ngap().warn("Received new association request on an association that is being %s, ignoring", ng_gnb_state_str[gc.get()->ng_state]);
    }else{
      Logger::ngap().debug("Update gNB context with assoc id (%d)", assoc_id);
    }
  }
  if(gc.get() == nullptr){
    Logger::ngap().error("Failed to create gNB context for assoc_id(%d)", assoc_id);
  }else{
    gc.get()->sctp_assoc_id = assoc_id;
    gc.get()->instreams = instreams;
    gc.get()->outstreams = outstreams;
    gc.get()->next_sctp_stream = 1;
    gc.get()->ng_state = NGAP_INIT;
  }
}


uint32_t ngap_app::getPpid(){
  return ppid_;
}

/******************************************* gnb context management **********************************************/

bool ngap_app::is_assoc_id_2_gnb_context(const sctp_assoc_id_t & assoc_id) const{
  std::shared_lock lock(m_assoc2gnbContext);
  return bool{assoc2gnbContext.count(assoc_id) > 0};
}

std::shared_ptr<gnb_context>  ngap_app::assoc_id_2_gnb_context(const sctp_assoc_id_t & assoc_id) const{
  std::shared_lock lock(m_assoc2gnbContext);
  return assoc2gnbContext.at(assoc_id);
}

void ngap_app::set_assoc_id_2_gnb_context(const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context> gc){
  std::shared_lock lock(m_assoc2gnbContext);
  assoc2gnbContext[assoc_id] = gc;
} 


