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


#include "SeafData.h"

namespace oai {
namespace amf {
namespace model {

SeafData::SeafData()
{
    m_Nh = "";
    m_NhIsSet = false;
    m_Ncc = 0;
    m_NccIsSet = false;
    m_KeyAmfChangeInd = false;
    m_KeyAmfChangeIndIsSet = false;
    m_KeyAmfHDerivationInd = false;
    m_KeyAmfHDerivationIndIsSet = false;
    
}

SeafData::~SeafData()
{
}

void SeafData::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const SeafData& o)
{
    j = nlohmann::json();
    j["ngKsi"] = o.m_NgKsi;
    j["keyAmf"] = o.m_KeyAmf;
    if(o.nhIsSet())
        j["nh"] = o.m_Nh;
    if(o.nccIsSet())
        j["ncc"] = o.m_Ncc;
    if(o.keyAmfChangeIndIsSet())
        j["keyAmfChangeInd"] = o.m_KeyAmfChangeInd;
    if(o.keyAmfHDerivationIndIsSet())
        j["keyAmfHDerivationInd"] = o.m_KeyAmfHDerivationInd;
}

void from_json(const nlohmann::json& j, SeafData& o)
{
    j.at("ngKsi").get_to(o.m_NgKsi);
    j.at("keyAmf").get_to(o.m_KeyAmf);
    if(j.find("nh") != j.end())
    {
        j.at("nh").get_to(o.m_Nh);
        o.m_NhIsSet = true;
    } 
    if(j.find("ncc") != j.end())
    {
        j.at("ncc").get_to(o.m_Ncc);
        o.m_NccIsSet = true;
    } 
    if(j.find("keyAmfChangeInd") != j.end())
    {
        j.at("keyAmfChangeInd").get_to(o.m_KeyAmfChangeInd);
        o.m_KeyAmfChangeIndIsSet = true;
    } 
    if(j.find("keyAmfHDerivationInd") != j.end())
    {
        j.at("keyAmfHDerivationInd").get_to(o.m_KeyAmfHDerivationInd);
        o.m_KeyAmfHDerivationIndIsSet = true;
    } 
}

NgKsi SeafData::getNgKsi() const
{
    return m_NgKsi;
}
void SeafData::setNgKsi(NgKsi const& value)
{
    m_NgKsi = value;
    
}
KeyAmf SeafData::getKeyAmf() const
{
    return m_KeyAmf;
}
void SeafData::setKeyAmf(KeyAmf const& value)
{
    m_KeyAmf = value;
    
}
std::string SeafData::getNh() const
{
    return m_Nh;
}
void SeafData::setNh(std::string const& value)
{
    m_Nh = value;
    m_NhIsSet = true;
}
bool SeafData::nhIsSet() const
{
    return m_NhIsSet;
}
void SeafData::unsetNh()
{
    m_NhIsSet = false;
}
int32_t SeafData::getNcc() const
{
    return m_Ncc;
}
void SeafData::setNcc(int32_t const value)
{
    m_Ncc = value;
    m_NccIsSet = true;
}
bool SeafData::nccIsSet() const
{
    return m_NccIsSet;
}
void SeafData::unsetNcc()
{
    m_NccIsSet = false;
}
bool SeafData::isKeyAmfChangeInd() const
{
    return m_KeyAmfChangeInd;
}
void SeafData::setKeyAmfChangeInd(bool const value)
{
    m_KeyAmfChangeInd = value;
    m_KeyAmfChangeIndIsSet = true;
}
bool SeafData::keyAmfChangeIndIsSet() const
{
    return m_KeyAmfChangeIndIsSet;
}
void SeafData::unsetKeyAmfChangeInd()
{
    m_KeyAmfChangeIndIsSet = false;
}
bool SeafData::isKeyAmfHDerivationInd() const
{
    return m_KeyAmfHDerivationInd;
}
void SeafData::setKeyAmfHDerivationInd(bool const value)
{
    m_KeyAmfHDerivationInd = value;
    m_KeyAmfHDerivationIndIsSet = true;
}
bool SeafData::keyAmfHDerivationIndIsSet() const
{
    return m_KeyAmfHDerivationIndIsSet;
}
void SeafData::unsetKeyAmfHDerivationInd()
{
    m_KeyAmfHDerivationIndIsSet = false;
}

}
}
}
