/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "asn.1/Information Element Definitions.asn1"
 * 	`asn1c -pdu=all -fcompound-names -fno-include-deps -findirect-choice -gen-PER -D src`
 */

#include "Ngap_QosFlowNotifyItem.h"

#include "Ngap_ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_Ngap_QosFlowNotifyItem_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_QosFlowNotifyItem, qosFlowIdentifier),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_QosFlowIdentifier,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"qosFlowIdentifier"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_QosFlowNotifyItem, notificationCause),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_NotificationCause,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"notificationCause"
		},
	{ ATF_POINTER, 1, offsetof(struct Ngap_QosFlowNotifyItem, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_ProtocolExtensionContainer_175P131,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_Ngap_QosFlowNotifyItem_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_Ngap_QosFlowNotifyItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Ngap_QosFlowNotifyItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* qosFlowIdentifier */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* notificationCause */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_Ngap_QosFlowNotifyItem_specs_1 = {
	sizeof(struct Ngap_QosFlowNotifyItem),
	offsetof(struct Ngap_QosFlowNotifyItem, _asn_ctx),
	asn_MAP_Ngap_QosFlowNotifyItem_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_Ngap_QosFlowNotifyItem_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Ngap_QosFlowNotifyItem = {
	"QosFlowNotifyItem",
	"QosFlowNotifyItem",
	&asn_OP_SEQUENCE,
	asn_DEF_Ngap_QosFlowNotifyItem_tags_1,
	sizeof(asn_DEF_Ngap_QosFlowNotifyItem_tags_1)
		/sizeof(asn_DEF_Ngap_QosFlowNotifyItem_tags_1[0]), /* 1 */
	asn_DEF_Ngap_QosFlowNotifyItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_Ngap_QosFlowNotifyItem_tags_1)
		/sizeof(asn_DEF_Ngap_QosFlowNotifyItem_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Ngap_QosFlowNotifyItem_1,
	3,	/* Elements count */
	&asn_SPC_Ngap_QosFlowNotifyItem_specs_1	/* Additional specs */
};

