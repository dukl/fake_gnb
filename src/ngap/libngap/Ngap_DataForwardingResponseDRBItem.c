/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "asn.1/Information Element Definitions.asn1"
 * 	`asn1c -pdu=all -fcompound-names -fno-include-deps -findirect-choice -gen-PER -D src`
 */

#include "Ngap_DataForwardingResponseDRBItem.h"

#include "Ngap_UPTransportLayerInformation.h"
#include "Ngap_ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_Ngap_DataForwardingResponseDRBItem_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Ngap_DataForwardingResponseDRBItem, dRB_ID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_DRB_ID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dRB-ID"
		},
	{ ATF_POINTER, 3, offsetof(struct Ngap_DataForwardingResponseDRBItem, dLForwardingUP_TNLInformation),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_Ngap_UPTransportLayerInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dLForwardingUP-TNLInformation"
		},
	{ ATF_POINTER, 2, offsetof(struct Ngap_DataForwardingResponseDRBItem, uLForwardingUP_TNLInformation),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_Ngap_UPTransportLayerInformation,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"uLForwardingUP-TNLInformation"
		},
	{ ATF_POINTER, 1, offsetof(struct Ngap_DataForwardingResponseDRBItem, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ngap_ProtocolExtensionContainer_175P33,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_Ngap_DataForwardingResponseDRBItem_oms_1[] = { 1, 2, 3 };
static const ber_tlv_tag_t asn_DEF_Ngap_DataForwardingResponseDRBItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Ngap_DataForwardingResponseDRBItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* dRB-ID */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* dLForwardingUP-TNLInformation */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* uLForwardingUP-TNLInformation */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_Ngap_DataForwardingResponseDRBItem_specs_1 = {
	sizeof(struct Ngap_DataForwardingResponseDRBItem),
	offsetof(struct Ngap_DataForwardingResponseDRBItem, _asn_ctx),
	asn_MAP_Ngap_DataForwardingResponseDRBItem_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_Ngap_DataForwardingResponseDRBItem_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Ngap_DataForwardingResponseDRBItem = {
	"DataForwardingResponseDRBItem",
	"DataForwardingResponseDRBItem",
	&asn_OP_SEQUENCE,
	asn_DEF_Ngap_DataForwardingResponseDRBItem_tags_1,
	sizeof(asn_DEF_Ngap_DataForwardingResponseDRBItem_tags_1)
		/sizeof(asn_DEF_Ngap_DataForwardingResponseDRBItem_tags_1[0]), /* 1 */
	asn_DEF_Ngap_DataForwardingResponseDRBItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_Ngap_DataForwardingResponseDRBItem_tags_1)
		/sizeof(asn_DEF_Ngap_DataForwardingResponseDRBItem_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Ngap_DataForwardingResponseDRBItem_1,
	4,	/* Elements count */
	&asn_SPC_Ngap_DataForwardingResponseDRBItem_specs_1	/* Additional specs */
};

