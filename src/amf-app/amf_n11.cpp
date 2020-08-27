#include "amf_n11.hpp"
#include "amf_n1.hpp"
#include "itti.hpp"
#include "itti_msg_amf_app.hpp"
#include "amf_config.hpp"
#include "nas_context.hpp"

#include <curl/curl.h>
#include <nlohmann/json.hpp>
/************* for smf_client ***************/
#include "SmContextCreateData.h"
#include "SMContextsCollectionApi.h"
#include "ApiConfiguration.h"
#include "ApiClient.h"

#include "3gpp_ts24501.hpp"

using namespace oai::smf::model;
using namespace oai::smf::api;
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;

using namespace config;
using namespace amf_application;
extern itti_mw * itti_inst;
extern amf_config amf_cfg;
extern amf_n11 * amf_n11_inst;

extern amf_n1  * amf_n1_inst;

extern void msg_str_2_msg_hex(std::string msg, bstring &b);
extern void convert_string_2_hex(std::string&input, std::string&output);
extern void print_buffer(const std::string app, const std::string commit, uint8_t *buf, int len);
extern bool multipart_parser(string input, string &jsonData, string &n1sm, string &n2sm);
extern unsigned char * format_string_as_hex(std::string str);
//extern std::size_t callback(const char* in, std::size_t size, std::size_t num, std::string* out);
extern char* bstring2charString(bstring b);

std::size_t callback(
    const char* in,
    std::size_t size,
    std::size_t num,
    std::string* out)
{
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

void octet_stream_2_hex_stream(uint8_t *buf, int len, string &out){
  out = "";
  char *tmp = (char*)calloc(1, 2*len*sizeof(uint8_t)+1);
  for(int i=0; i<len; i++){
    sprintf(tmp+2*i, "%02x", buf[i]);
  }
  tmp[2*len] = '\0';
  out = tmp;
  printf("n1sm buffer: %s\n", out.c_str());
}


/****************************************************/
/**  used to run NF(s) consumer, like smf_client ****/
/***************************************************/

void amf_n11_task(void*);
void amf_n11_task(void*){
  const task_id_t task_id = TASK_AMF_N11;
  itti_inst->notify_task_ready(task_id);
  do{
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto *msg = shared_msg.get();
    switch(msg->msg_type){
      case SMF_SERVICES_CONSUMER:{
        Logger::task_amf_n11().info("running SMF_SERVICES_CONSUMER");
        itti_smf_services_consumer *m = dynamic_cast<itti_smf_services_consumer*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m)); 
      }break;
      case NSMF_PDU_SESS_UPDATE_SMCTX:{
        Logger::task_amf_n11().info("receive NSMF_PDU_SESS_UPDATE_SMCTX, handling ...");
        itti_nsmf_pdusession_update_sm_context *m = dynamic_cast<itti_nsmf_pdusession_update_sm_context*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m)); 
      }break;
      case PDU_SESS_RES_SET_RESP:{
        Logger::task_amf_n11().info("receive PDU_SESS_RES_SET_RESP, handling ...");
        itti_pdu_session_resource_setup_response *m = dynamic_cast<itti_pdu_session_resource_setup_response*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m)); 
      }break;

     case NSMF_PDU_SESS_RELEASE_SMCTX: {
		  Logger::task_amf_n11().info("receive NSMF_PDU_SESS_RELEASE_SMCTX, handling ...");
		  itti_nsmf_pdusession_release_sm_context *m = dynamic_cast<itti_nsmf_pdusession_release_sm_context*>(msg);
		  amf_n11_inst->handle_itti_message(ref(*m));
     }break;


    }
  }while(true);
}

amf_n11::amf_n11(){
  if(itti_inst->create_task(TASK_AMF_N11, amf_n11_task, nullptr) ) {
    Logger::amf_n11().error( "Cannot create task TASK_AMF_N1" );
    throw std::runtime_error( "Cannot create task TASK_AMF_N1" );
  }
  Logger::task_amf_n11().startup( "Started" );
  Logger::task_amf_n11().debug("construct amf_n1 successfully");
}

amf_n11::~amf_n11(){}

/***************************** itti message handlers *********************************/
void amf_n11::handle_itti_message(itti_pdu_session_resource_setup_response &itti_msg){}
void amf_n11::handle_itti_message(itti_nsmf_pdusession_update_sm_context &itti_msg){
  //string supi = pduid2supi.at(itti_msg.pdu_session_id);
  string supi = itti_msg.supi;
  Logger::amf_n11().debug("Try to find supi(%s) from pdusession_id(%d)", supi.c_str(), itti_msg.pdu_session_id);
  std::shared_ptr<pdu_session_context> psc;
  if(is_supi_to_pdu_ctx(supi)){
    psc = supi_to_pdu_ctx(supi);
  }else{
    Logger::amf_n11().error("trying to find psu_session_context with supi(%s), Falied", supi.c_str());
    return;
  }
  string smf_addr;
  if(!psc.get()->smf_avaliable){
    if(!smf_selection_from_configuration(smf_addr)){
      Logger::amf_n11().error("No candidate smf is avaliable");
      return;
    }
  }else{
    smf_selection_from_context(smf_addr);
  }

  std::shared_ptr<pdu_session_context> context;
  context = supi_to_pdu_ctx(supi);
  string remote_uri = context.get()->location+ "/modify";

  //remote_uri = smf_addr + "/nsmf-pdusession/v2/sm-contexts/" + "1" + "/modify";//scid
  Logger::amf_n11().debug("remote uri================================================%s", remote_uri.c_str());
  nlohmann::json pdu_session_update_request;
  pdu_session_update_request["n2SmInfoType"] = "PDU_RES_SETUP_RSP";
  pdu_session_update_request["n2SmInfo"]["contentId"] = "n2SmMsg";
  std::string json_part = pdu_session_update_request.dump();
  std::string n2SmMsg;
  octet_stream_2_hex_stream((uint8_t*)bdata(itti_msg.n2sm), blength(itti_msg.n2sm), n2SmMsg);
  curl_http_client(remote_uri ,json_part, "", n2SmMsg, supi, itti_msg.pdu_session_id);
}

void amf_n11::handle_itti_message(itti_smf_services_consumer& smf){
  std::shared_ptr<nas_context> nc;
  nc = amf_n1_inst->amf_ue_id_2_nas_context(smf.amf_ue_ngap_id);
  string supi = "imsi-" + nc.get()->imsi;

  std::shared_ptr<pdu_session_context> psc;
  if(is_supi_to_pdu_ctx(supi)){
    psc = supi_to_pdu_ctx(supi);
  }else{
    psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
    set_supi_to_pdu_ctx(supi, psc);
  }
  
  pduid2supi[smf.pdu_sess_id] = supi;

  psc.get()->amf_ue_ngap_id = nc.get()->amf_ue_ngap_id;
  psc.get()->ran_ue_ngap_id = nc.get()->ran_ue_ngap_id;
  psc.get()->req_type = smf.req_type;
  psc.get()->pdu_session_id = smf.pdu_sess_id;
  //psc.get()->isn2sm_avaliable = false;
  //parse binary dnn and store
  std::string dnn = "default";
  if ((smf.dnn != nullptr) && (blength(smf.dnn) > 0)){
    char * tmp = bstring2charString(smf.dnn);
    dnn = tmp;
    free (tmp);
    tmp = nullptr;
  }

  Logger::amf_n11().debug("requested DNN: %s", dnn.c_str());
  psc.get()->dnn = dnn;

  string smf_addr;
  if(!psc.get()->smf_avaliable){
    if(!smf_selection_from_configuration(smf_addr)){
      Logger::amf_n11().error("No candidate smf is avaliable");
      return;
    }
  }else{
    smf_selection_from_context(smf_addr);
  }

  switch(smf.req_type & 0x07){
    case PDU_SESSION_INITIAL_REQUEST:{
      //get pti
      uint8_t *sm_msg = (uint8_t*)bdata(smf.sm_msg);
      uint8_t pti = sm_msg[2];
      Logger::amf_n1().debug("decoded PTI for PDUSessionEstablishmentRequest(0x%x)", pti);
      if(psc.get()->isn1sm_avaliable && psc.get()->isn2sm_avaliable){
        itti_n1n2_message_transfer_request * itti_msg = new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);
        itti_msg->supi = supi;
        
        uint8_t accept_len = blength(psc.get()->n1sm);
        uint8_t *accept = (uint8_t*)calloc(1, accept_len);
        memcpy(accept, (uint8_t*)bdata(psc.get()->n1sm), accept_len);
        accept[2] = pti;
        itti_msg->n1sm = blk2bstr(accept, accept_len);
        itti_msg->is_n1sm_set = true;
        itti_msg->n2sm = psc.get()->n2sm;
        itti_msg->is_n2sm_set = true;
        itti_msg->pdu_session_id = psc.get()->pdu_session_id;
        std::shared_ptr<itti_n1n2_message_transfer_request> i = std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (0 != ret) {
          Logger::amf_server().error( "Could not send ITTI message %s to task TASK_AMF_APP", i->get_msg_name());
        } 
      }else{
        psc.get()->isn2sm_avaliable = false;
        handle_pdu_session_initial_request(supi, psc, smf_addr, smf.sm_msg, dnn);
      }
    }break;
    case EXISTING_PDU_SESSION:{
    }break;
    case PDU_SESSION_MODIFICATION_REQUEST:{
    }break;
  }
}

void amf_n11::handle_pdu_session_initial_request(string supi, std::shared_ptr<pdu_session_context> psc, string smf_addr, bstring sm_msg, string dnn){
  string remote_uri = smf_addr + "/nsmf-pdusession/v2/sm-contexts";
  nlohmann::json pdu_session_establishment_request;
  pdu_session_establishment_request["supi"] = supi.c_str();
  pdu_session_establishment_request["pei"] = "imei-200000000000001";
  pdu_session_establishment_request["gpsi"] = "msisdn-200000000001";
  pdu_session_establishment_request["dnn"] = dnn.c_str();
  pdu_session_establishment_request["sNssai"]["sst"] = 1;
  pdu_session_establishment_request["sNssai"]["sd"] = "0";
  pdu_session_establishment_request["pduSessionId"] = psc.get()->pdu_session_id;
  pdu_session_establishment_request["requestType"] = "INITIAL_REQUEST";
  pdu_session_establishment_request["servingNfId"] = "servingNfId";
  //pdu_session_establishment_request["servingNetwork"]["mcc"] = "460";
  //pdu_session_establishment_request["servingNetwork"]["mnc"] = "011";
  pdu_session_establishment_request["servingNetwork"]["mcc"] = "110";
  pdu_session_establishment_request["servingNetwork"]["mnc"] = "011";
  pdu_session_establishment_request["anType"] = "3GPP_ACCESS";
  pdu_session_establishment_request["smContextStatusUri"] = "smContextStatusUri";

  pdu_session_establishment_request["n1MessageContainer"]["n1MessageClass"] = "SM";
  pdu_session_establishment_request["n1MessageContainer"]["n1MessageContent"]["contentId"] = "n1SmMsg";

  std::string json_part = pdu_session_establishment_request.dump();
  std::string n1SmMsg;
  octet_stream_2_hex_stream((uint8_t*)bdata(sm_msg), blength(sm_msg), n1SmMsg);
  curl_http_client(remote_uri ,json_part, n1SmMsg, "", supi, psc.get()->pdu_session_id); 
}

void amf_n11::handle_itti_message(itti_nsmf_pdusession_release_sm_context &itti_msg) {
        std::shared_ptr<pdu_session_context> psc = supi_to_pdu_ctx(itti_msg.supi);
        string smf_addr;
  	if(!psc.get()->smf_avaliable){
    		if(!smf_selection_from_configuration(smf_addr)){
      			Logger::amf_n11().error("No candidate smf is avaliable");
      			return;
    	}
  	}else{
    		smf_selection_from_context(smf_addr);
  	}
        string remote_uri = psc.get()->location +"release";
	nlohmann::json pdu_session_release_request;
	pdu_session_release_request["supi"] = itti_msg.supi.c_str();
	pdu_session_release_request["dnn"] = psc.get()->dnn.c_str();
	pdu_session_release_request["sNssai"]["sst"] = 1;
	pdu_session_release_request["sNssai"]["sd"] = "0";
	pdu_session_release_request["pduSessionId"] = psc.get()->pdu_session_id;
	pdu_session_release_request["cause"] = "REL_DUE_TO_REACTIVATION";
	pdu_session_release_request["ngApCause"] = "radioNetwork";
	std::string json_part = pdu_session_release_request.dump();
	curl_http_client(remote_uri, json_part, "", "", itti_msg.supi, psc.get()->pdu_session_id);
}

/************************************************* context management functions *********************************/

bool amf_n11::is_supi_to_pdu_ctx(const string &supi) const {
  std::shared_lock lock(m_supi2pdu);
  return bool{supi2pdu.count(supi) > 0};
}

std::shared_ptr<pdu_session_context> amf_n11::supi_to_pdu_ctx(const string & supi) const {
  std::shared_lock lock(m_supi2pdu);
  return supi2pdu.at(supi);
}

void amf_n11::set_supi_to_pdu_ctx(const string &supi, std::shared_ptr<pdu_session_context> psc){
  std::shared_lock lock(m_supi2pdu);
  supi2pdu[supi] = psc;
}

/************************************** smf selection ********************************/
bool amf_n11::smf_selection_from_configuration(string & smf_addr){
  for(int i=0; i<amf_cfg.smf_pool.size(); i++){
    if(amf_cfg.smf_pool[i].selected){
      smf_addr = "http://"+amf_cfg.smf_pool[i].ipv4+":"+amf_cfg.smf_pool[i].port;
      return true;
    }
  }
  return false;
}
bool amf_n11::smf_selection_from_context(string & smf_addr){
}

/************************************* handlers for smf client response **************************/
void amf_n11::handle_post_sm_context_response_error_400(){}
void amf_n11::handle_post_sm_context_response_error(long code, string cause, bstring n1sm, string supi, uint8_t pdu_session_id){
  print_buffer("amf_n11", "n1 sm", (uint8_t*)bdata(n1sm), blength(n1sm));
  itti_n1n2_message_transfer_request *itti_msg = new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);
  itti_msg->n1sm = n1sm;
  itti_msg->is_n2sm_set = false;
  itti_msg->supi = supi;
  itti_msg->pdu_session_id = pdu_session_id;
  std::shared_ptr<itti_n1n2_message_transfer_request> i = std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_n1().error( "Could not send ITTI message %s to task TASK_AMF_APP", i->get_msg_name());
    }
}

struct header_info{
	char *optbuf;
	char *buffer;
	int bufsize;
};

static int HeaderInfoInit(struct header_info &info,std::string &opt,int buffersize)
{
	if(opt.size() > 0)
	{
		info.optbuf = opt.data();
	}
	else
	{
		return -1;
	}
	info.buffer = new char[buffersize];
	info.bufsize = 0;

	return 0;
}
static void HeaderInfoDeleteInit(struct header_info &info)
{
	info.optbuf = NULL;
	delete [] info.buffer;
	info.bufsize = 0;
}
static size_t header_callback(char *buffer, size_t size,size_t nitems, void *userdata)
{
	struct header_info *info = (struct header_info *)userdata;
	size_t buffer_size = nitems*size;

	if(info->optbuf && info->buffer && (buffer_size > strlen(info->optbuf)))
	{
		if(!strncmp(info->optbuf,buffer,strlen(info->optbuf)))
		{
			info->bufsize = buffer_size;
			memcpy(info->buffer,buffer,buffer_size);

			//delete "\r\n"
			if(info->buffer[info->bufsize -2] == '\r' && info->buffer[info->bufsize-1] == '\n')
			{
				info->buffer[info->bufsize-2] = '\0';
				info->bufsize -= 2;
			}
		}
	}

	return buffer_size;
}
void amf_n11::curl_http_client(string remoteUri, string jsonData, string n1SmMsg, string n2SmMsg, string supi, uint8_t pdu_session_id){
  Logger::amf_n11().debug("call smf service operation: %s", remoteUri.c_str());
  CURL *curl = curl_easy_init();
  if(curl){
    CURLcode res;
    struct curl_slist *headers = nullptr;
    struct curl_slist *slist = nullptr;
    curl_mime *mime;
    curl_mime *alt;
    curl_mimepart *part;

    //headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: multipart/related");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, remoteUri.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET,1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);

struct header_info locationmsg;
	std::string location = "Location";

	HeaderInfoInit(locationmsg,location,256);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &locationmsg);

    mime = curl_mime_init(curl);
    alt = curl_mime_init(curl);

    //part with N1N2MessageTransferReqData (JsonData)
    part = curl_mime_addpart(mime);
    curl_mime_data(part, jsonData.c_str(), CURL_ZERO_TERMINATED);
    curl_mime_type(part, "application/json");

    if(n1SmMsg != ""){
      Logger::amf_n11().debug("is there ok? n1");
      unsigned char *n1_msg_hex  = format_string_as_hex(n1SmMsg);
      //Logger::amf_n11().debug("n1 msg hex: %s", n1_msg_hex);
      part = curl_mime_addpart(mime);
      curl_mime_data(part, reinterpret_cast<const char*>(n1_msg_hex), n1SmMsg.length()/2);
      curl_mime_type(part, "application/vnd.3gpp.5gnas");
      //curl_mime_name (part, "n1SmMsg");
    }


    if(n2SmMsg != ""){
      unsigned char *n2_msg_hex  = format_string_as_hex(n2SmMsg);
      part = curl_mime_addpart(mime);
      curl_mime_data(part, reinterpret_cast<const char*>(n2_msg_hex), n2SmMsg.length()/2);
      curl_mime_type(part, "application/vnd.3gpp.ngap");
      //curl_mime_name (part, "n2SmMsg");
    }
    
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    //res = curl_easy_perform(curl);

    // Response information.
    long httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    //get cause from the response
    string response = *httpData.get();
    string jsonData = "";
    string n1sm = "";
    string n2sm = "";
    bool is_response_ok = true;
    Logger::amf_n11().debug("Get response with httpcode(%d)", httpCode);
    if(httpCode == 0){
      Logger::amf_n11().error("Cannot get response When calling %s", remoteUri.c_str());
      return;
    }
    if(httpCode != 200 && httpCode != 201){
      is_response_ok = false;
      if(!(multipart_parser(response, jsonData, n1sm, n2sm))){
        Logger::amf_n11().error("Could not get the cause from the response");
      }
    }


   if (httpCode == 201)
    {
	
      std::shared_ptr<pdu_session_context> context;
      context = supi_to_pdu_ctx(supi);
      string locationinfo_string = locationmsg.buffer;
      context->location="http://"+locationinfo_string.substr(10,string::npos);
      HeaderInfoDeleteInit(locationmsg);		
      Logger::amf_n11().debug("context.location in 201 response========================================%s===", context->location.c_str());
    } 
    nlohmann::json response_data;
    bstring n1sm_hex;
   
    if(!is_response_ok){
      response_data = nlohmann::json::parse(jsonData);
      Logger::amf_n11().debug("Get response with jsonData: %s", jsonData.c_str());
      msg_str_2_msg_hex(n1sm.substr(0, n1sm.length()-2), n1sm_hex);//pdu session establishment reject bugs from SMF
      print_buffer("amf_n11", "Get response with n1sm:", (uint8_t*)bdata(n1sm_hex), blength(n1sm_hex));
    
      string cause = response_data["error"]["cause"];
      Logger::amf_n11().error("call Network Function services failure ");
      Logger::amf_n11().info("Cause value: %s", cause.c_str());
      if(!cause.compare("DNN_DENIED")) handle_post_sm_context_response_error(httpCode, cause, n1sm_hex, supi, pdu_session_id);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
  }
}
