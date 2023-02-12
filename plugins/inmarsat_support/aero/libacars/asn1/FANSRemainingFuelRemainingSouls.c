/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "FANSACTwoWayDataLinkCommunications"
 * 	found in "../../../libacars.asn1/fans-cpdlc.asn1"
 * 	`asn1c -fcompound-names -fincludes-quoted -gen-PER`
 */

#include "FANSRemainingFuelRemainingSouls.h"

static asn_TYPE_member_t asn_MBR_FANSRemainingFuelRemainingSouls_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct FANSRemainingFuelRemainingSouls, remainingFuel),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_FANSRemainingFuel,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"remainingFuel"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct FANSRemainingFuelRemainingSouls, remainingSouls),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_FANSRemainingSouls,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"remainingSouls"
		},
};
static const ber_tlv_tag_t asn_DEF_FANSRemainingFuelRemainingSouls_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_FANSRemainingFuelRemainingSouls_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, 0, 0 }, /* remainingSouls */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 0, 0, 0 } /* remainingFuel */
};
static asn_SEQUENCE_specifics_t asn_SPC_FANSRemainingFuelRemainingSouls_specs_1 = {
	sizeof(struct FANSRemainingFuelRemainingSouls),
	offsetof(struct FANSRemainingFuelRemainingSouls, _asn_ctx),
	asn_MAP_FANSRemainingFuelRemainingSouls_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_FANSRemainingFuelRemainingSouls = {
	"FANSRemainingFuelRemainingSouls",
	"FANSRemainingFuelRemainingSouls",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_FANSRemainingFuelRemainingSouls_tags_1,
	sizeof(asn_DEF_FANSRemainingFuelRemainingSouls_tags_1)
		/sizeof(asn_DEF_FANSRemainingFuelRemainingSouls_tags_1[0]), /* 1 */
	asn_DEF_FANSRemainingFuelRemainingSouls_tags_1,	/* Same as above */
	sizeof(asn_DEF_FANSRemainingFuelRemainingSouls_tags_1)
		/sizeof(asn_DEF_FANSRemainingFuelRemainingSouls_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_FANSRemainingFuelRemainingSouls_1,
	2,	/* Elements count */
	&asn_SPC_FANSRemainingFuelRemainingSouls_specs_1	/* Additional specs */
};

