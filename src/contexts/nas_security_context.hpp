#ifndef _NAS_SECURITY_CONTEXT_H_
#define _NAS_SECURITY_CONTEXT_H_

#include <stdint.h>

#define AUTH_KNAS_INT_SIZE  16 /* NAS integrity key     */
#define AUTH_KNAS_ENC_SIZE  16 /* NAS cyphering key     */

#define NGKSI_MAX_VALUE 6

/* Type of security context */
typedef enum {
  SECURITY_CTX_TYPE_NOT_AVAILABLE = 0,
  SECURITY_CTX_TYPE_PARTIAL_NATIVE,
  SECURITY_CTX_TYPE_FULL_NATIVE,
  SECURITY_CTX_TYPE_MAPPED        
} nas_sc_type_t;

/*
   Internal data used for security mode control procedure
*/
typedef struct {
  unsigned int                            ue_id; /* UE identifier                         */
#define SECURITY_COUNTER_MAX    5
  unsigned int                            retransmission_count; /* Retransmission counter    */
  int                                     ksi;  /* NAS key set identifier                */
  int                                     nea;  /* Replayed EPS encryption algorithms    */
  int                                     eea;  /* Replayed EPS encryption algorithms    */
  int                                     nia;  /* Replayed EPS integrity algorithms     */
  int                                     eia;  /* Replayed EPS integrity algorithms     */
  int                                     ucs2; /* Replayed Alphabet                     */
  int                                     uea;  /* Replayed UMTS encryption algorithms   */
  int                                     uia;  /* Replayed UMTS integrity algorithms    */
  int                                     gea;  /* Replayed G encryption algorithms      */
  bool                                    umts_present;
  bool                                    gprs_present;
  int                                     selected_eea; /* Selected EPS encryption algorithms    */
  int                                     selected_eia; /* Selected EPS integrity algorithms     */
  int                                     saved_selected_eea; /* Previous selected EPS encryption algorithms    */
  int                                     saved_selected_nea; /* Previous selected EPS encryption algorithms    */
  int                                     saved_selected_eia; /* Previous selected EPS integrity algorithms     */
  int                                     saved_selected_nia; /* Previous selected EPS integrity algorithms     */
  int                                     saved_ngksi; /* Previous ksi     */
  uint16_t                                saved_overflow; /* Previous dl_count overflow     */
  uint8_t                                 saved_seq_num; /* Previous dl_count seq_num     */
  nas_sc_type_t                           saved_sc_type;
  bool                                    notify_failure;       /* Indicates whether the identification
                                                                 * procedure failure shall be notified
                                                                 * to the ongoing EMM procedure */
} security_data_t;


typedef uint8_t ngksi_t;

typedef struct{
  uint32_t spare:8;
  uint32_t overflow:16;
  uint32_t seq_num:8;
}count_t;

typedef struct{
  uint8_t _5gs_encryption;
  uint8_t _5gs_integrity;
}capability_t;

typedef struct{
  uint8_t encryption:4;
  uint8_t integrity:4;
}selected_algs;

class nas_secu_ctx{
public:
  int           vector_pointer;
  nas_sc_type_t sc_type;
  ngksi_t       ngksi;
  uint8_t       knas_enc[AUTH_KNAS_ENC_SIZE];
  uint8_t       knas_int[AUTH_KNAS_INT_SIZE];
  count_t       dl_count;
  count_t       ul_count;
  capability_t  ue_algorithms;
  selected_algs nas_algs; 
};













#endif
