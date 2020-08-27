#ifndef _AMF_N11_H_
#define _AMF_N11_H_
#include "itti_msg_n11.hpp"
#include <map>
#include <shared_mutex>
#include "pdu_session_context.hpp"
#include <string>
using namespace std;

namespace amf_application{

class amf_n11{
public:
  amf_n11();
  ~amf_n11();
public:
  void handle_itti_message(itti_smf_services_consumer &);
  void handle_pdu_session_initial_request(string supi, std::shared_ptr<pdu_session_context> psc, string smf_addr, bstring sm_msg, string dnn);
  void handle_itti_message(itti_pdu_session_resource_setup_response &itti_msg);
  void handle_itti_message(itti_nsmf_pdusession_update_sm_context &itti_msg);
  void handle_itti_message(itti_nsmf_pdusession_release_sm_context &itti_msg);
public:
  std::map<string, std::shared_ptr<pdu_session_context>> supi2pdu; // amf ue ngap id
  mutable std::shared_mutex m_supi2pdu;
  bool is_supi_to_pdu_ctx(const string &supi) const;
  std::shared_ptr<pdu_session_context> supi_to_pdu_ctx(const string & supi) const;
  void set_supi_to_pdu_ctx(const string &supi, std::shared_ptr<pdu_session_context> psc);
public:
  std::map<uint8_t, string> pduid2supi;

public:
  bool smf_selection_from_configuration(string & smf_addr);
  bool smf_selection_from_context(string & smf_addr);
public:
  void handle_post_sm_context_response_error_400();
  void handle_post_sm_context_response_error(long code, string cause, bstring n1sm, string supi, uint8_t pdu_session_id);
public:
  void curl_http_client(string remoteUri, string jsonData, string n1SmMsg, string n2SmMsg, string supi, uint8_t pdu_session_id);
};





}





























#endif
