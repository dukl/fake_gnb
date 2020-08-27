#include "amf_app.hpp"
#include "itti.hpp"
#include "ngap_app.hpp"
#include "amf_config.hpp"
#include "amf_n2.hpp"
#include "amf_n1.hpp"
#include "amf_n11.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include "amf_statistics.hpp"

#include "DLNASTransport.hpp"

using namespace ngap;
using namespace nas;
using namespace amf_application;
using namespace config;

extern void print_buffer(const std::string app, const std::string commit, uint8_t *buf, int len);
extern amf_app * amf_app_inst;
extern itti_mw * itti_inst;
amf_n2 * amf_n2_inst = nullptr;
amf_n1 * amf_n1_inst = nullptr;
amf_n11 * amf_n11_inst = nullptr;
extern amf_config amf_cfg;
extern statistics stacs;

void amf_app_task(void*);

uint32_t golbal_tmsi = 1;

amf_app::amf_app(const amf_config &amf_cfg){
  Logger::amf_app().startup("Creating amf application functionality layer");
  if(itti_inst->create_task(TASK_AMF_APP, amf_app_task, nullptr)){
    Logger::amf_app().error( "Cannot create task TASK_AMF_APP" );
    throw std::runtime_error( "Cannot create task TASK_AMF_APP" );
  }
  try{
    amf_n2_inst = new amf_n2(std::string(inet_ntoa(amf_cfg.n2.addr4)),amf_cfg.n2.port); 
  }catch(std::exception& e){
    Logger::amf_app().error( "Cannot create amf n2 interface: %s", e.what() );
    throw;
  }
  try{
    amf_n1_inst = new amf_n1();
  }catch(std::exception& e){
    Logger::amf_app().error( "Cannot create amf n1 interface: %s", e.what() );
  }
  try{
    amf_n11_inst = new amf_n11();
  }catch(std::exception& e){
    Logger::amf_app().error( "Cannot create amf n11 interface: %s", e.what() );
  }
  timer_id_t tid = itti_inst->timer_setup(amf_cfg.statistics_interval,0,TASK_AMF_APP,TASK_AMF_APP_PERIODIC_STATISTICS,0);
  Logger::amf_app().startup( "Started timer(%d)", tid);
}

void amf_app::allRegistredModulesInit(const amf_modules & modules){
  Logger::amf_app().info("Initiating all registred modules");
}

void amf_app_task(void*){
  const task_id_t task_id = TASK_AMF_APP;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto *msg = shared_msg.get();
    timer_id_t tid;
    switch(msg->msg_type){
      case NAS_SIG_ESTAB_REQ:{
        Logger::amf_app().debug("Received NAS_SIG_ESTAB_REQ");
        itti_nas_signalling_establishment_request *m = dynamic_cast<itti_nas_signalling_establishment_request*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      }break;

      case N1N2_MESSAGE_TRANSFER_REQ:{
        Logger::amf_app().debug("Received N1N2_MESSAGE_TRANSFER_REQ");
        itti_n1n2_message_transfer_request *m = dynamic_cast<itti_n1n2_message_transfer_request*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      }break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          switch(to->arg1_user){
            case TASK_AMF_APP_PERIODIC_STATISTICS:
              tid = itti_inst->timer_setup(amf_cfg.statistics_interval,0,TASK_AMF_APP,TASK_AMF_APP_PERIODIC_STATISTICS,0);
              //Logger::amf_app().info("statistics(ready to be implemented)");
              stacs.display();
              break;
            default:
              Logger::amf_app().info( "no handler for timer(%d) with arg1_user(%d) ", to->timer_id, to->arg1_user);
          }
      }
      break;
      default:
        Logger::amf_app().info( "no handler for msg type %d", msg->msg_type);
    }
  }while(true);
}

long amf_app::generate_amf_ue_ngap_id(){
  long tmp = 0;
  tmp = __sync_fetch_and_add(&amf_app_ue_ngap_id_generator,1);
  return tmp & 0xffffffffff;
}

/****************************** context management **************************/
bool amf_app::is_amf_ue_id_2_ue_context(const long & amf_ue_ngap_id) const {
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  return bool{amf_ue_ngap_id2ue_ctx.count(amf_ue_ngap_id) > 0};
}

std::shared_ptr<ue_context> amf_app::amf_ue_id_2_ue_context(const long & amf_ue_ngap_id) const {
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  return amf_ue_ngap_id2ue_ctx.at(amf_ue_ngap_id);
}

void amf_app::set_amf_ue_ngap_id_2_ue_context(const long & amf_ue_ngap_id, std::shared_ptr<ue_context> uc){
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  amf_ue_ngap_id2ue_ctx[amf_ue_ngap_id] = uc;
}

bool amf_app::is_ran_amf_id_2_ue_context(const string & ue_context_key) const {
  std::shared_lock lock(m_ue_ctx_key);
  return bool{ue_ctx_key.count(ue_context_key) > 0};
}

std::shared_ptr<ue_context> amf_app::ran_amf_id_2_ue_context(const string & ue_context_key) const{
  std::shared_lock lock(m_ue_ctx_key);
  return ue_ctx_key.at(ue_context_key);
}

void amf_app::set_ran_amf_id_2_ue_context(const string & ue_context_key, std::shared_ptr<ue_context> uc){
  std::shared_lock lock(m_ue_ctx_key);
  ue_ctx_key[ue_context_key] = uc;
}
/****************************** itti handlers *******************************/
void amf_app::handle_itti_message(itti_n1n2_message_transfer_request & itti_msg){
  //1. encode DL NAS TRANSPORT message(NAS message)
  DLNASTransport * dl = new DLNASTransport();
  dl->setHeader(PLAIN_5GS_MSG);
  dl->setPayload_Container_Type(N1_SM_INFORMATION);
  dl->setPayload_Container((uint8_t*)bdata(itti_msg.n1sm), blength(itti_msg.n1sm));
  dl->setPDUSessionId(itti_msg.pdu_session_id);
  uint8_t nas[1024];
  int encoded_size = dl->encode2buffer(nas, 1024);
  print_buffer("amf_app", "n1n2 transfer", nas, encoded_size);
  bstring dl_nas = blk2bstr(nas,encoded_size);

  itti_downlink_nas_transfer * dl_msg = new itti_downlink_nas_transfer(TASK_AMF_APP, TASK_AMF_N1);
  dl_msg->dl_nas = dl_nas;
  if(!itti_msg.is_n2sm_set){
    dl_msg->is_n2sm_set = false;
  }else{
    dl_msg->n2sm = itti_msg.n2sm;
    dl_msg->pdu_session_id = itti_msg.pdu_session_id;
    dl_msg->is_n2sm_set = true;
  }
  dl_msg->amf_ue_ngap_id = amf_n1_inst->supi2amfId.at(itti_msg.supi); 
  dl_msg->ran_ue_ngap_id = amf_n1_inst->supi2ranId.at(itti_msg.supi); 
  std::shared_ptr<itti_downlink_nas_transfer> i = std::shared_ptr<itti_downlink_nas_transfer>(dl_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_app().error( "Could not send ITTI message %s to task TASK_AMF_N1", i->get_msg_name());
  }
}

void amf_app::handle_itti_message(itti_nas_signalling_establishment_request & itti_msg){
//1. generate amf_ue_ngap_id
//2. establish ue_context associated with amf_ue_ngap_id
//3. store ue-reated core information
//4. send nas-pdu to task_amf_n1
  long amf_ue_ngap_id = 0;
  std::shared_ptr<ue_context> uc;
  //check ue context with 5g-s-tmsi

  if(amf_ue_ngap_id = itti_msg.amf_ue_ngap_id == -1){
    amf_ue_ngap_id = generate_amf_ue_ngap_id();
  }
  string ue_context_key = "app_ue_ranid_"+to_string(itti_msg.ran_ue_ngap_id)+":amfid_"+to_string(amf_ue_ngap_id);
  //if(!is_amf_ue_id_2_ue_context(amf_ue_ngap_id)){
  if(!is_ran_amf_id_2_ue_context(ue_context_key)){
    Logger::amf_app().debug("no existed ue_context, Create one with ran_amf_id(%s)", ue_context_key.c_str());
    uc = std::shared_ptr<ue_context>(new ue_context());
    //set_amf_ue_ngap_id_2_ue_context(amf_ue_ngap_id, uc);
    set_ran_amf_id_2_ue_context(ue_context_key, uc);
  }
  if(uc.get() == nullptr){
    Logger::amf_app().error("Failed to create ue_context with ran_amf_id(%s)", ue_context_key.c_str());
  }else{
    uc.get()->cgi = itti_msg.cgi;
    uc.get()->tai = itti_msg.tai;
    if(itti_msg.rrc_cause != -1)
      uc.get()->rrc_estb_cause = (e_Ngap_RRCEstablishmentCause)itti_msg.rrc_cause;
    if(itti_msg.ueCtxReq == -1)
      uc.get()->isUeContextRequest = false;
    else
      uc.get()->isUeContextRequest = true;
    uc.get()->ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;
    uc.get()->amf_ue_ngap_id = amf_ue_ngap_id;

    std::string guti; bool is_guti_valid = false;
    if(itti_msg.is_5g_s_tmsi_present){
      guti = itti_msg.tai.mcc + itti_msg.tai.mnc + amf_cfg.guami.regionID + itti_msg._5g_s_tmsi;
      is_guti_valid = true;
      Logger::amf_app().debug("Receiving guti: %s", guti.c_str());
    }

    itti_uplink_nas_data_ind * itti_n1_msg = new itti_uplink_nas_data_ind(TASK_AMF_APP, TASK_AMF_N1);
    itti_n1_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_n1_msg->ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;
    itti_n1_msg->is_nas_signalling_estab_req = true;
    itti_n1_msg->nas_msg = itti_msg.nas_buf;
    itti_n1_msg->mcc = itti_msg.tai.mcc;
    itti_n1_msg->mnc = itti_msg.tai.mnc;
    itti_n1_msg->is_guti_valid = is_guti_valid;
    if(is_guti_valid){
      itti_n1_msg->guti = guti;
    }
    std::shared_ptr<itti_uplink_nas_data_ind> i = std::shared_ptr<itti_uplink_nas_data_ind>(itti_n1_msg);
    int ret = itti_inst->send_msg(i);
      if (0 != ret) {
      Logger::amf_app().error( "Could not send ITTI message %s to task TASK_AMF_N1", i->get_msg_name());
    }

  }

}


/************************ SMF Client response handlers *****************************/
void amf_app::handle_post_sm_context_response_error_400(){
  Logger::amf_app().error("post sm context response error 400");
}

bool amf_app::generate_5g_guti(uint32_t ranid, long amfid, string &mcc, string &mnc, uint32_t& tmsi){
  string ue_context_key = "app_ue_ranid_"+to_string(ranid)+":amfid_"+to_string(amfid); 
  if(!is_ran_amf_id_2_ue_context(ue_context_key)){
    Logger::amf_app().error("no ue context for ran_amf_id(%s), exit", ue_context_key.c_str());
    return false;
  }
  std::shared_ptr<ue_context> uc;
  uc = ran_amf_id_2_ue_context(ue_context_key);
  mcc = uc.get()->tai.mcc;
  mnc = uc.get()->tai.mnc;
  tmsi = golbal_tmsi;
  golbal_tmsi ++;
  return true;
} 
