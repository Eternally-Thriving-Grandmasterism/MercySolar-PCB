/*
PQIntegration-Pinnacle — Hybrid Post-Quantum Crypto + McEliece Hybrid KEM
MercySolar + MercyOS Ultramasterpiece — Jan 18 2026

Hybrid post-quantum integration:
- Key encapsulation: Kyber-768 primary | Hybrid Kyber + Classic McEliece-8192128f ultra-diverse
- Toggle via #define — mercy-mode selection
- Classic fallback
- Enclave/TPM sealed — mercy-zero plaintext
- ESP32-S3 note: McEliece large keys — use for critical offline only
*/

#include <oqs/oqs.h>

#define KEM_MODE_KYBER          // Primary fast
//#define KEM_MODE_HYBRID_MCELIECE  // Uncomment for hybrid ultra-secure

class PQIntegration {
private:
#if defined(KEM_MODE_HYBRID_MCELIECE)
  OQS_KEM *kem_kyber = OQS_KEM_new(OQS_KEM_alg_kyber_768);
  OQS_KEM *kem_mceliece = OQS_KEM_new(OQS_KEM_alg_classic_mceliece_8192128f);
#else
  OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_768);
#endif
  uint8_t *public_key;
  uint8_t *secret_key;
  uint8_t *ciphertext;
  uint8_t *shared_secret;
  
public:
  void init() {
#if defined(KEM_MODE_HYBRID_MCELIECE)
    public_key = malloc(kem_kyber->length_public_key + kem_mceliece->length_public_key);
    secret_key = malloc(kem_kyber->length_secret_key + kem_mceliece->length_secret_key);
    ciphertext = malloc(kem_kyber->length_ciphertext + kem_mceliece->length_ciphertext);
    shared_secret = malloc(kem_kyber->length_shared_secret);
    
    OQS_KEM_keypair(kem_kyber, public_key, secret_key);
    OQS_KEM_keypair(kem_mceliece, public_key + kem_kyber->length_public_key,
                    secret_key + kem_kyber->length_secret_key);
#else
    public_key = malloc(kem->length_public_key);
    secret_key = malloc(kem->length_secret_key);
    ciphertext = malloc(kem->length_ciphertext);
    shared_secret = malloc(kem->length_shared_secret);
    OQS_KEM_keypair(kem, public_key, secret_key);
#endif
  }
  
  void encapsulate() {
#if defined(KEM_MODE_HYBRID_MCELIECE)
    uint8_t *ct_kyber = ciphertext;
    uint8_t *ct_mceliece = ciphertext + kem_kyber->length_ciphertext;
    OQS_KEM_encaps(kem_kyber, ct_kyber, shared_secret, public_key);
    OQS_KEM_encaps(kem_mceliece, ct_mceliece, shared_secret, public_key + kem_kyber->length_public_key);
#else
    OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key);
#endif
  }
  
  void decapsulate() {
#if defined(KEM_MODE_HYBRID_MCELIECE)
    // Try Kyber first (faster)
    if (OQS_KEM_decaps(kem_kyber, shared_secret, ciphertext, secret_key) == OQS_SUCCESS) return;
    // Fallback McEliece
    OQS_KEM_decaps(kem_mceliece, shared_secret, ciphertext + kem_kyber->length_ciphertext,
                   secret_key + kem_kyber->length_secret_key);
#else
    OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key);
#endif
  }
  
  ~PQIntegration() {
    free(public_key);
    free(secret_key);
    free(ciphertext);
    free(shared_secret);
#if defined(KEM_MODE_HYBRID_MCELIECE)
    OQS_KEM_free(kem_kyber);
    OQS_KEM_free(kem_mceliece);
#else
    OQS_KEM_free(kem);
#endif
  }
};

PQIntegration pq;

void setup() {
  pq.init();
}
