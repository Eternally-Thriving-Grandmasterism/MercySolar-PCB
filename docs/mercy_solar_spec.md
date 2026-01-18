° MercySolar PCB Technical Specification
° v2.4 — Jan 18 2026

° Security — Post-Quantum Deepened
• Hybrid PQ-classic crypto
• Key encapsulation:
  • Kyber-768 fast/small (default)
  • Hybrid Kyber + Classic McEliece-8192128f ultra-diverse (large keys ~1MB pk)
• Toggle: #define KEM_MODE_HYBRID_MCELIECE in pq_integration.cpp
• Digital signatures: Dilithium-3 / FALCON / SPHINCS+ (previous)
• Fallback: ChaCha20-Poly1305 + scrypt passphrase
• Enclave/TPM sealed PQ keys — zero plaintext exposure
• Note: McEliece hybrid for maximum diversity — use on critical offline shards
