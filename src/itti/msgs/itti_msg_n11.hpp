#ifndef _ITTI_N11_MSG_H_
#define _ITTI_N11_MSG_H_

#include "bstrlib.h"
#include "itti_msg.hpp"
#include <string>

class itti_msg_n11 : public itti_msg{
public:
  itti_msg_n11(const itti_msg_type_t  msg_type, const task_id_t origin, const task_id_t destination):itti_msg(msg_type,origin,destination){}
  itti_msg_n11(const itti_msg_n11& i) : itti_msg(i){
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }

public:
  long amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id;

};

class itti_smf_services_consumer : public itti_msg_n11{
public:
  itti_smf_services_consumer(const task_id_t origin, const task_id_t destination):itti_msg_n11(SMF_SERVICES_CONSUMER, origin, destination){
  }
  itti_smf_services_consumer(const itti_smf_services_consumer& i) : itti_msg_n11(i){
  }
public:
  uint8_t req_type;
  uint8_t pdu_sess_id; 
  bstring dnn;
  bstring sm_msg;

};


class itti_pdu_session_resource_setup_response : public itti_msg_n11{
public:
  itti_pdu_session_resource_setup_response(const task_id_t origin, const task_id_t destination) : itti_msg_n11(PDU_SESS_RES_SET_RESP, origin, destination){}
  itti_pdu_session_resource_setup_response(const itti_pdu_session_resource_setup_response &i) : itti_msg_n11(i){}
public:
  uint8_t pdu_session_id;
  bstring n2sm;
};

class itti_nsmf_pdusession_update_sm_context : public itti_msg_n11{
public:
  itti_nsmf_pdusession_update_sm_context(const task_id_t origin, const task_id_t destination) : itti_msg_n11(NSMF_PDU_SESS_UPDATE_SMCTX, origin, destination){}
  itti_nsmf_pdusession_update_sm_context(const itti_nsmf_pdusession_update_sm_context &i) : itti_msg_n11(i){}
public:
  std::string supi;
  uint8_t pdu_session_id;
  bstring n2sm;
};

class itti_nsmf_pdusession_release_sm_context : public itti_msg_n11 {
public:
	itti_nsmf_pdusession_release_sm_context(const task_id_t origin, const task_id_t destination) : itti_msg_n11(NSMF_PDU_SESS_RELEASE_SMCTX, origin, destination) {}
	itti_nsmf_pdusession_release_sm_context(const itti_nsmf_pdusession_update_sm_context &i) : itti_msg_n11(i) {}
public:
	std::string supi;
};









#endif
