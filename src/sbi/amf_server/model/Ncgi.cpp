/**
* Namf_Communication
* AMF Communication Service © 2019, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved. 
*
* The version of the OpenAPI document: 1.1.0.alpha-1
* 
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/


#include "Ncgi.h"

namespace oai {
namespace amf {
namespace model {

Ncgi::Ncgi()
{
    m_NrCellId = "";
    
}

Ncgi::~Ncgi()
{
}

void Ncgi::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const Ncgi& o)
{
    j = nlohmann::json();
    j["plmnId"] = o.m_PlmnId;
    j["nrCellId"] = o.m_NrCellId;
}

void from_json(const nlohmann::json& j, Ncgi& o)
{
    j.at("plmnId").get_to(o.m_PlmnId);
    j.at("nrCellId").get_to(o.m_NrCellId);
}

PlmnId Ncgi::getPlmnId() const
{
    return m_PlmnId;
}
void Ncgi::setPlmnId(PlmnId const& value)
{
    m_PlmnId = value;
    
}
std::string Ncgi::getNrCellId() const
{
    return m_NrCellId;
}
void Ncgi::setNrCellId(std::string const& value)
{
    m_NrCellId = value;
    
}

}
}
}

