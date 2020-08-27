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
/*
 * AssignEbiFailed.h
 *
 * 
 */

#ifndef AssignEbiFailed_H_
#define AssignEbiFailed_H_


#include <vector>
#include "Arp.h"
#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
/// 
/// </summary>
class  AssignEbiFailed
{
public:
    AssignEbiFailed();
    virtual ~AssignEbiFailed();

    void validate();

    /////////////////////////////////////////////
    /// AssignEbiFailed members

    /// <summary>
    /// 
    /// </summary>
    int32_t getPduSessionId() const;
    void setPduSessionId(int32_t const value);
        /// <summary>
    /// 
    /// </summary>
    std::vector<Arp>& getFailedArpList();
    bool failedArpListIsSet() const;
    void unsetFailedArpList();

    friend void to_json(nlohmann::json& j, const AssignEbiFailed& o);
    friend void from_json(const nlohmann::json& j, AssignEbiFailed& o);
protected:
    int32_t m_PduSessionId;

    std::vector<Arp> m_FailedArpList;
    bool m_FailedArpListIsSet;
};

}
}
}

#endif /* AssignEbiFailed_H_ */
