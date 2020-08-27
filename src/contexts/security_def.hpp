#ifndef _SECURITY_DEF_H_
#define _SECURITY_DEF_H_


#define AUTH_SQN_INDEX  0
#define AUTH_AMF_INDEX  (AUTH_SQN_INDEX + AUTH_SQN_SIZE)
#define AUTH_MAC_INDEX  (AUTH_AMF_INDEX + AUTH_AMF_SIZE)

/*
 * Size of the authentication challenge parameters in bytes
 */
#define AUTH_SQN_SIZE   6  /* Sequence number:          48 bits  */
#define AUTH_AK_SIZE    6  /* Anonymity key:            48 bits  */
#define AUTH_AMF_SIZE   2  /* Authentication Management Field:  16 bits  */
#define AUTH_MAC_SIZE   8  /* Message Authentication Code:  64 bits  */
#define AUTH_AUTN_SIZE  16 /* Authentication token:     128 bits
                AUTN = (SQN ⊕ AK) || AMF || MAC        */
#define AUTH_MACS_SIZE  8  /* Re-synchronization MAC:       64 bits  */
#define AUTH_AUTS_SIZE  16 /* Re-synchronization AUT:       128 bits */
#define AUTH_RAND_SIZE  16 /* Random challenge:         128 bits     */
#define AUTH_CK_SIZE    16 /* Ciphering key:            128 bits     */
#define AUTH_IK_SIZE    16 /* Integrity key:            128 bits     */
#define AUTH_RES_SIZE   16 /* Authentication response:      128 bits */
#define AUTH_SNID_SIZE  3  /* Serving network's identity:   24 bits  */
#define AUTH_KASME_SIZE 32 /* KASME security key:        256 bits    */
#define AUTH_KNAS_INT_SIZE  16 /* NAS integrity key     */
#define AUTH_KNAS_ENC_SIZE  16 /* NAS cyphering key     */
#define AUTH_KENB_SIZE      AUTH_KASME_SIZE /* eNodeB security key   */

/* "Separation bit" of AMF field */
#define AUTH_AMF_SEPARATION_BIT(a)  ((a) & 0x80)

/*
 * 5GS authentication vector
 */
typedef struct {
  /* ASME security key                */
  uint8_t kasme[AUTH_KASME_SIZE];
  /* Random challenge parameter           */
  uint8_t rand[AUTH_RAND_SIZE];
  /* Authentication token parameter       */
  uint8_t autn[AUTH_AUTN_SIZE];
  /* Expected Authentication response parameter   */
#define AUTH_XRES_SIZE  AUTH_RES_SIZE
  uint8_t xres_size;
  uint8_t xres[AUTH_XRES_SIZE];
} auth_vector_t;

typedef struct security_context_s {
  //emm_sc_type_t sc_type;     /* Type of security context        */
                      /* state of security context is implicit due to its storage location (current/non-current)*/
#define EKSI_MAX_VALUE 6
  //ksi_t eksi;           /* NAS key set identifier for E-UTRAN      */
#define EMM_SECURITY_VECTOR_INDEX_INVALID (-1)
  int vector_index;   /* Pointer on vector */
  uint8_t knas_enc[AUTH_KNAS_ENC_SIZE];/* NAS cyphering key               */
  uint8_t knas_int[AUTH_KNAS_INT_SIZE];/* NAS integrity key               */

  struct count_s{
    uint32_t spare:8;
    uint32_t overflow:16;
    uint32_t seq_num:8;
  } dl_count, ul_count;   /* Downlink and uplink count parameters    */
  struct {
    uint8_t eps_encryption;   /* algorithm used for ciphering            */
    uint8_t eps_integrity;    /* algorithm used for integrity protection */
    uint8_t umts_encryption;  /* algorithm used for ciphering            */
    uint8_t umts_integrity;   /* algorithm used for integrity protection */
    uint8_t gprs_encryption;  /* algorithm used for ciphering            */
    bool    umts_present:1;
    bool    gprs_present:1;
  } capability;       /* UE network capability           */
  struct {
    uint8_t encryption:4;   /* algorithm used for ciphering           */
    uint8_t integrity:4;    /* algorithm used for integrity protection */
  } selected_algorithms;       /* MME selected algorithms                */

  // Requirement MME24.301R10_4.4.4.3_2 (DETACH REQUEST (if sent before security has been activated);)
  uint8_t   activated;
} security_context_t;













#endif
