/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "FANSACTwoWayDataLinkCommunications"
 * 	found in "../../../libacars.asn1/fans-cpdlc.asn1"
 * 	`asn1c -fcompound-names -fincludes-quoted -gen-PER`
 */

#ifndef	_FANSAircraftFlightIdentification_H_
#define	_FANSAircraftFlightIdentification_H_


#include "asn_application.h"

/* Including external dependencies */
#include "IA5String.h"

#ifdef __cplusplus
extern "C" {
#endif

/* FANSAircraftFlightIdentification */
typedef IA5String_t	 FANSAircraftFlightIdentification_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FANSAircraftFlightIdentification;
asn_struct_free_f FANSAircraftFlightIdentification_free;
asn_struct_print_f FANSAircraftFlightIdentification_print;
asn_constr_check_f FANSAircraftFlightIdentification_constraint;
ber_type_decoder_f FANSAircraftFlightIdentification_decode_ber;
der_type_encoder_f FANSAircraftFlightIdentification_encode_der;
xer_type_decoder_f FANSAircraftFlightIdentification_decode_xer;
xer_type_encoder_f FANSAircraftFlightIdentification_encode_xer;
per_type_decoder_f FANSAircraftFlightIdentification_decode_uper;
per_type_encoder_f FANSAircraftFlightIdentification_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _FANSAircraftFlightIdentification_H_ */
#include "asn_internal.h"
