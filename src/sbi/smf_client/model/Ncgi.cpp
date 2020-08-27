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



#include "Ncgi.h"

namespace oai {
namespace smf {
namespace model {




Ncgi::Ncgi()
{
    m_NrCellId = utility::conversions::to_string_t("");
}

Ncgi::~Ncgi()
{
}

void Ncgi::validate()
{
    // TODO: implement validation
}

web::json::value Ncgi::toJson() const
{
    web::json::value val = web::json::value::object();

    val[utility::conversions::to_string_t("plmnId")] = ModelBase::toJson(m_PlmnId);
    val[utility::conversions::to_string_t("nrCellId")] = ModelBase::toJson(m_NrCellId);

    return val;
}

void Ncgi::fromJson(const web::json::value& val)
{
    std::shared_ptr<PlmnId> newPlmnId(new PlmnId());
    newPlmnId->fromJson(val.at(utility::conversions::to_string_t("plmnId")));
    setPlmnId( newPlmnId );
    setNrCellId(ModelBase::stringFromJson(val.at(utility::conversions::to_string_t("nrCellId"))));
}

void Ncgi::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    m_PlmnId->toMultipart(multipart, utility::conversions::to_string_t("plmnId."));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("nrCellId"), m_NrCellId));
}

void Ncgi::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    std::shared_ptr<PlmnId> newPlmnId(new PlmnId());
    newPlmnId->fromMultiPart(multipart, utility::conversions::to_string_t("plmnId."));
    setPlmnId( newPlmnId );
    setNrCellId(ModelBase::stringFromHttpContent(multipart->getContent(utility::conversions::to_string_t("nrCellId"))));
}

std::shared_ptr<PlmnId> Ncgi::getPlmnId() const
{
    return m_PlmnId;
}

void Ncgi::setPlmnId(const std::shared_ptr<PlmnId>& value)
{
    m_PlmnId = value;
    
}

utility::string_t Ncgi::getNrCellId() const
{
    return m_NrCellId;
}

void Ncgi::setNrCellId(const utility::string_t& value)
{
    m_NrCellId = value;
    
}

}
}
}


