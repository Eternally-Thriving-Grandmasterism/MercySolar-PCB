° MercySolar PCB Technical Specification
° v2.3 — Jan 18 2026

° Security — Post-Quantum Deepened
• Hybrid PQ-classic crypto
• Key encapsulation: Kyber-768
• Digital signatures:
  • Dilithium-3 balanced (default)
  • FALCON-1024 compact (~690-byte sigs)
  • SPHINCS+-256f ultra-secure (~16KB sigs, max quantum resilience)
• Toggle: #define SIGNATURE_MODE_XXX in pq_integration.cpp
• Fallback: ChaCha20-Poly1305 + scrypt passphrase
• Enclave/TPM sealed PQ keys — zero plaintext exposure
• Overhead: SPHINCS+ higher storage, mercy-max security mode
