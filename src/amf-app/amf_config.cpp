#include "amf_config.hpp"
#include "logger.hpp"
#include <libconfig.h++>
#include "string.hpp"
#include "thread_sched.hpp"
#include "amf_app.hpp"
#include "if.hpp"
#include "3gpp_ts24501.hpp"
extern "C"{
  #include <arpa/inet.h>
  #include <stdbool.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include "common_defs.h"
}

#include <iostream>

using namespace libconfig;
using namespace std;
using namespace amf_application;

namespace config{

  amf_config::amf_config(){
  }
  amf_config::~amf_config(){}
  int amf_config::load(const std::string &config_file){
    cout<<endl;
    Logger::amf_app().debug("Load amf system configuration file(%s)",config_file.c_str());
    Config                 cfg;
    unsigned char          buf_in6_addr[sizeof (struct in6_addr)];
    try{
      cfg.readFile(config_file.c_str());
    }catch(const FileIOException &fioex){
      Logger::amf_app().error("I/O error while reading file %s - %s", config_file.c_str(), fioex.what());
      throw;
    }catch(const ParseException &pex){
      Logger::amf_app().error("Parse error at %s:%d - %s", pex.getFile(), pex.getLine(), pex.getError());
      throw;
    }
    const Setting &root = cfg.getRoot();
    try{
      const Setting& amf_cfg = root[AMF_CONFIG_STRING_AMF_CONFIG];
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s", nfex.what(), nfex.getPath());
      return -1;
    }
    const Setting &amf_cfg = root[AMF_CONFIG_STRING_AMF_CONFIG];
    try{
      amf_cfg.lookupValue(AMF_CONFIG_STRING_INSTANCE_ID, instance);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      amf_cfg.lookupValue(AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL, statistics_interval);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      amf_cfg.lookupValue(AMF_CONFIG_STRING_PID_DIRECTORY, pid_dir);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      amf_cfg.lookupValue(AMF_CONFIG_STRING_AMF_NAME, AMF_Name);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      const Setting &guami_cfg = amf_cfg[AMF_CONFIG_STRING_GUAMI];
      guami_cfg.lookupValue(AMF_CONFIG_STRING_MCC, guami.mcc);
      guami_cfg.lookupValue(AMF_CONFIG_STRING_MNC, guami.mnc);
      guami_cfg.lookupValue(AMF_CONFIG_STRING_RegionID, guami.regionID);
      guami_cfg.lookupValue(AMF_CONFIG_STRING_AMFSetID, guami.AmfSetID);
      guami_cfg.lookupValue(AMF_CONFIG_STRING_AMFPointer, guami.AmfPointer);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      const Setting &guami_list_cfg = amf_cfg[AMF_CONFIG_STRING_ServedGUAMIList];
      int count = guami_list_cfg.getLength();
      for(int i=0;i<count;i++){
        guami_t guami;
        const Setting &guami_item = guami_list_cfg[i];
        guami_item.lookupValue(AMF_CONFIG_STRING_MCC,guami.mcc);
        guami_item.lookupValue(AMF_CONFIG_STRING_MNC,guami.mnc);
        guami_item.lookupValue(AMF_CONFIG_STRING_RegionID,guami.regionID);
        guami_item.lookupValue(AMF_CONFIG_STRING_AMFSetID,guami.AmfSetID);
        guami_item.lookupValue(AMF_CONFIG_STRING_AMFPointer,guami.AmfPointer);
        guami_list.push_back(guami);
      }
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      amf_cfg.lookupValue(AMF_CONFIG_STRING_RelativeAMFCapacity, relativeAMFCapacity);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      const Setting &plmn_list_cfg = amf_cfg[AMF_CONFIG_STRING_PLMNSupportList];
      int count = plmn_list_cfg.getLength();
      for(int i=0;i<count;i++){
        plmn_item_t plmn_item;
        const Setting & item = plmn_list_cfg[i];
        item.lookupValue(AMF_CONFIG_STRING_MCC, plmn_item.mcc);
        item.lookupValue(AMF_CONFIG_STRING_MNC, plmn_item.mnc);
        item.lookupValue(AMF_CONFIG_STRING_TAC, plmn_item.tac);
        const Setting &slice_list_cfg = plmn_list_cfg[i][AMF_CONFIG_STRING_SliceSupportList];
        int numOfSlice = slice_list_cfg.getLength();
        for(int j=0;j<numOfSlice;j++){
          slice_t slice;
          const Setting & slice_item = slice_list_cfg[j];
          slice_item.lookupValue(AMF_CONFIG_STRING_SST, slice.sST);
          slice_item.lookupValue(AMF_CONFIG_STRING_SD, slice.sD);
          plmn_item.slice_list.push_back(slice);
        }
        plmn_list.push_back(plmn_item);
      }
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
    try{
      const Setting &new_if_cfg = amf_cfg[AMF_CONFIG_STRING_INTERFACES];
      const Setting &n2_amf_cfg = new_if_cfg[AMF_CONFIG_STRING_INTERFACE_NGAP_AMF];
      load_interface(n2_amf_cfg, n2);
      const Setting &n11_cfg = new_if_cfg[AMF_CONFIG_STRING_INTERFACE_N11];
      const Setting &smf_addr_pool = n11_cfg[AMF_CONFIG_STRING_SMF_INSTANCES_POOL];
      int count = smf_addr_pool.getLength();
      for(int i=0; i< count; i++){
        const Setting & smf_addr_item = smf_addr_pool[i];
        smf_inst_t smf_inst; string selected;
        smf_addr_item.lookupValue(AMF_CONFIG_STRING_SMF_INSTANCE_ID, smf_inst.id);
        smf_addr_item.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, smf_inst.ipv4);
        smf_addr_item.lookupValue(AMF_CONFIG_STRING_SMF_INSTANCE_PORT, smf_inst.port);
        smf_addr_item.lookupValue(AMF_CONFIG_STRING_SMF_INSTANCE_VERSION, smf_inst.version);
        smf_addr_item.lookupValue(AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED, selected);
        if(!selected.compare("true"))
          smf_inst.selected = true;
        else
          smf_inst.selected = false;
        smf_pool.push_back(smf_inst); 
      }
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
      return -1;
    }
    try{
      const Setting &core_config = amf_cfg[AMF_CONFIG_STRING_CORE_CONFIGURATION];
      core_config.lookupValue(AMF_CONFIG_STRING_EMERGENCY_SUPPORT, is_emergency_support);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
      return -1;
    }
    try{
      const Setting &auth = amf_cfg[AMF_CONFIG_STRING_AUTHENTICATION];
      auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_SERVER, auth_para.mysql_server);
      auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_USER, auth_para.mysql_user);
      auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_PASS, auth_para.mysql_pass);
      auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_DB, auth_para.mysql_db);
      auth.lookupValue(AMF_CONFIG_STRING_AUTH_OPERATOR_KEY, auth_para.operator_key);
      auth.lookupValue(AMF_CONFIG_STRING_AUTH_RANDOM, auth_para.random);
    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
      return -1;
    }
    try{
      const Setting &nas = amf_cfg[AMF_CONFIG_STRING_NAS];
      const Setting &intAlg = nas[AMF_CONFIG_STRING_NAS_SUPPORTED_INTEGRITY_ALGORITHM_LIST];

      int intCount = intAlg.getLength();
      for(int i=0; i<intCount; i++){
      string intAlgStr = intAlg[i];
        if(!intAlgStr.compare("NIA0"))
          nas_cfg.prefered_integrity_algorithm[i] = IA0_5G;
        if(!intAlgStr.compare("NIA1"))
          nas_cfg.prefered_integrity_algorithm[i] = IA1_128_5G;
        if(!intAlgStr.compare("NIA2"))
          nas_cfg.prefered_integrity_algorithm[i] = IA2_128_5G;
      }
      for(int i=intCount; i<8; i++){
        nas_cfg.prefered_integrity_algorithm[i] = IA0_5G;
      }
      const Setting &encAlg = nas[AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST];
      int encCount = encAlg.getLength();
      for(int i=0; i<encCount; i++){
      string encAlgStr = encAlg[i];
        if(!encAlgStr.compare("NEA0"))
          nas_cfg.prefered_ciphering_algorithm[i] = EA0_5G;
        if(!encAlgStr.compare("NEA1"))
          nas_cfg.prefered_ciphering_algorithm[i] = EA1_128_5G;
        if(!encAlgStr.compare("NEA2"))
          nas_cfg.prefered_ciphering_algorithm[i] = EA2_128_5G;
      }
      for(int i=encCount; i<8; i++){
        nas_cfg.prefered_ciphering_algorithm[i] = EA0_5G;
      }

    }catch(const SettingNotFoundException &nfex){
      Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
      return -1;
    }
  }

  void amf_config::display(){
    Logger::config().info( "=======    BUPTv1.0   =======");
    Logger::config().info( "Configuration AMF:");
    Logger::config().info( "- Instance .......................: %d", instance);
    Logger::config().info( "- PID dir ........................: %s", pid_dir.c_str());
    Logger::config().info( "- AMF NAME........................: %s", AMF_Name.c_str());
    Logger::config().info( "- GUAMI...........................: ");
    Logger::config().info( "   [%s] [%s] [%s] [%s] [%s]", guami.mcc.c_str(),guami.mnc.c_str(),guami.regionID.c_str(),guami.AmfSetID.c_str(),guami.AmfPointer.c_str());
    Logger::config().info( "- ServedGUAMIList ................: ");
    for(int i=0;i<guami_list.size();i++){
      Logger::config().info( "   [%s] [%s] [%s] [%s] [%s]", guami_list[i].mcc.c_str(),guami_list[i].mnc.c_str(),guami_list[i].regionID.c_str(),guami_list[i].AmfSetID.c_str(),guami_list[i].AmfPointer.c_str());
    }
    Logger::config().info( "- RelativeAMFCapacity ............: %d", relativeAMFCapacity);
    Logger::config().info( "- PLMNSupportList ................: ");
    for(int i=0;i<plmn_list.size();i++){
      Logger::config().info( "   [%s] [%s] ", plmn_list[i].mcc.c_str(),plmn_list[i].mnc.c_str());
      Logger::config().info( "   tac[%d]", plmn_list[i].tac);
      Logger::config().info( "   - SliceSupportList ............: ");
      for(int j=0;j<plmn_list[i].slice_list.size();j++){
        Logger::config().info( "     [%s] [%s] ", plmn_list[i].slice_list[j].sST.c_str(),plmn_list[i].slice_list[j].sD.c_str());
      }
    }
    Logger::config().info( "- Emergency Support ...............: %s", is_emergency_support.c_str());
    Logger::config().info( "- MYSQL server ....................: %s", auth_para.mysql_server.c_str());
    Logger::config().info( "- MYSQL user ......................: %s", auth_para.mysql_user.c_str());
    Logger::config().info( "- MYSQL pass ......................: %s", auth_para.mysql_pass.c_str());
    Logger::config().info( "- MYSQL db ........................: %s", auth_para.mysql_db.c_str());
    Logger::config().info( "- operator key ....................: %s", auth_para.operator_key.c_str());
    Logger::config().info( "- random ..........................: %s", auth_para.random.c_str());
    Logger::config().info( "- Remote SMF Pool..................: ");
    for(int i=0; i<smf_pool.size(); i++){
      string selected;
      if(smf_pool[i].selected) selected = "true";
      else selected = "false";
      Logger::config().info( "    SMF_INSTANCE_ID(%d) : (%s:%s) version(%s) is selected(%s)", smf_pool[i].id, smf_pool[i].ipv4.c_str(), smf_pool[i].port.c_str(), smf_pool[i].version.c_str(), selected.c_str());
    }
  }

  int amf_config::load_interface(const libconfig::Setting& if_cfg, interface_cfg_t& cfg){
    if_cfg.lookupValue(AMF_CONFIG_STRING_INTERFACE_NAME, cfg.if_name);
    util::trim(cfg.if_name);
    if (not boost::iequals(cfg.if_name, "none")) {
      std::string address = {};
      if_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
      util::trim(address);
      if (boost::iequals(address, "read")) {
        if (get_inet_addr_infos_from_iface(cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
          Logger::amf_app().error("Could not read %s network interface configuration", cfg.if_name);
          return RETURNerror;
        }
      } else {
        std::vector<std::string> words;
        boost::split(words, address, boost::is_any_of("/"), boost::token_compress_on);
        if (words.size() != 2) {
          Logger::amf_app().error("Bad value " AMF_CONFIG_STRING_IPV4_ADDRESS " = %s in config file", address.c_str());
          return RETURNerror;
        }
        unsigned char buf_in_addr[sizeof(struct in6_addr)]; // you never know...
        if (inet_pton (AF_INET, util::trim(words.at(0)).c_str(), buf_in_addr) == 1) {
          memcpy (&cfg.addr4, buf_in_addr, sizeof (struct in_addr));
        } else {
          Logger::amf_app().error("In conversion: Bad value " AMF_CONFIG_STRING_IPV4_ADDRESS " = %s in config file", util::trim(words.at(0)).c_str());
          return RETURNerror;
        }
        cfg.network4.s_addr = htons(ntohs(cfg.addr4.s_addr) & 0xFFFFFFFF << (32 - std::stoi (util::trim(words.at(1)))));
      }
      if_cfg.lookupValue(AMF_CONFIG_STRING_SCTP_PORT, cfg.port);

      try {
        const Setting& sched_params_cfg = if_cfg[AMF_CONFIG_STRING_SCHED_PARAMS];
        load_thread_sched_params(sched_params_cfg, cfg.thread_rd_sched_params);
      } catch(const SettingNotFoundException &nfex) {
        Logger::amf_app().error("%s : %s, using defaults", nfex.what(), nfex.getPath());
      }
    }
    return RETURNok;
  }

int amf_config::load_thread_sched_params(const Setting& thread_sched_params_cfg, util::thread_sched_params& cfg)
{

  try {
    thread_sched_params_cfg.lookupValue(AMF_CONFIG_STRING_THREAD_RD_CPU_ID, cfg.cpu_id);
  } catch(const SettingNotFoundException &nfex) {
    Logger::amf_app().info("%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    std::string thread_rd_sched_policy;
    thread_sched_params_cfg.lookupValue(AMF_CONFIG_STRING_THREAD_RD_SCHED_POLICY, thread_rd_sched_policy);
    util::trim(thread_rd_sched_policy);
    if (boost::iequals(thread_rd_sched_policy, "SCHED_OTHER")) {
      cfg.sched_policy = SCHED_OTHER;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_IDLE")) {
      cfg.sched_policy = SCHED_IDLE;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_BATCH")) {
      cfg.sched_policy = SCHED_BATCH;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_FIFO")) {
      cfg.sched_policy = SCHED_FIFO;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_RR")) {
      cfg.sched_policy = SCHED_RR;
    } else {
      Logger::amf_app().error("thread_rd_sched_policy: %s, unknown in config file", thread_rd_sched_policy.c_str());
      return RETURNerror;
    }
  } catch(const SettingNotFoundException &nfex) {
    Logger::amf_app().info("%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    thread_sched_params_cfg.lookupValue(AMF_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY, cfg.sched_priority);
    if ((cfg.sched_priority > 99) || (cfg.sched_priority < 1)) {
      Logger::amf_app().error("thread_rd_sched_priority: %d, must be in interval [1..99] in config file", cfg.sched_priority);
      return RETURNerror;
    }
  } catch(const SettingNotFoundException &nfex) {
    Logger::amf_app().info("%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  return RETURNok;
}



}
