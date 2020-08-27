//#include <vector>
#include "amf_n2.hpp"
#include "amf_n1.hpp"
#include "amf_app.hpp"
#include "logger.hpp"
#include "sctp_server.hpp"
#include "itti.hpp"
#include "itti_msg_amf_app.hpp"
#include "amf_config.hpp"
#include "DefaultPagingDRX.hpp"
#include "NGSetupFailure.hpp"
#include "NGSetupResponse.hpp"
#include "DownLinkNasTransport.hpp"
#include "InitialContextSetupRequest.hpp"
#include "PduSessionResourceSetupRequest.hpp"
#include "UEContextReleaseCommand.hpp"

#include "PDUSessionResourceReleaseCommandTransfer.hpp"
#include "amf_statistics.hpp"

#include "Ngap_Cause.h"
#include "Ngap_CauseRadioNetwork.h"
#include "Ngap_TimeToWait.h"
#include "Ngap_CauseNas.h"
using namespace amf_application;
using namespace std;
using namespace config;
extern itti_mw * itti_inst;
extern amf_n2  * amf_n2_inst;
extern amf_n1  * amf_n1_inst;
extern amf_config amf_cfg; 
extern amf_app *amf_app_inst;
extern statistics stacs;
 
  void amf_n2_task(void*);
  void amf_n2_task(void *args_p){
    const task_id_t task_id = TASK_AMF_N2;
    itti_inst->notify_task_ready(task_id);
    do{
      std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
      auto *msg = shared_msg.get();
      switch(msg->msg_type){
        case NEW_SCTP_ASSOCIATION:{
          Logger::task_amf_n2().info("Received NEW_SCTP_ASSOCIATION");
          itti_new_sctp_association *m = dynamic_cast<itti_new_sctp_association*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        case NG_SETUP_REQ:{
          Logger::task_amf_n2().info("Received NGSetupRequest message, handling");
          itti_ng_setup_request *m = dynamic_cast<itti_ng_setup_request*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        case INITIAL_UE_MSG:{
          Logger::task_amf_n2().info("Received INITIAL_UE_MESSAGE message, handling");
          itti_initial_ue_message *m = dynamic_cast<itti_initial_ue_message*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        case ITTI_UL_NAS_TRANSPORT:{
          Logger::task_amf_n2().info("Received UPLINK_NAS_TRANSPORT message, handling");
          itti_ul_nas_transport *m = dynamic_cast<itti_ul_nas_transport*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        case ITTI_DL_NAS_TRANSPORT:{
          Logger::task_amf_n2().info("Encoding DOWNLINK NAS TRANSPORT message, sending ");
          itti_dl_nas_transport *m = dynamic_cast<itti_dl_nas_transport*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        case PDU_SESSION_RESOURCE_SETUP_REQUEST:{
          Logger::task_amf_n2().info("Encoding PDU SESSION RESOURCE SETUP REQUEST message, sending ");
          itti_pdu_session_resource_setup_request *m = dynamic_cast<itti_pdu_session_resource_setup_request*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        case INITIAL_CONTEXT_SETUP_REQUEST:{
          Logger::task_amf_n2().info("Encoding INITIAL CONTEXT SETUP REQUEST message, sending ");
          itti_initial_context_setup_request *m = dynamic_cast<itti_initial_context_setup_request*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        case UE_CONTEXT_RELEASE_REQUEST:{
          Logger::task_amf_n2().info("Received UE_CONTEXT_RELEASE_REQUEST message, handling");
          itti_ue_context_release_request *m = dynamic_cast<itti_ue_context_release_request*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break; 
        case UE_CONTEXT_RELEASE_COMMAND: {
			Logger::task_amf_n2().info("Received UE_CONTEXT_RELEASE_COMMAND message, handling");
			itti_ue_context_release_command *m = dynamic_cast<itti_ue_context_release_command*>(msg);
			amf_n2_inst->handle_itti_message(ref(*m));
	}break;
	case PDU_SESSION_RESOURCE_RELEASE_COMMAND: {
			Logger::task_amf_n2().info("Received PDU_SESSION_RESOURCE_RELEASE_COMMAND message, handling");
			itti_pdu_session_resource_release_command *m = dynamic_cast<itti_pdu_session_resource_release_command*>(msg);
			amf_n2_inst->handle_itti_message(ref(*m));
	}break;
        case UE_RADIO_CAP_IND:{
          Logger::task_amf_n2().info("Received UE_RADIO_CAP_IND message, handling");
          itti_ue_radio_capability_indication *m = dynamic_cast<itti_ue_radio_capability_indication*>(msg);
          amf_n2_inst->handle_itti_message(ref(*m));
        }break;
        default:
          Logger::task_amf_n2().info( "no handler for msg type %d", msg->msg_type);
      }
    }while(true);
  }

  amf_n2::amf_n2(const string &address, const uint16_t port_num) : ngap_app(address, port_num){
    if (itti_inst->create_task(TASK_AMF_N2, amf_n2_task, nullptr) ) {
      Logger::amf_n2().error( "Cannot create task TASK_AMF_N2" );
      throw std::runtime_error( "Cannot create task TASK_AMF_N2" );
    }
    Logger::task_amf_n2().startup( "Started" );
    Logger::task_amf_n2().debug("construct amf_n2 successfully");
  }
  amf_n2::~amf_n2(){}



/******************************************************** NGAP Messages Handlers******************************************************************/

void amf_n2::handle_itti_message(itti_new_sctp_association &new_assoc){}//handled in class ngap_app
/************************* NG_SETUP_REQUEST Handler **************************/
void amf_n2::handle_itti_message(itti_ng_setup_request & itti_msg){
  Logger::amf_n2().debug("parameters(assoc_id(%d))(stream(%d))",itti_msg.assoc_id, itti_msg.stream);

  std::shared_ptr<gnb_context> gc;
  if(!is_assoc_id_2_gnb_context(itti_msg.assoc_id)) {
    Logger::amf_n2().error("no existed gnb context with assoc_id(%d)",itti_msg.assoc_id);
    return;
  }
  gc = assoc_id_2_gnb_context(itti_msg.assoc_id);
  if(gc.get()->ng_state == NGAP_RESETING || gc.get()->ng_state == NGAP_SHUTDOWN){
    Logger::amf_n2().warn("Received new association request on an association that is being %s, ignoring", ng_gnb_state_str[gc.get()->ng_state]);
  }else{   
    Logger::amf_n2().debug("Update gNB context with assoc id (%d)", itti_msg.assoc_id);
  } 

  gnb_infos gnbItem;

  //Get IE Global RAN Node ID
  uint32_t gnb_id; string gnb_mcc; string gnb_mnc;
  if(!itti_msg.ngSetupReq->getGlobalGnbID(gnb_id, gnb_mcc, gnb_mnc)){
    Logger::amf_n2().error("Missing Mandontary IE GlobalGnbID");
    return;
  }
  Logger::amf_n2().debug("IE GlobalGNBID(0x%x)",gnb_id);
  gc->globalRanNodeId = gnb_id; 
  gnbItem.gnb_id = gnb_id;

  string gnb_name;
  if(!itti_msg.ngSetupReq->getRanNodeName(gnb_name)){
    Logger::amf_n2().warn("IE RanNodeName not existed");
  }else{
    gc->gnb_name = gnb_name;
    gnbItem.gnb_name = gnb_name;
    Logger::amf_n2().debug("IE RanNodeName(%s)",gnb_name.c_str());
  }

  int defPagingDrx = itti_msg.ngSetupReq->getDefaultPagingDRX();
  if(defPagingDrx == -1){
    Logger::amf_n2().error("Missing Mandontary IE DefaultPagingDRX");
    return;
  }
  Logger::amf_n2().debug("IE DefaultPagingDRX(%d)",defPagingDrx);

  vector<SupportedItem_t>   s_ta_list;
  if(!itti_msg.ngSetupReq->getSupportedTAList(s_ta_list)){//getSupportedTAList
    return;
  }
  gnbItem.mcc = s_ta_list[0].b_plmn_list[0].mcc; 
  gnbItem.mnc = s_ta_list[0].b_plmn_list[0].mnc;
  gnbItem.tac = s_ta_list[0].tac; 
  //association GlobalRANNodeID with assoc_id
  //store RAN Node Name in gNB context, if present
  //verify PLMN Identity and TAC with configuration and store supportedTAList in gNB context, if verified; else response NG SETUP FAILURE with cause "Unknown PLMN"(9.3.1.2, ts38413)
  if(!verifyPlmn(s_ta_list)){
    //encode NG SETUP FAILURE MESSAGE and send back
    void *buffer = calloc(1,1000);
    NGSetupFailureMsg ngSetupFailure; 
    ngSetupFailure.setMessageType();
    ngSetupFailure.setCauseRadioNetwork(Ngap_CauseRadioNetwork_unspecified,Ngap_TimeToWait_v5s);
    int encoded = ngSetupFailure.encode2buffer((uint8_t*)buffer,1000);
    bstring b = blk2bstr(buffer, encoded);
    sctp_s_38412.sctp_send_msg(itti_msg.assoc_id,itti_msg.stream ,&b);
    Logger::amf_n2().error("no common plmn, encoding NG_SETUP_FAILURE with cause( Unknown PLMN )");
    return;
  }else{
    gc->s_ta_list = s_ta_list;
  }
  //store Paging DRX in gNB context
  Logger::amf_n2().debug("encoding NG_SETUP_RESPONSE ...");
  //encode NG SETUP RESPONSE message with information stored in configuration file and send_msg
  void *buffer = calloc(1,1000); 
  NGSetupResponseMsg ngSetupResp;
  ngSetupResp.setMessageType();
  ngSetupResp.setAMFName(amf_cfg.AMF_Name);
  ngSetupResp.setRelativeAmfCapacity(amf_cfg.relativeAMFCapacity);
  std::vector<struct GuamiItem_s> guami_list;
  for(int i=0; i< amf_cfg.guami_list.size(); i++){
    struct GuamiItem_s tmp;
    tmp.mcc = amf_cfg.guami_list[i].mcc;
    tmp.mnc = amf_cfg.guami_list[i].mnc;
    tmp.regionID = amf_cfg.guami_list[i].regionID;
    tmp.AmfSetID = amf_cfg.guami_list[i].AmfSetID;
    tmp.AmfPointer = amf_cfg.guami_list[i].AmfPointer;
    //tmp.mcc = amf_cfg.guami_list[i].mcc;
    guami_list.push_back(tmp);
  }
  ngSetupResp.setGUAMIList(guami_list);
  std::vector<PlmnSliceSupport_t> plmn_list;
  for(int i=0; i<amf_cfg.plmn_list.size(); i++){
    PlmnSliceSupport_t tmp;
    tmp.mcc = amf_cfg.plmn_list[i].mcc;
    tmp.mnc = amf_cfg.plmn_list[i].mnc;
    for(int j=0; j<amf_cfg.plmn_list[i].slice_list.size(); j++){
      SliceSupportItem_t s_tmp;
      s_tmp.sst = amf_cfg.plmn_list[i].slice_list[j].sST; 
      s_tmp.sd = amf_cfg.plmn_list[i].slice_list[j].sD; 
      tmp.slice_list.push_back(s_tmp);
    }
    plmn_list.push_back(tmp);
  }
  ngSetupResp.setPlmnSupportList(plmn_list);
  int encoded = ngSetupResp.encode2buffer((uint8_t*)buffer,1000);
  bstring b = blk2bstr(buffer, encoded);
  sctp_s_38412.sctp_send_msg(itti_msg.assoc_id,itti_msg.stream ,&b);
  Logger::amf_n2().debug("sending NG_SETUP_RESPONSE ok");
  gc.get()->ng_state = NGAP_READY;
  Logger::amf_n2().debug("gnb with [gnb_id(0x%x), assoc_id(%d)] has been attached to AMF", gc.get()->globalRanNodeId, itti_msg.assoc_id);
  stacs.gNB_connected += 1;
  stacs.gnbs.push_back(gnbItem);
  return;
}


/************************* INITIAL_UE_MESSAGE Handler **************************/
void amf_n2::handle_itti_message(itti_initial_ue_message &init_ue_msg){
  //create ngap-ue context and store in gNB context to store UE information in gNB, for example, here RAN UE NGAP ID and location information and RRC Establishment Cause
  //send NAS-PDU to NAS layer
  /*get INITIAL_UE_MESSAGE IEs*/
  //check the gNB context on which  this UE is attached with assoc_id
  
  itti_nas_signalling_establishment_request * itti_msg = new itti_nas_signalling_establishment_request(TASK_AMF_N2, TASK_AMF_APP);
  
  if(!is_assoc_id_2_gnb_context(init_ue_msg.assoc_id)) {
    Logger::amf_n2().error("no existed gnb context with assoc_id(%d)",init_ue_msg.assoc_id);
    return;
  }
  std::shared_ptr<gnb_context> gc;
  gc = assoc_id_2_gnb_context(init_ue_msg.assoc_id);
  if(gc.get()->ng_state == NGAP_RESETING || gc.get()->ng_state == NGAP_SHUTDOWN){
    Logger::amf_n2().warn("Received new association request on an association that is being %s, ignoring", ng_gnb_state_str[gc.get()->ng_state]);
  }else if(gc.get()->ng_state != NGAP_READY){   
    Logger::amf_n2().debug("gNB with assoc_id(%d) is illegal", init_ue_msg.assoc_id);
    return;
  } 

  uint32_t ran_ue_ngap_id;
  if( (ran_ue_ngap_id = init_ue_msg.initUeMsg->getRanUENgapID()) == -1){
    Logger::amf_n2().error("Missing Mondontary IE(RanUeNgapId)");
    return;
  }
  std::shared_ptr<ue_ngap_context> unc;
  if(!is_ran_ue_id_2_ne_ngap_context(ran_ue_ngap_id)){
    Logger::amf_n2().debug("Create a new ue ngap context with ran_ue_ngap_id(0x%x)", ran_ue_ngap_id);
    unc = std::shared_ptr<ue_ngap_context>(new ue_ngap_context());
    set_ran_ue_ngap_id_2_ue_ngap_context(ran_ue_ngap_id, unc);
  }else{
    unc = ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id);
  }
  if(unc.get() == nullptr){
    Logger::amf_n2().error("Failed to get ue ngap context for ran_ue_ngap_id(0x%x)",21);
  }else{
    //store information into ue ngap context
    unc.get()->ran_ue_ngap_id = ran_ue_ngap_id;
    unc.get()->sctp_stream_recv = init_ue_msg.stream;
    unc.get()->sctp_stream_send == gc.get()->next_sctp_stream;
    gc.get()->next_sctp_stream += 1;
    if(gc.get()->next_sctp_stream >= gc.get()->instreams)
      gc.get()->next_sctp_stream = 1;
    unc.get()->gnb_assoc_id = init_ue_msg.assoc_id;
    NrCgi_t cgi;
    Tai_t   tai;
    if(init_ue_msg.initUeMsg->getUserLocationInfoNR(cgi, tai)){
      itti_msg->cgi = cgi;
      itti_msg->tai = tai;
    }else{
      Logger::amf_n2().error("Missing Mondontary IE UserLocationInfoNR");
      return;
    }
    if(init_ue_msg.initUeMsg->getRRCEstablishmentCause() == -1){
      Logger::amf_n2().warn("IE RRCEstablishmentCause not present");
      itti_msg->rrc_cause = -1;//not present
    }else{
      itti_msg->rrc_cause = init_ue_msg.initUeMsg->getRRCEstablishmentCause();
    }
#if 0
    if(init_ue_msg.initUeMsg->getUeContextRequest() == -1){
      Logger::amf_n2().warn("IE UeContextRequest not present");
      itti_msg->ueCtxReq = -1;//not present
    }else{
      itti_msg->ueCtxReq = init_ue_msg.initUeMsg->getUeContextRequest();
      Logger::amf_n2().debug("testing 12");
    }
#endif
    std::string _5g_s_tmsi;
    if(!init_ue_msg.initUeMsg->get5GS_TMSI(_5g_s_tmsi)){
      itti_msg->is_5g_s_tmsi_present = false;
      Logger::amf_n2().debug("5g_s_tmsi false");
    }else{
      itti_msg->is_5g_s_tmsi_present = true;
      itti_msg->_5g_s_tmsi = _5g_s_tmsi;
      Logger::amf_n2().debug("5g_s_tmsi true");
    }

    uint8_t *nas_buf;
    size_t   nas_len = 0;
    if(init_ue_msg.initUeMsg->getNasPdu(nas_buf,nas_len)){
      bstring nas = blk2bstr(nas_buf, nas_len);
      itti_msg->nas_buf = nas;
    }else{
      Logger::amf_n2().error("Missing IE NAS-PDU");
      return;
    }

  }
  itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = -1;
  std::shared_ptr<itti_nas_signalling_establishment_request> i = std::shared_ptr<itti_nas_signalling_establishment_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n2().error( "Could not send ITTI message %s to task TASK_AMF_APP", i->get_msg_name());
  }
}

void amf_n2::handle_itti_message(itti_ul_nas_transport &ul_nas_transport){
  unsigned long amf_ue_ngap_id = ul_nas_transport.ulNas->getAmfUeNgapId();
  uint32_t      ran_ue_ngap_id = ul_nas_transport.ulNas->getRanUeNgapId();
  std::shared_ptr<gnb_context>gc;
  if(!is_assoc_id_2_gnb_context(ul_nas_transport.assoc_id)){
    Logger::amf_n2().error("gnb with assoc_id(%d) is illegal",ul_nas_transport.assoc_id);
    return;
  }
  gc = assoc_id_2_gnb_context(ul_nas_transport.assoc_id); 
  std::shared_ptr<ue_ngap_context> unc;
  if(!is_ran_ue_id_2_ne_ngap_context(ran_ue_ngap_id)){
    Logger::amf_n2().error("UE with ran_ue_ngap_id(0x%x) is not attached to gnb with assoc_id(%d)", ran_ue_ngap_id, ul_nas_transport.assoc_id);
    return;
  }
  if(!is_ran_ue_id_2_ne_ngap_context(ran_ue_ngap_id)){
    Logger::amf_n2().error("no ue ngap context with ran_ue_ngap_id(%d)", ran_ue_ngap_id);
    return;
  }
  unc = ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id);
  if(unc.get()->amf_ue_ngap_id != amf_ue_ngap_id){
    Logger::amf_n2().error("The requested UE(amf_ue_ngap_id:0x%x) is not valid, existed UE which's amf_ue_ngap_id(0x%x)", amf_ue_ngap_id, unc.get()->amf_ue_ngap_id);
  }
  if(unc.get()->ng_ue_state != NGAP_UE_CONNECTED){
    Logger::amf_n2().error("Received NGAP UPLINK_NAS_TRANSPORT while UE in state != NGAP_UE_CONNECTED");
    //return;
  }
  itti_uplink_nas_data_ind * itti_msg = new itti_uplink_nas_data_ind(TASK_AMF_N2, TASK_AMF_N1);
  itti_msg->is_nas_signalling_estab_req = false;
  itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
  itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
  itti_msg->is_guti_valid = false;
  uint8_t *nas_buf = NULL;
  size_t nas_len = 0;
  if(ul_nas_transport.ulNas->getNasPdu(nas_buf, nas_len)){
    itti_msg->nas_msg = blk2bstr(nas_buf, nas_len);
  }else{
    Logger::amf_n2().error("Missing IE NAS-PDU");
    return;
  }
  std::shared_ptr<itti_uplink_nas_data_ind> i = std::shared_ptr<itti_uplink_nas_data_ind>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n2().error( "Could not send ITTI message %s to task TASK_AMF_N1", i->get_msg_name());
  }
}

void amf_n2::handle_itti_message(itti_dl_nas_transport &dl_nas_transport){
  std::shared_ptr<ue_ngap_context> unc;
  unc = ran_ue_id_2_ue_ngap_context(dl_nas_transport.ran_ue_ngap_id);
  if(unc.get() == nullptr){
    Logger::amf_n2().error("Illegal ue with ran_ue_ngap_id(0x%x)", dl_nas_transport.ran_ue_ngap_id);
    return;
  }
  std::shared_ptr<gnb_context> gc;
  gc = assoc_id_2_gnb_context(unc.get()->gnb_assoc_id);
  if(gc.get() == nullptr){
    Logger::amf_n2().error("Illegal gnb with assoc id(0x%x)", unc.get()->gnb_assoc_id);
    return;
  }
  unc.get()->amf_ue_ngap_id = dl_nas_transport.amf_ue_ngap_id;
  unc.get()->ng_ue_state = NGAP_UE_CONNECTED;
  DownLinkNasTransportMsg *ngap_msg = new DownLinkNasTransportMsg();
  ngap_msg->setMessageType();
  ngap_msg->setAmfUeNgapId(dl_nas_transport.amf_ue_ngap_id);
  ngap_msg->setRanUeNgapId(dl_nas_transport.ran_ue_ngap_id);
  ngap_msg->setNasPdu((uint8_t*)bdata(dl_nas_transport.nas), blength(dl_nas_transport.nas));
  uint8_t buffer[1024];
  int encoded_size = ngap_msg->encode2buffer(buffer, 1024);
  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc.get()->sctp_assoc_id, unc.get()->sctp_stream_send,&b);
}

void amf_n2::handle_itti_message(itti_initial_context_setup_request &itti_msg){
  std::shared_ptr<ue_ngap_context> unc;
  unc = ran_ue_id_2_ue_ngap_context(itti_msg.ran_ue_ngap_id);
  if(unc.get() == nullptr){
    Logger::amf_n2().error("Illegal ue with ran_ue_ngap_id(0x%x)", itti_msg.ran_ue_ngap_id);
    return;
  }
  std::shared_ptr<gnb_context> gc;
  gc = assoc_id_2_gnb_context(unc.get()->gnb_assoc_id);
  if(gc.get() == nullptr){
    Logger::amf_n2().error("Illegal gnb with assoc id(0x%x)", unc.get()->gnb_assoc_id);
    return;
  }
  InitialContextSetupRequestMsg * msg = new InitialContextSetupRequestMsg();
  msg->setMessageType();
  msg->setAmfUeNgapId(itti_msg.amf_ue_ngap_id);
  msg->setRanUeNgapId(itti_msg.ran_ue_ngap_id);
  Guami_t guami;
  guami.mcc = amf_cfg.guami.mcc; 
  guami.mnc = amf_cfg.guami.mnc; 
  guami.regionID = amf_cfg.guami.regionID; 
  guami.AmfSetID = amf_cfg.guami.AmfSetID; 
  guami.AmfPointer = amf_cfg.guami.AmfPointer; 
  msg->setGuami(guami); 
  msg->setUESecurityCapability(0xe000, 0xe000, 0xe000, 0xe000); 
  msg->setSecurityKey((uint8_t*)bdata(itti_msg.kgnb));
  msg->setNasPdu((uint8_t*)bdata(itti_msg.nas), blength(itti_msg.nas));


  if(itti_msg.is_sr){
    bstring ueCapability = gc.get()->ue_radio_cap_ind;
    uint8_t *uecap = (uint8_t*)calloc(1, blength(ueCapability)+1);
    memcpy(uecap, (uint8_t*)bdata(ueCapability), blength(ueCapability));
    uecap[blength(ueCapability)] = '\0';
    msg->setUERadioCapability(uecap, (size_t)blength(ueCapability));
    //msg->setUERadioCapability((uint8_t*)bdata(ueCapability), (size_t)blength(ueCapability));
    Logger::amf_n2().debug("Encoding parameters for service request");
    if (itti_msg.is_pdu_exist) {
      std::vector<PDUSessionResourceSetupRequestItem_t>list;
      PDUSessionResourceSetupRequestItem_t item;
      item.pduSessionId = itti_msg.pdu_session_id;
      item.s_nssai.sst = "01";
      item.s_nssai.sd = "";
      item.pduSessionNAS_PDU = NULL;
      if(itti_msg.isn2sm_avaliable){
        bstring n2sm = itti_msg.n2sm;
        if(blength(itti_msg.n2sm) != 0){
          item.pduSessionResourceSetupRequestTransfer.buf = (uint8_t*)bdata(itti_msg.n2sm);
          item.pduSessionResourceSetupRequestTransfer.size = blength(itti_msg.n2sm);
        }else{
          Logger::amf_n2().error("n2sm empty!");
        }
      }
      list.push_back(item);
      msg->setPduSessionResourceSetupRequestList(list);
      msg->setUEAggregateMaxBitRate(1000000000, 100000000);
    }
  }

  uint8_t buffer[10000];
  int encoded_size = msg->encode2buffer(buffer, 10000);
  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc.get()->sctp_assoc_id, unc.get()->sctp_stream_send,&b);
}

void amf_n2::handle_itti_message(itti_pdu_session_resource_setup_request &itti_msg){
  std::shared_ptr<ue_ngap_context> unc;
  unc = ran_ue_id_2_ue_ngap_context(itti_msg.ran_ue_ngap_id);
  if(unc.get() == nullptr){
    Logger::amf_n2().error("Illegal ue with ran_ue_ngap_id(0x%x)", itti_msg.ran_ue_ngap_id);
    return;
  }
  std::shared_ptr<gnb_context> gc;
  gc = assoc_id_2_gnb_context(unc.get()->gnb_assoc_id);
  if(gc.get() == nullptr){
    Logger::amf_n2().error("Illegal gnb with assoc id(0x%x)", unc.get()->gnb_assoc_id);
    return;
  }
  PduSessionResourceSetupRequestMsg * psrsr = new PduSessionResourceSetupRequestMsg();
  psrsr->setMessageType();
  psrsr->setAmfUeNgapId(itti_msg.amf_ue_ngap_id);
  psrsr->setRanUeNgapId(itti_msg.ran_ue_ngap_id);
 
  std::vector<PDUSessionResourceSetupRequestItem_t> list;
  PDUSessionResourceSetupRequestItem_t item;
  item.pduSessionId = itti_msg.pdu_session_id;
  uint8_t *nas_pdu = (uint8_t*)calloc(1, blength(itti_msg.nas)+1);
  memcpy(nas_pdu, (uint8_t*)bdata(itti_msg.nas), blength(itti_msg.nas));
  nas_pdu[blength(itti_msg.nas)] = '\0';
  item.pduSessionNAS_PDU = nas_pdu;//(uint8_t*)bdata(itti_msg.nas);
  item.sizeofpduSessionNAS_PDU = blength(itti_msg.nas);
  item.s_nssai.sst = "01";
  item.s_nssai.sd = "";
  item.pduSessionResourceSetupRequestTransfer.buf = (uint8_t*)bdata(itti_msg.n2sm);
  item.pduSessionResourceSetupRequestTransfer.size = blength(itti_msg.n2sm);
  list.push_back(item);
  psrsr->setPduSessionResourceSetupRequestList(list);
  
  uint8_t buffer[5000];
  int encoded_size = psrsr->encode2buffer(buffer, 5000);
  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc.get()->sctp_assoc_id, unc.get()->sctp_stream_send,&b);
}

void amf_n2::handle_itti_message(itti_ue_context_release_request &itti_msg){
  Logger::amf_n2().debug("handling ue context release request ...");
  unsigned long amf_ue_ngap_id = itti_msg.ueCtxRel->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id = itti_msg.ueCtxRel->getRanUeNgapId();
  e_Ngap_CauseRadioNetwork cause;
  itti_msg.ueCtxRel->getCauseRadioNetwork(cause);
  UEContextReleaseCommandMsg * ueCtxRelCmd = new UEContextReleaseCommandMsg();
  ueCtxRelCmd->setMessageType();
  ueCtxRelCmd->setUeNgapIdPair(amf_ue_ngap_id, ran_ue_ngap_id);
  ueCtxRelCmd->setCauseRadioNetwork(cause);
  uint8_t buffer[200];
  int encoded_size = ueCtxRelCmd->encode2buffer(buffer, 200);
  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(itti_msg.assoc_id, itti_msg.stream, &b);
}


void amf_n2::handle_itti_message(itti_ue_context_release_command &itti_msg) {
	Logger::amf_n2().debug("handling ue context release command ...");

	std::shared_ptr<ue_ngap_context> unc;
	unc = ran_ue_id_2_ue_ngap_context(itti_msg.ran_ue_ngap_id);
	if (unc.get() == nullptr) {
		Logger::amf_n2().error("Illegal ue with ran_ue_ngap_id(0x%x)", itti_msg.ran_ue_ngap_id);
		return;
	}
	std::shared_ptr<gnb_context> gc;
	gc = assoc_id_2_gnb_context(unc.get()->gnb_assoc_id);
	if (gc.get() == nullptr) {
		Logger::amf_n2().error("Illegal gnb with assoc id(0x%x)", unc.get()->gnb_assoc_id);
		return;
	}

	UEContextReleaseCommandMsg * ueCtxRelCmd = new UEContextReleaseCommandMsg();
	ueCtxRelCmd->setMessageType();
	ueCtxRelCmd->setUeNgapIdPair(itti_msg.amf_ue_ngap_id, itti_msg.ran_ue_ngap_id);
	if (itti_msg.cause.getChoiceOfCause() == Ngap_Cause_PR_nas)
	{
		ueCtxRelCmd->setCauseNas((e_Ngap_CauseNas)itti_msg.cause.getValue());
	}
	if (itti_msg.cause.getChoiceOfCause() == Ngap_Cause_PR_radioNetwork)
	{
		ueCtxRelCmd->setCauseRadioNetwork((e_Ngap_CauseRadioNetwork)itti_msg.cause.getValue());
	}
	uint8_t buffer[200];
	int encoded_size = ueCtxRelCmd->encode2buffer(buffer, 200);
	bstring b = blk2bstr(buffer, encoded_size);
	sctp_s_38412.sctp_send_msg(gc.get()->sctp_assoc_id, unc.get()->sctp_stream_send, &b);
}

void amf_n2::handle_itti_message(itti_pdu_session_resource_release_command &itti_msg) {
	Logger::amf_n2().debug("handling pdu session resource release command ...");

	std::shared_ptr<ue_ngap_context> unc;
	unc = ran_ue_id_2_ue_ngap_context(itti_msg.ran_ue_ngap_id);
	if (unc.get() == nullptr) {
		Logger::amf_n2().error("Illegal ue with ran_ue_ngap_id(0x%x)", itti_msg.ran_ue_ngap_id);
		return;
	}
	std::shared_ptr<gnb_context> gc;
	gc = assoc_id_2_gnb_context(unc.get()->gnb_assoc_id);
	if (gc.get() == nullptr) {
		Logger::amf_n2().error("Illegal gnb with assoc id(0x%x)", unc.get()->gnb_assoc_id);
		return;
	}

	PduSessionResourceReleaseCommand *pdusessionresourcereleasecommand = new PduSessionResourceReleaseCommand();
	Logger::amf_n2().error(" handling pdu session resource release command set messagetype");
	pdusessionresourcereleasecommand->setMessageType();
	pdusessionresourcereleasecommand->setAmfUeNgapId(itti_msg.amf_ue_ngap_id);
	pdusessionresourcereleasecommand->setRanUeNgapId(itti_msg.ran_ue_ngap_id);
	std::vector<PDUSessionResourceReleaseCommandItem_t> list;
	PDUSessionResourceReleaseCommandItem_t item;
	PDUSessionResourceReleaseCommandTransfer *transfer = new PDUSessionResourceReleaseCommandTransfer();
	uint8_t buffertrans[1000];
	transfer->setCauseRadioNetwork(Ngap_CauseRadioNetwork_multiple_PDU_session_ID_instances);
	item.pduSessionResourceReleaseCommandTransfer.size = transfer->encode2buffer(buffertrans, 1000);
	item.pduSessionResourceReleaseCommandTransfer.buf = buffertrans;
	item.pduSessionId = 5;
	list.push_back(item);
	pdusessionresourcereleasecommand->setPduSessionResourceToReleaseList(list);
	Logger::amf_n2().error(" handling pdu session resource release command set amf/ran ngap id%d %d ",itti_msg.amf_ue_ngap_id,itti_msg.ran_ue_ngap_id);
	uint8_t buffer[1000];
	int encoded_size = pdusessionresourcereleasecommand->encode2buffer(buffer, 200);
	Logger::amf_n2().error(" handling pdu session resource release command encoder");
	bstring b = blk2bstr(buffer, encoded_size);
	sctp_s_38412.sctp_send_msg(gc.get()->sctp_assoc_id, unc.get()->sctp_stream_send, &b);
}

void amf_n2::handle_itti_message(itti_ue_radio_capability_indication &itti_msg){
  std::shared_ptr<gnb_context> gc;
  if(!is_assoc_id_2_gnb_context(itti_msg.assoc_id)) {
    Logger::amf_n2().error("no existed gnb context with assoc_id(%d)",itti_msg.assoc_id);
    return;
  }
  gc = assoc_id_2_gnb_context(itti_msg.assoc_id);
  unsigned long amf_ue_ngap_id; itti_msg.ueRadioCap->getAmfUeNgapId(amf_ue_ngap_id);
  uint32_t ran_ue_ngap_id; itti_msg.ueRadioCap->getRanUeNgapId(ran_ue_ngap_id);
  uint8_t *ue_radio_cap; size_t size;
  if(!itti_msg.ueRadioCap->getUERadioCapability(ue_radio_cap, size)){
    Logger::amf_n2().warn("No IE UERadioCapability");
  }
  gc.get()->ue_radio_cap_ind = blk2bstr(ue_radio_cap, (int)size);
   
}

/************************************************* context management functions *********************************/

bool amf_n2::is_ran_ue_id_2_ne_ngap_context(const uint32_t & ran_ue_ngap_id) const {
  std::shared_lock lock(m_ranid2uecontext);
  return bool{ranid2uecontext.count(ran_ue_ngap_id) > 0};
}

std::shared_ptr<ue_ngap_context> amf_n2::ran_ue_id_2_ue_ngap_context(const uint32_t & ran_ue_ngap_id) const {
  std::shared_lock lock(m_ranid2uecontext);
  return ranid2uecontext.at(ran_ue_ngap_id); 
}

void amf_n2::set_ran_ue_ngap_id_2_ue_ngap_context(const uint32_t & ran_ue_ngap_id, std::shared_ptr<ue_ngap_context> unc){
  std::shared_lock lock(m_ranid2uecontext);
  ranid2uecontext[ran_ue_ngap_id] = unc;
}

/************************************************* internal analysis functions ***********************************/
bool amf_n2::verifyPlmn(vector<SupportedItem_t> list){
  for(int i=0; i<amf_cfg.plmn_list.size(); i++){
    for(int j=0; j<list.size(); j++){
      Logger::amf_n2().debug("tac configured(%d) -- tac received(%d)",amf_cfg.plmn_list[i].tac, list[j].tac);
      if(amf_cfg.plmn_list[i].tac != list[j].tac){ continue;}
      for(int k=0; k< list[j].b_plmn_list.size();k++){
        if(!(list[j].b_plmn_list[k].mcc.compare(amf_cfg.plmn_list[i].mcc)) && !(list[j].b_plmn_list[k].mnc.compare(amf_cfg.plmn_list[i].mnc))){
          return true;
        }
      }
    }
  }
  return false;
}
