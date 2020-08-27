#ifndef _AMF_APP_H_
#define _AMF_APP_H_

#include <map>
#include <set>
#include <shared_mutex>
#include <string>
#include <thread>
#include <sstream>
#include "amf_config.hpp"
#include "amf_module_from_config.hpp"
#include "itti_msg_amf_app.hpp"
#include "ue_context.hpp"

using namespace config;
using namespace std;

static uint32_t amf_app_ue_ngap_id_generator = 1;

namespace amf_application{

#define TASK_AMF_APP_PERIODIC_STATISTICS (0)

class amf_app{
public:
  explicit amf_app(const amf_config &amf_cfg);
  amf_app(amf_app const&) = delete;
  void operator=(amf_app const&)     = delete;
  void allRegistredModulesInit(const amf_modules & modules);
  long generate_amf_ue_ngap_id();
public://itti handlers
  void handle_itti_message(itti_nas_signalling_establishment_request & itti_msg);
  void handle_itti_message(itti_n1n2_message_transfer_request & itti_msg);
public://context management
  std::map<long, std::shared_ptr<ue_context>> amf_ue_ngap_id2ue_ctx;
  mutable std::shared_mutex m_amf_ue_ngap_id2ue_ctx;
  std::map<std::string, std::shared_ptr<ue_context>> ue_ctx_key;
  mutable std::shared_mutex m_ue_ctx_key;

  bool is_amf_ue_id_2_ue_context(const long & amf_ue_ngap_id) const;
  std::shared_ptr<ue_context> amf_ue_id_2_ue_context(const long & amf_ue_ngap_id) const; 
  void set_amf_ue_ngap_id_2_ue_context(const long & amf_ue_ngap_id, std::shared_ptr<ue_context> uc);
  
  bool is_ran_amf_id_2_ue_context(const string & ue_context_key) const;
  std::shared_ptr<ue_context> ran_amf_id_2_ue_context(const string & ue_context_key) const;
  void set_ran_amf_id_2_ue_context(const string & ue_context_key, std::shared_ptr<ue_context> uc);
public:/*** SMF Client response handlers ****/
  void handle_post_sm_context_response_error_400();
public:
  bool generate_5g_guti(uint32_t ranid, long amfid, string &mcc, string &mnc, uint32_t& tmsi);
};



}













#endif
