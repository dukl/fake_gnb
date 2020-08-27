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



#include "NgApCause.h"

namespace oai {
namespace smf {
namespace model {




NgApCause::NgApCause()
{
    m_Group = 0;
    m_Value = 0;
}

NgApCause::~NgApCause()
{
}

void NgApCause::validate()
{
    // TODO: implement validation
}

web::json::value NgApCause::toJson() const
{
    web::json::value val = web::json::value::object();

    val[utility::conversions::to_string_t("group")] = ModelBase::toJson(m_Group);
    val[utility::conversions::to_string_t("value")] = ModelBase::toJson(m_Value);

    return val;
}

void NgApCause::fromJson(const web::json::value& val)
{
    setGroup(ModelBase::int32_tFromJson(val.at(utility::conversions::to_string_t("group"))));
    setValue(ModelBase::int32_tFromJson(val.at(utility::conversions::to_string_t("value"))));
}

void NgApCause::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("group"), m_Group));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("value"), m_Value));
}

void NgApCause::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    setGroup(ModelBase::int32_tFromHttpContent(multipart->getContent(utility::conversions::to_string_t("group"))));
    setValue(ModelBase::int32_tFromHttpContent(multipart->getContent(utility::conversions::to_string_t("value"))));
}

int32_t NgApCause::getGroup() const
{
    return m_Group;
}

void NgApCause::setGroup(int32_t value)
{
    m_Group = value;
    
}

int32_t NgApCause::getValue() const
{
    return m_Value;
}

void NgApCause::setValue(int32_t value)
{
    m_Value = value;
    
}

}
}
}


