#ifndef _AMF_N2_H_
#define _AMF_N2_H_

#include "ngap_app.hpp"
#include "itti_msg_n2.hpp"
#include "ue_ngap_context.hpp"
#include "PduSessionResourceReleaseCommand.hpp"

namespace amf_application{

class amf_n2 : public ngap::ngap_app{
public:
  amf_n2(const string &address, const uint16_t port_num);
  ~amf_n2();
  //void handle_receive(bstring payload, sctp_assoc_id_t assoc_id, sctp_stream_id_t stream, sctp_stream_id_t instreams, sctp_stream_id_t outstreams);
  void handle_itti_message(itti_new_sctp_association &new_assoc);
  void handle_itti_message(itti_ng_setup_request &ngsetupreq);
  void handle_itti_message(itti_initial_ue_message &init_ue_msg);
  void handle_itti_message(itti_ul_nas_transport &ul_nas_transport);
  void handle_itti_message(itti_dl_nas_transport &dl_nas_transport);
  void handle_itti_message(itti_initial_context_setup_request &itti_msg);
  void handle_itti_message(itti_pdu_session_resource_setup_request &itti_msg);
  void handle_itti_message(itti_ue_context_release_request &itti_msg);
  void handle_itti_message(itti_ue_radio_capability_indication &itti_msg);
  void handle_itti_message(itti_ue_context_release_command &itti_msg);
  void handle_itti_message(itti_pdu_session_resource_release_command &itti_msg);
  bool verifyPlmn(vector<SupportedItem_t> list);
private:
  std::map<uint32_t, std::shared_ptr<ue_ngap_context>> ranid2uecontext;// ran ue ngap id
  mutable std::shared_mutex m_ranid2uecontext;

  bool is_ran_ue_id_2_ne_ngap_context(const uint32_t & ran_ue_ngap_id) const;
  std::shared_ptr<ue_ngap_context> ran_ue_id_2_ue_ngap_context(const uint32_t & ran_ue_ngap_id) const;
  void set_ran_ue_ngap_id_2_ue_ngap_context(const uint32_t & ran_ue_ngap_id, std::shared_ptr<ue_ngap_context> unc);
}; 








}
















#endif
