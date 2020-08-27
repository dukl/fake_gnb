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



#include "GbrQosFlowInformation.h"

namespace oai {
namespace smf {
namespace model {




GbrQosFlowInformation::GbrQosFlowInformation()
{
    m_MaxFbrDl = utility::conversions::to_string_t("");
    m_MaxFbrUl = utility::conversions::to_string_t("");
    m_GuaFbrDl = utility::conversions::to_string_t("");
    m_GuaFbrUl = utility::conversions::to_string_t("");
    m_NotifControlIsSet = false;
    m_MaxPacketLossRateDl = 0;
    m_MaxPacketLossRateDlIsSet = false;
    m_MaxPacketLossRateUl = 0;
    m_MaxPacketLossRateUlIsSet = false;
}

GbrQosFlowInformation::~GbrQosFlowInformation()
{
}

void GbrQosFlowInformation::validate()
{
    // TODO: implement validation
}

web::json::value GbrQosFlowInformation::toJson() const
{
    web::json::value val = web::json::value::object();

    val[utility::conversions::to_string_t("maxFbrDl")] = ModelBase::toJson(m_MaxFbrDl);
    val[utility::conversions::to_string_t("maxFbrUl")] = ModelBase::toJson(m_MaxFbrUl);
    val[utility::conversions::to_string_t("guaFbrDl")] = ModelBase::toJson(m_GuaFbrDl);
    val[utility::conversions::to_string_t("guaFbrUl")] = ModelBase::toJson(m_GuaFbrUl);
    if(m_NotifControlIsSet)
    {
        val[utility::conversions::to_string_t("notifControl")] = ModelBase::toJson(m_NotifControl);
    }
    if(m_MaxPacketLossRateDlIsSet)
    {
        val[utility::conversions::to_string_t("maxPacketLossRateDl")] = ModelBase::toJson(m_MaxPacketLossRateDl);
    }
    if(m_MaxPacketLossRateUlIsSet)
    {
        val[utility::conversions::to_string_t("maxPacketLossRateUl")] = ModelBase::toJson(m_MaxPacketLossRateUl);
    }

    return val;
}

void GbrQosFlowInformation::fromJson(const web::json::value& val)
{
    setMaxFbrDl(ModelBase::stringFromJson(val.at(utility::conversions::to_string_t("maxFbrDl"))));
    setMaxFbrUl(ModelBase::stringFromJson(val.at(utility::conversions::to_string_t("maxFbrUl"))));
    setGuaFbrDl(ModelBase::stringFromJson(val.at(utility::conversions::to_string_t("guaFbrDl"))));
    setGuaFbrUl(ModelBase::stringFromJson(val.at(utility::conversions::to_string_t("guaFbrUl"))));
    if(val.has_field(utility::conversions::to_string_t("notifControl")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("notifControl"));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<NotificationControl> newItem(new NotificationControl());
            newItem->fromJson(fieldValue);
            setNotifControl( newItem );
        }
    }
    if(val.has_field(utility::conversions::to_string_t("maxPacketLossRateDl")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("maxPacketLossRateDl"));
        if(!fieldValue.is_null())
        {
            setMaxPacketLossRateDl(ModelBase::int32_tFromJson(fieldValue));
        }
    }
    if(val.has_field(utility::conversions::to_string_t("maxPacketLossRateUl")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("maxPacketLossRateUl"));
        if(!fieldValue.is_null())
        {
            setMaxPacketLossRateUl(ModelBase::int32_tFromJson(fieldValue));
        }
    }
}

void GbrQosFlowInformation::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("maxFbrDl"), m_MaxFbrDl));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("maxFbrUl"), m_MaxFbrUl));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("guaFbrDl"), m_GuaFbrDl));
    multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("guaFbrUl"), m_GuaFbrUl));
    if(m_NotifControlIsSet)
    {
        if (m_NotifControl.get())
        {
            m_NotifControl->toMultipart(multipart, utility::conversions::to_string_t("notifControl."));
        }
    }
    if(m_MaxPacketLossRateDlIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("maxPacketLossRateDl"), m_MaxPacketLossRateDl));
    }
    if(m_MaxPacketLossRateUlIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("maxPacketLossRateUl"), m_MaxPacketLossRateUl));
    }
}

void GbrQosFlowInformation::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    setMaxFbrDl(ModelBase::stringFromHttpContent(multipart->getContent(utility::conversions::to_string_t("maxFbrDl"))));
    setMaxFbrUl(ModelBase::stringFromHttpContent(multipart->getContent(utility::conversions::to_string_t("maxFbrUl"))));
    setGuaFbrDl(ModelBase::stringFromHttpContent(multipart->getContent(utility::conversions::to_string_t("guaFbrDl"))));
    setGuaFbrUl(ModelBase::stringFromHttpContent(multipart->getContent(utility::conversions::to_string_t("guaFbrUl"))));
    if(multipart->hasContent(utility::conversions::to_string_t("notifControl")))
    {
        if(multipart->hasContent(utility::conversions::to_string_t("notifControl")))
        {
            std::shared_ptr<NotificationControl> newItem(new NotificationControl());
            newItem->fromMultiPart(multipart, utility::conversions::to_string_t("notifControl."));
            setNotifControl( newItem );
        }
    }
    if(multipart->hasContent(utility::conversions::to_string_t("maxPacketLossRateDl")))
    {
        setMaxPacketLossRateDl(ModelBase::int32_tFromHttpContent(multipart->getContent(utility::conversions::to_string_t("maxPacketLossRateDl"))));
    }
    if(multipart->hasContent(utility::conversions::to_string_t("maxPacketLossRateUl")))
    {
        setMaxPacketLossRateUl(ModelBase::int32_tFromHttpContent(multipart->getContent(utility::conversions::to_string_t("maxPacketLossRateUl"))));
    }
}

utility::string_t GbrQosFlowInformation::getMaxFbrDl() const
{
    return m_MaxFbrDl;
}

void GbrQosFlowInformation::setMaxFbrDl(const utility::string_t& value)
{
    m_MaxFbrDl = value;
    
}

utility::string_t GbrQosFlowInformation::getMaxFbrUl() const
{
    return m_MaxFbrUl;
}

void GbrQosFlowInformation::setMaxFbrUl(const utility::string_t& value)
{
    m_MaxFbrUl = value;
    
}

utility::string_t GbrQosFlowInformation::getGuaFbrDl() const
{
    return m_GuaFbrDl;
}

void GbrQosFlowInformation::setGuaFbrDl(const utility::string_t& value)
{
    m_GuaFbrDl = value;
    
}

utility::string_t GbrQosFlowInformation::getGuaFbrUl() const
{
    return m_GuaFbrUl;
}

void GbrQosFlowInformation::setGuaFbrUl(const utility::string_t& value)
{
    m_GuaFbrUl = value;
    
}

std::shared_ptr<NotificationControl> GbrQosFlowInformation::getNotifControl() const
{
    return m_NotifControl;
}

void GbrQosFlowInformation::setNotifControl(const std::shared_ptr<NotificationControl>& value)
{
    m_NotifControl = value;
    m_NotifControlIsSet = true;
}

bool GbrQosFlowInformation::notifControlIsSet() const
{
    return m_NotifControlIsSet;
}

void GbrQosFlowInformation::unsetNotifControl()
{
    m_NotifControlIsSet = false;
}

int32_t GbrQosFlowInformation::getMaxPacketLossRateDl() const
{
    return m_MaxPacketLossRateDl;
}

void GbrQosFlowInformation::setMaxPacketLossRateDl(int32_t value)
{
    m_MaxPacketLossRateDl = value;
    m_MaxPacketLossRateDlIsSet = true;
}

bool GbrQosFlowInformation::maxPacketLossRateDlIsSet() const
{
    return m_MaxPacketLossRateDlIsSet;
}

void GbrQosFlowInformation::unsetMaxPacketLossRateDl()
{
    m_MaxPacketLossRateDlIsSet = false;
}

int32_t GbrQosFlowInformation::getMaxPacketLossRateUl() const
{
    return m_MaxPacketLossRateUl;
}

void GbrQosFlowInformation::setMaxPacketLossRateUl(int32_t value)
{
    m_MaxPacketLossRateUl = value;
    m_MaxPacketLossRateUlIsSet = true;
}

bool GbrQosFlowInformation::maxPacketLossRateUlIsSet() const
{
    return m_MaxPacketLossRateUlIsSet;
}

void GbrQosFlowInformation::unsetMaxPacketLossRateUl()
{
    m_MaxPacketLossRateUlIsSet = false;
}

}
}
}


