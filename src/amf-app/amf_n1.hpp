#ifndef _AMF_N1_H_
#define _AMF_N1_H_

#include <map>
#include <shared_mutex>
#include "nas_context.hpp"
#include "pdu_session_context.hpp"
#include "itti_msg_n1.hpp"
#include "bstrlib.h"
#include "3gpp_ts24501.hpp"
#include "amf_statistics.hpp"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <inttypes.h>
#include <mysql/mysql.h>

#include "mysql_db.hpp"

namespace amf_application{

#define NAS_MESSAGE_DOWNLINK 1
#define NAS_MESSAGE_UPLINK   0

typedef enum{
  PlainNasMsg = 0x0,
  IntegrityProtected = 0x1,
  IntegrityProtectedAndCiphered = 0x2,
  IntegrityProtectedWithNew5GNASSecurityContext = 0x3,
  IntegrityProtectedAndCipheredWithNew5GNASSecurityContext = 0x4,
}SecurityHeaderType;

class amf_n1{
public:
  amf_n1();
  ~amf_n1();
  void handle_itti_message(itti_uplink_nas_data_ind&);
  void handle_itti_message(itti_downlink_nas_transfer & itti_msg);
public: // nas message decode
  void nas_signalling_establishment_request_handle(SecurityHeaderType type, std::shared_ptr<nas_context> nc, uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring plain_msg, std::string snn, uint8_t ulCount);
  void uplink_nas_msg_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring plain_msg);
  bool check_security_header_type(SecurityHeaderType & type, uint8_t *buffer);

public:
  std::map<long, std::shared_ptr<nas_context>> amfueid2nas_context; // amf ue ngap id
  std::map<string, std::shared_ptr<nas_context>> imsi2nas_context;
  std::map<std::string, long> supi2amfId;
  std::map<std::string, uint32_t> supi2ranId;

  std::map<std::string, std::shared_ptr<nas_context>> guti2nas_context;
  mutable std::shared_mutex m_guti2nas_context;
  bool is_guti_2_nas_context(const std::string & guti) const;
  std::shared_ptr<nas_context> guti_2_nas_context(const std::string & guti) const;
  void set_guti_2_nas_context(const std::string & guti, std::shared_ptr<nas_context>nc);

  mutable std::shared_mutex m_amfueid2nas_context;
  bool is_amf_ue_id_2_nas_context(const long & amf_ue_ngap_id) const;
  std::shared_ptr<nas_context> amf_ue_id_2_nas_context(const long & amf_ue_ngap_id) const;
  void set_amf_ue_ngap_id_2_nas_context(const long & amf_ue_ngap_id, std::shared_ptr<nas_context> nc);
  database_t *db_desc;

private://nas message handlers
  void ue_initiate_de_registration_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring nas);
  void registration_request_handle(bool isNasSig, std::shared_ptr<nas_context>nc, uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, std::string snn, bstring  reg);
  void authentication_response_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring plain_msg);
  void authentication_failure_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring plain_msg);
  void security_mode_complete_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring nas_msg);
  void security_mode_reject_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring nas_msg);
  void ul_nas_transport_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring nas);
  void sha256(unsigned char * message, int msg_len, unsigned char * output);
  void service_request_handle(bool isNasSig, std::shared_ptr<nas_context> nc, uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring nas);
  void identity_response_handle(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, bstring plain_msg);
private://authentication vector
  bool generate_authentication_vector();

private:
  void itti_send_dl_nas_buffer_to_task_n2(bstring & b, uint32_t ran_ue_ngap_id, long amf_ue_ngap_id);
private://response message
  void response_registration_reject_msg(uint8_t cause_value, uint32_t ran_ue_ngap_id, long amf_ue_ngap_id);
public://procedures
  void run_registration_procedure(std::shared_ptr<nas_context>&nc);
  void run_initial_registration_procedure();
  void run_mobility_registration_update_procedure(std::shared_ptr<nas_context>nc);
public://authentication
  bool auth_vectors_generator(std::shared_ptr<nas_context>&nc);
  bool authentication_vectors_generator_in_ausf(std::shared_ptr<nas_context>&nc);
  bool authentication_vectors_generator_in_udm(std::shared_ptr<nas_context>&nc);
public://mysql handlers in mysql_db.cpp
  bool get_mysql_auth_info(std::string imsi, mysql_auth_info_t &resp);
  void mysql_push_rand_sqn(std::string imsi, uint8_t *rand_p, uint8_t *sqn);
  void mysql_increment_sqn(std::string imsi);
  bool connect_to_mysql();
  void generate_random(uint8_t *random_p, ssize_t length);
  void generate_5g_he_av_in_udm(const uint8_t opc[16], std::string imsi, uint8_t key[16], uint8_t sqn[6], std::string serving_network, _5G_HE_AV_t &vector);
  void test_generate_5g_he_av_in_udm(const uint8_t opc[16],  uint8_t key[16], uint8_t sqnak[6], std::string serving_network, _5G_HE_AV_t &vector);
  void handle_auth_vector_successful_result(std::shared_ptr<nas_context> nc);
  bool start_authentication_procedure(std::shared_ptr<nas_context> nc, int vindex, uint8_t ngksi);
  bool check_nas_common_procedure_on_going(std::shared_ptr<nas_context> nc);
  int security_select_algorithms(uint8_t nea, uint8_t nia, uint8_t &amf_nea, uint8_t &amf_nia);
  bool start_security_mode_control_procedure(std::shared_ptr<nas_context>nc);
  void encode_nas_message_protected(nas_secu_ctx * nsc, bool is_secu_ctx_new, uint8_t security_header_type, uint8_t direction, uint8_t *input_nas_buf, int input_nas_len, bstring & encrypted_nas);
  bool nas_message_integrity_protected(nas_secu_ctx *nsc, uint8_t direction, uint8_t *input_nas, int input_nas_len, uint32_t &mac);
  bool nas_message_cipher_protected(nas_secu_ctx *nsc, uint8_t direction, bstring input_nas, bstring &output_nas);
public:
  void dump_nas_message(uint8_t *buf, int len);
public:
  void ue_authentication_simulator(uint8_t *rand, uint8_t *autn);
  void annex_a_4_33501(uint8_t ck[16], uint8_t ik[16], uint8_t *input, uint8_t rand[16], std::string serving_network, uint8_t *output);
public:
  void send_itti_to_smf_services_consumer(uint32_t ran_ue_ngap_id, long amf_ue_ngap_id, uint8_t request_type, uint8_t pdu_session_id, bstring dnn, bstring sm_msg);
public:
  void update_ue_information_statics(ue_infos &ueItem, const string connStatus, const string registerStatus, uint32_t ranid, uint32_t amfid, string imsi, string guti, string mcc, string mnc, uint32_t cellId);
};
}










#endif
