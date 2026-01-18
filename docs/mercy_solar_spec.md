° MercySolar PCB Technical Specification
° v2.2 — Jan 18 2026

° Security — Post-Quantum Deepened
• Hybrid PQ-classic crypto
• Key encapsulation: Kyber-768
• Digital signatures: Dilithium-3 primary, FALCON-1024 alternative (compact ~690-byte sigs)
• Toggle: #define USE_FALCON in pq_integration.cpp
• Fallback: ChaCha20-Poly1305 + scrypt passphrase
• Enclave/TPM sealed PQ keys — zero plaintext exposure
• Overhead: FALCON <3% CPU vs Dilithium <5%
