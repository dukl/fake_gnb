#ifndef _AMF_CONFIG_H_
#define _AMF_CONFIG_H_

#include <arpa/inet.h>
#include <libconfig.h++>
#include <netinet/in.h>
#include <sys/socket.h>

#include <mutex>
#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "amf_config.hpp"
#include "thread_sched.hpp"

#define AMF_CONFIG_STRING_AMF_CONFIG                    "AMF"
#define AMF_CONFIG_STRING_PID_DIRECTORY                 "PID_DIRECTORY"
#define AMF_CONFIG_STRING_INSTANCE_ID                   "INSTANCE_ID"
#define AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL     "STATISTICS_TIMER_INTERVAL"
#define AMF_CONFIG_STRING_INTERFACES                    "INTERFACES"
#define AMF_CONFIG_STRING_INTERFACE_NGAP_AMF            "NGAP_AMF"
#define AMF_CONFIG_STRING_INTERFACE_NAME                "INTERFACE_NAME"
#define AMF_CONFIG_STRING_IPV4_ADDRESS                  "IPV4_ADDRESS"
#define AMF_CONFIG_STRING_SCTP_PORT                     "SCTP_PORT"
#define AMF_CONFIG_STRING_PPID                          "PPID"

#define AMF_CONFIG_STRING_INTERFACE_N11                 "N11"
#define AMF_CONFIG_STRING_SMF_INSTANCES_POOL            "SMF_INSTANCES_POOL"
#define AMF_CONFIG_STRING_SMF_INSTANCE_ID               "SMF_INSTANCE_ID"
#define AMF_CONFIG_STRING_SMF_INSTANCE_PORT             "PORT"
#define AMF_CONFIG_STRING_SMF_INSTANCE_VERSION          "VERSION"
#define AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED         "SELECTED"

#define AMF_CONFIG_STRING_SCHED_PARAMS                  "SCHED_PARAMS"
#define AMF_CONFIG_STRING_THREAD_RD_CPU_ID              "CPU_ID"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_POLICY        "SCHED_POLICY"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY      "SCHED_PRIORITY"

#define AMF_CONFIG_STRING_AMF_NAME                      "AMF_NAME"
#define AMF_CONFIG_STRING_GUAMI                         "GUAMI"
#define AMF_CONFIG_STRING_ServedGUAMIList               "ServedGUAMIList"
#define AMF_CONFIG_STRING_TAC                           "TAC"
#define AMF_CONFIG_STRING_MCC                           "MCC"
#define AMF_CONFIG_STRING_MNC                           "MNC"
#define AMF_CONFIG_STRING_RegionID                      "RegionID"
#define AMF_CONFIG_STRING_AMFSetID                      "AMFSetID"
#define AMF_CONFIG_STRING_AMFPointer                    "AMFPointer"
#define AMF_CONFIG_STRING_RelativeAMFCapacity           "RelativeAMFCapacity"
#define AMF_CONFIG_STRING_PLMNSupportList               "PLMNSupportList"
#define AMF_CONFIG_STRING_SliceSupportList              "SliceSupportList"
#define AMF_CONFIG_STRING_SST                           "SST"
#define AMF_CONFIG_STRING_SD                            "SD"
#define AMF_CONFIG_STRING_CORE_CONFIGURATION            "CORE_CONFIGURATION"
#define AMF_CONFIG_STRING_EMERGENCY_SUPPORT             "EMERGENCY_SUPPORT"
#define AMF_CONFIG_STRING_AUTHENTICATION                "AUTHENTICATION"
#define AMF_CONFIG_STRING_AUTH_MYSQL_SERVER             "MYSQL_server"
#define AMF_CONFIG_STRING_AUTH_MYSQL_USER               "MYSQL_user"
#define AMF_CONFIG_STRING_AUTH_MYSQL_PASS               "MYSQL_pass"
#define AMF_CONFIG_STRING_AUTH_MYSQL_DB                 "MYSQL_db"
#define AMF_CONFIG_STRING_AUTH_OPERATOR_KEY             "OPERATOR_key"
#define AMF_CONFIG_STRING_AUTH_RANDOM                   "RANDOM"
#define AMF_CONFIG_STRING_NAS                           "NAS"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_INTEGRITY_ALGORITHM_LIST  "ORDERED_SUPPORTED_INTEGRITY_ALGORITHM_LIST"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST  "ORDERED_SUPPORTED_CIPHERING_ALGORITHM_LIST"


using namespace libconfig;
using namespace std;

namespace config{

typedef struct{
  string mysql_server;
  string mysql_user;
  string mysql_pass;
  string mysql_db;
  string operator_key;
  string random;
}auth_conf;

typedef struct interface_cfg_s {
  std::string     if_name;
  struct in_addr  addr4;
  struct in_addr  network4;
  struct in6_addr addr6;
  unsigned int    mtu;
  unsigned int    port;
  util::thread_sched_params thread_rd_sched_params;
} interface_cfg_t;

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params sx_sched_params;
  util::thread_sched_params s5s8_sched_params;
  util::thread_sched_params pgw_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

typedef struct guami_s{
  string mcc;
  string mnc;
  string regionID;
  string AmfSetID;
  string AmfPointer;
}guami_t;

typedef struct slice_s{
  string sST;
  string sD;
}slice_t;

typedef struct plmn_support_item_s{
  string mcc;
  string mnc;
  uint32_t tac;
  vector<slice_t> slice_list;
}plmn_item_t;

typedef struct{
  uint8_t  prefered_integrity_algorithm[8];
  uint8_t  prefered_ciphering_algorithm[8];
}nas_conf_t;

typedef struct{
  int id;
  string ipv4;
  string port;
  string version;
  bool selected;
}smf_inst_t;

class amf_config{
public:
  amf_config();
  ~amf_config();
  int load(const std::string &config_file);
  int load_interface(const Setting& if_cfg, interface_cfg_t & cfg);
  int load_thread_sched_params(const libconfig::Setting& thread_sched_params_cfg, util::thread_sched_params& cfg);
  void display();
public:
  unsigned int                  instance;
  string                        pid_dir;
  interface_cfg_t               n2;
  itti_cfg_t                    itti;
  unsigned int                  statistics_interval;
  string                        AMF_Name;
  guami_t                       guami;
  vector<guami_t>               guami_list;  
  unsigned int                  relativeAMFCapacity;
  vector<plmn_item_t>           plmn_list;
  string                        is_emergency_support;
  auth_conf                     auth_para;
  nas_conf_t                    nas_cfg;
  vector<smf_inst_t>            smf_pool;
};






}







#endif
