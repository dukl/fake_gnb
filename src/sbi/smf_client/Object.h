/**
 * Nsmf_PDUSession
 * SMF PDU Session Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved. 
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.1.1-SNAPSHOT.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * Object.h
 *
 * This is the implementation of a JSON object.
 */

#ifndef OAI_SMF_MODEL_Object_H_
#define OAI_SMF_MODEL_Object_H_


#include "ModelBase.h"

#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>

namespace oai {
namespace smf {
namespace model {

class  Object : public ModelBase
{
public:
    Object();
    virtual ~Object();

    /////////////////////////////////////////////
    /// ModelBase overrides
    void validate() override;

    web::json::value toJson() const override;
    void fromJson(const web::json::value& json) override;

    void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    void fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// Object manipulation
    web::json::value getValue(const utility::string_t& key) const;
    void setValue(const utility::string_t& key, const web::json::value& value);

private:
    web::json::value m_object;
};

}
}
}

#endif /* OAI_SMF_MODEL_Object_H_ */