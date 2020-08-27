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



#include "PlmnId.h"

namespace oai {
namespace smf {
namespace model {




PlmnId::PlmnId()
{
    m_Mcc = utility::conversions::to_string_t("");
    m_Mnc = utility::conversions::to_string_t("");
}

PlmnId::~PlmnId()
{
}

void PlmnId::validate()
{
    // TODO: implement validation
}

web::json::value PlmnId::toJson() const
{
    web::json::value val = web::json::value::object();

    val[utility::conversions::to_string_t("mcc")] = ModelBase::toJson(m_Mcc);
    val[utility::conversions::to_string_t("mnc")] = ModelBase::toJson(m_Mnc);

    return val;
}

void PlmnId::fromJson(const web::json::value& val)
{
    setMcc(ModelBase::stringFromJson(val.at(utility::conversions::to_string_t("mcc"))));
    setMnc(ModelBase::stringFromJson(val.at(utility::conversions::to_string_t("mnc"))));
}

void PlmnId::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("mcc"), m_Mcc));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("mnc"), m_Mnc));
}

void PlmnId::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    setMcc(ModelBase::stringFromHttpContent(multipart->getContent(utility::conversions::to_string_t("mcc"))));
    setMnc(ModelBase::stringFromHttpContent(multipart->getContent(utility::conversions::to_string_t("mnc"))));
}

utility::string_t PlmnId::getMcc() const
{
    return m_Mcc;
}

void PlmnId::setMcc(const utility::string_t& value)
{
    m_Mcc = value;
    
}

utility::string_t PlmnId::getMnc() const
{
    return m_Mnc;
}

void PlmnId::setMnc(const utility::string_t& value)
{
    m_Mnc = value;
    
}

}
}
}

