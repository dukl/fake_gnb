#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
/*
 * To read content of the response from UDM
 */
#if 0
static std::size_t callback(
    const char* in,
    std::size_t size,
    std::size_t num,
    std::string* out)
{
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}
#endif
#define ENCODE_U8(buffer, value, size)    \
    *(uint8_t*)(buffer) = value;    \
    size += sizeof(uint8_t)
extern void msg_str_2_msg_hex(std::string msg, bstring &b);
extern void convert_string_2_hex(std::string&input, std::string&output);
extern void print_buffer(const std::string app, const std::string commit, uint8_t *buf, int len);
extern unsigned char * format_string_as_hex(std::string str);
extern std::size_t callback(
    const char* in,
    std::size_t size,
    std::size_t num,
    std::string* out);
extern bool multipart_parser(string input, string &jsonData, string &n1sm, string &n2sm);
//---------------------------------------------------------------------------------------------

void send_pdu_session_establishment_request()
{
  std::cout << "[AMF N11] PDU Session Establishment Request"<<std::endl;

  nlohmann::json pdu_session_establishment_request;
  std::string n1_msg = "2e0561c1ffff91a17b000780000a00000d00";
  std::string n2_msg;

  //format string as hex
  unsigned char *n1_msg_hex  = format_string_as_hex(n1_msg);

  //Fill Json part
  //get supi and put into URL
  std::string supi_str;
  std::string url = std::string("http://192.168.2.189:8889/nsmf-pdusession/v2/sm-contexts");


  //Fill the json part
  pdu_session_establishment_request["supi"] = "imsi-200000000000001";
  pdu_session_establishment_request["pei"] = "imei-200000000000001";
  pdu_session_establishment_request["gpsi"] = "msisdn-200000000001";
  pdu_session_establishment_request["dnn"] = "carrier.com";
  pdu_session_establishment_request["sNssai"]["sst"] = 222;
  pdu_session_establishment_request["sNssai"]["sd"] = "0000D4";
  pdu_session_establishment_request["pduSessionId"] = 5;
  pdu_session_establishment_request["requestType"] = "INITIAL_REQUEST";
  pdu_session_establishment_request["servingNfId"] = "servingNfId";
  pdu_session_establishment_request["servingNetwork"]["mcc"] = "234";
  pdu_session_establishment_request["servingNetwork"]["mnc"] = "067";
  pdu_session_establishment_request["anType"] = "3GPP_ACCESS";
  pdu_session_establishment_request["smContextStatusUri"] = "smContextStatusUri";

  pdu_session_establishment_request["n1MessageContainer"]["n1MessageClass"] = "SM";
  pdu_session_establishment_request["n1MessageContainer"]["n1MessageContent"]["contentId"] = "n1SmMsg"; //part 2

  //N1SM
  //pdu_session_establishment_request["n1SmMsg"] = "SM";
  //pdu_session_establishment_request["n1SmMsg"]["contentId"] = "n1SmMsg"; //part 2

  CURL *curl = curl_easy_init();

  //N1N2MessageTransfer Notification URI??
  std::string json_part = pdu_session_establishment_request.dump();

  std::cout<< " Sending message to SMF....\n";
  if(curl) {
    std::cout << "send curl command"<<std::endl;

    CURLcode res;
    struct curl_slist *headers = nullptr;
    struct curl_slist *slist = nullptr;
    curl_mime *mime;
    curl_mime *alt;
    curl_mimepart *part;

    //headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: multipart/related");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET,1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);

    mime = curl_mime_init(curl);
    alt = curl_mime_init(curl);

    //part with N1N2MessageTransferReqData (JsonData)
    part = curl_mime_addpart(mime);
    curl_mime_data(part, json_part.c_str(), CURL_ZERO_TERMINATED);
    curl_mime_type(part, "application/json");

    part = curl_mime_addpart(mime);
    curl_mime_data(part, reinterpret_cast<const char*>(n1_msg_hex), n1_msg.length()/2);
    //curl_mime_data(part, reinterpret_cast<const char*>(n1_msg_hex), CURL_ZERO_TERMINATED);
    curl_mime_type(part, "application/vnd.3gpp.5gnas");
    //curl_mime_name (part, "n1SmMsg");

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
    if(httpCode != 200 && httpCode != 201){
      if(!(multipart_parser(response, jsonData, n1sm, n2sm))){
        std::cout << "Could not get the cause from the response" <<std::endl;
      }
    }
    nlohmann::json response_data;
    try{
      response_data = nlohmann::json::parse(jsonData);
      Logger::amf_server().debug("Get response with jsonData: %s", jsonData.c_str());
      bstring n1sm_hex;
      msg_str_2_msg_hex(n1sm, n1sm_hex);
      print_buffer("amf_server", "Get response with n1sm:", (uint8_t*)bdata(n1sm_hex), blength(n1sm_hex));
    } catch (nlohmann::json::exception& e){
      //Set the default Cause
      response_data["cause"] = "504 Gateway Timeout";
    }
    std::cout << "[AMF N11] PDU session establishment request, response from SMF, Http Code " << httpCode << " cause  "<<  response_data["cause"].dump().c_str()<<std::endl;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
  }

}


void send_pdu_session_update_sm_context_establishment()
{
  std::cout << "[AMF N11] send_pdu_session_update_sm_context_establishment"<<std::endl;

  nlohmann::json pdu_session_modification_request;
  std::string n2_msg = "0003e0c0a8029d56ba263c003c";

  //format string as hex
  unsigned char *n2_msg_hex  = format_string_as_hex(n2_msg);

  //encode
  size_t buffer_size = 128;
  char *buffer = (char *)calloc(1,buffer_size);

  int  size = 0;

  ENCODE_U8 (buffer, 0 , size);
  ENCODE_U8 (buffer+size, 3 , size);
  ENCODE_U8 (buffer+size, 0xe0 , size);
  ENCODE_U8 (buffer+size, 0xac , size);
  ENCODE_U8 (buffer+size, 0x0a , size);
  ENCODE_U8 (buffer+size, 0x05 , size);
  ENCODE_U8 (buffer+size, 0x01 , size);
  ENCODE_U8 (buffer+size, 0x00 , size);
  ENCODE_U8 (buffer+size, 0x00 , size);
  ENCODE_U8 (buffer+size, 0x00 , size);
  ENCODE_U8 (buffer+size, 0x01 , size);
  ENCODE_U8 (buffer+size, 0x00 , size);
  ENCODE_U8 (buffer+size, 0x00 , size);


/*
  0000   00 00 04 00 82 00 04 00 01 00 02 00 8b 00 0a 01
  0010   f0 04 03 02 01 7f 00 00 01 00 86 00 01 10 00 88
  0020   00 07 00 3c 00 00 3c 00 00 00 00 00 00 00 00 00
  0030   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0040   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*/


  std::cout << "Buffer: "<<std::endl;
  for(int i=0;i<2;i++)
  {
    printf("%02x ", buffer[i]);
  }
  std::cout << "Buffer: "<<std::endl;

  //Fill Json part
  //get supi and put into URL
  std::string supi_str;
  //std::string url = std::string("http://172.16.1.101/nsmf-pdusession/v2/sm-contexts");
  //std::string url = std::string("http://172.16.1.101/nsmf-pdusession/v2/sm-contexts/imsi-200000000000001/modify");
  std::string url = std::string("http://192.168.2.189:8889/nsmf-pdusession/v2/sm-contexts/1/modify");

  //Fill the json part
  pdu_session_modification_request["n2SmInfoType"] = "PDU_RES_SETUP_RSP";
  pdu_session_modification_request["n2SmInfo"]["contentId"] = "n2SmMsg"; //part 2

  //N1SM
  //pdu_session_establishment_request["n1SmMsg"] = "SM";
  //pdu_session_establishment_request["n1SmMsg"]["contentId"] = "n1SmMsg"; //part 2

  CURL *curl = curl_easy_init();

  //N1N2MessageTransfer Notification URI??
  std::string json_part = pdu_session_modification_request.dump();

  std::cout<< " Sending message to SMF....\n";
  if(curl) {
    std::cout << "send curl command"<<std::endl;

    CURLcode res;
    struct curl_slist *headers = nullptr;
    struct curl_slist *slist = nullptr;
    curl_mime *mime;
    curl_mime *alt;
    curl_mimepart *part;

    headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: multipart/related");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET,1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);

    mime = curl_mime_init(curl);
    alt = curl_mime_init(curl);

    //part with N1N2MessageTransferReqData (JsonData)
    part = curl_mime_addpart(mime);
    curl_mime_data(part, json_part.c_str(), CURL_ZERO_TERMINATED);
    curl_mime_type(part, "application/json");

    part = curl_mime_addpart(mime);
    curl_mime_data(part, reinterpret_cast<const char*>(buffer), size);
    //curl_mime_data(part, "\x00\x03\xe0\xac\x0a\x05\x01\x01\x01\x01\x01\x00\x00", CURL_ZERO_TERMINATED);
    //curl_mime_data(part, "\x2e\x01\x01\xc1\xff\xff\x95", CURL_ZERO_TERMINATED);


    curl_mime_type(part, "application/vnd.3gpp.ngap");
    curl_mime_name (part, "n2SmMsg");

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
    nlohmann::json response_data;
    try{
      response_data = nlohmann::json::parse(*httpData.get());
    } catch (nlohmann::json::exception& e){
      std::cout << "Could not get the cause from the response" <<std::endl;
      //Set the default Cause
      response_data["cause"] = "504 Gateway Timeout";
    }
    std::cout << "[AMF N11] PDU session modification request, response from SMF, Http Code " << httpCode << " cause  "<<  response_data["cause"].dump().c_str()<<std::endl;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
  }

}

void send_pdu_session_update_sm_context_modification()
{
  std::cout << "[AMF N11] send_pdu_session_update_sm_context_modification"<<std::endl;

  nlohmann::json pdu_session_modification_request;
  std::string n2_msg = "0003e0ac0a0501000000010000";

  //format string as hex
  unsigned char *n2_msg_hex  = format_string_as_hex(n2_msg);

  //encode
  size_t buffer_size = 128;
  char *buffer = (char *)calloc(1,buffer_size);

  int  size = 0;

  ENCODE_U8 (buffer, 0x2e , size);
  ENCODE_U8 (buffer+size, 0x01 , size);
  ENCODE_U8 (buffer+size, 0x01 , size);
  ENCODE_U8 (buffer+size, 0xc1 , size);
  ENCODE_U8 (buffer+size, 0xff , size);
  ENCODE_U8 (buffer+size, 0xff , size);
  ENCODE_U8 (buffer+size, 0x95 , size);

  ////step 1.a,UE-initiated: SM Context ID + N1 (PDU Session Modification Request)


  std::cout << "Buffer: "<<std::endl;
  for(int i=0;i<2;i++)
  {
    printf("%02x ", buffer[i]);
  }
  std::cout << "Buffer: "<<std::endl;

  //Fill Json part
  //get supi and put into URL
  std::string supi_str;
  //std::string url = std::string("http://172.16.1.101/nsmf-pdusession/v2/sm-contexts");
  //std::string url = std::string("http://172.16.1.101/nsmf-pdusession/v2/sm-contexts/imsi-200000000000001/modify");
  std::string url = std::string("http://172.16.1.101/nsmf-pdusession/v2/sm-contexts/1/modify");

  //Fill the json part
  pdu_session_modification_request["n1SmMsg"]["contentId"] = "n1SmMsg"; //part 2

  //N1SM
  //pdu_session_establishment_request["n1SmMsg"] = "SM";
  //pdu_session_establishment_request["n1SmMsg"]["contentId"] = "n1SmMsg"; //part 2

  CURL *curl = curl_easy_init();

  //N1N2MessageTransfer Notification URI??
  std::string json_part = pdu_session_modification_request.dump();

  std::cout<< " Sending message to SMF....\n";
  if(curl) {
    std::cout << "send curl command"<<std::endl;

    CURLcode res;
    struct curl_slist *headers = nullptr;
    struct curl_slist *slist = nullptr;
    curl_mime *mime;
    curl_mime *alt;
    curl_mimepart *part;

    headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: multipart/related");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET,1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);

    mime = curl_mime_init(curl);
    alt = curl_mime_init(curl);

    //part with N1N2MessageTransferReqData (JsonData)
    part = curl_mime_addpart(mime);
    curl_mime_data(part, json_part.c_str(), CURL_ZERO_TERMINATED);
    curl_mime_type(part, "application/json");

    part = curl_mime_addpart(mime);
    curl_mime_data(part, reinterpret_cast<const char*>(buffer), size);
    //curl_mime_data(part, "\x00\x03\xe0\xac\x0a\x05\x01\x01\x01\x01\x01\x00\x00", CURL_ZERO_TERMINATED);
    //curl_mime_data(part, "\x2e\x01\x01\xc1\xff\xff\x95", CURL_ZERO_TERMINATED);


    curl_mime_type(part, "application/vnd.3gpp.5gnas");
    curl_mime_name (part, "n1SmMsg");

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    //res = curl_easy_perform(curl);

    // Response information.
    long httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    // Hook up data handling function.
    //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    //get cause from the response
    nlohmann::json response_data;
    try{
      response_data = nlohmann::json::parse(*httpData.get());
    } catch (nlohmann::json::exception& e){
      std::cout << "Could not get the cause from the response" <<std::endl;
      //Set the default Cause
      response_data["cause"] = "504 Gateway Timeout";
    }
    std::cout << "[AMF N11] PDU session modification request, response from SMF, Http Code " << httpCode << " cause  "<<  response_data["cause"].dump().c_str()<<std::endl;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
  }

}

int main_test()
{

  send_pdu_session_establishment_request();
  //usleep(100000);
  //send_pdu_session_update_sm_context_establishment();
  //send_pdu_session_update_sm_context_modification();
  return 0;
}


extern void octet_stream_2_hex_stream(uint8_t *buf, int len, string &out);
void hexTest(){
  string hexString;
  uint8_t test[4] = {0x12, 0x43, 0x00, 0x45};
  octet_stream_2_hex_stream(test, 4, hexString);
  printf("hexStream: %s\n", hexString.c_str());
}
