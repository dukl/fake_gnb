
#include "IndividualPDUSessionHSMFApi.h"
#include "IndividualSMContextApi.h"
#include "PDUSessionsCollectionApi.h"
#include "SMContextsCollectionApi.h"
#include "ApiConfiguration.h"
#include "ApiClient.h"
#include "nlohmann/json.hpp"

#include "logger.hpp"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#define DEFAULT_JSON_FILE  "/home/oai/oai-5g-amf/src/sbi/smf_client/inputs/SmContextCreateData.json"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams                            // JSON library
using namespace oai::smf::api;
using namespace oai::smf::model;
//using json = nlohmann::json;


int smf_client_test()
{

   // create ApiConfiguration
   std::shared_ptr <ApiConfiguration> apiConfiguration (new ApiConfiguration);
   apiConfiguration->setBaseUrl(utility::conversions::to_string_t("http://10.103.238.82:8889/nsmf-pdusession/v2"));
   //apiConfiguration->setBaseUrl(utility::conversions::to_string_t("http://192.168.2.189:8889/nsmf-pdusession/v2"));

   std::shared_ptr <ApiClient> apiClient(new ApiClient (apiConfiguration));

   std::shared_ptr<SmContextMessage> smContextMessage (new SmContextMessage);
   //fill the content of smContextMessage
   //TODO:
   std::shared_ptr<SmContextCreateData> smContextCreateData (new SmContextCreateData);

   Logger::amf_app().debug("starting test smf_client");

   json::value     jv;                                          // JSON read from input file

   try {
     Logger::amf_app().debug("try to get smcontextcreatedata json(%s)", DEFAULT_JSON_FILE);
          string_t        importFile = DEFAULT_JSON_FILE;           // extract filename
          ifstream_t      f(importFile);                              // filestream of working file
          stringstream_t  s;                                          // string stream for holding JSON read from file

          if (f) {
              s << f.rdbuf();                                         // stream results of reading from file stream into string stream
              f.close();                                              // close the filestream

              jv = json::value::parse(s);                                             // parse the resultant string stream.
              std::cout << "file" << DEFAULT_JSON_FILE<<std::endl;
          }else{
             Logger::amf_app().error("no json file");
          }
      }
      catch (web::json::json_exception excep) {
          //std::cout << "ERROR Parsing JSON: ";
          //std::cout << excep.what();
        Logger::amf_app().error("error!");
      }

      //auto supi = jv.at(U("supi"));
      //std::cout << supi <<std::endl;


   smContextCreateData->fromJson(jv);
   smContextMessage->setJsonData(smContextCreateData);


   std::shared_ptr<HttpContent> n1sm(new HttpContent);
   //n1sm->setName("\\\"n1SmMsg\\\"");
   //n1sm->setContentDisposition("attachment");
   n1sm->setContentType("application/vnd.3gpp.5gnas");
   //uint8_t buf[18] = {0x2e, 0x05, 0x1e, 0xc1, 0xff, 0xff, 0x91, 0xa1, 0x7b, 0x00, 0x07, 0x80, 0x00, 0xa0, 0x00, 0x00, 0x0d, 0x00};
   uint8_t buf[8] = {0x2e, 0x05, 0x1e, 0xc1, 0xff, 0xff, 0x91, 0xa1};
   char test[8];
   for(int i=0; i<8; i++)
       test[i] = (char)buf[i];
   printf("%s\n", test);
   string value = test;
   n1sm->setData( std::shared_ptr<std::istream>( new std::stringstream( utility::conversions::to_utf8string(value) ) ) );
   //n1sm->setData( std::shared_ptr<std::istream>( new std::stringstream( "2e051ec1ffff91a17b00078000a00000d00" ) ) );
   //n1sm->setData( std::shared_ptr<std::istream>(test));

   smContextMessage->setBinaryDataN1SmMessage(n1sm);

   std::shared_ptr<SMContextsCollectionApi> smContextsCollectionApi (new SMContextsCollectionApi (apiClient));
   smContextsCollectionApi->postSmContexts (smContextMessage);

   return 0;
}

