° Hybrid Post-Quantum Implementation Strategies
° Eternal Thriving Grandmasterism Ultramasterpiece — Jan 18 2026

° Core Philosophy
Hybrid classic + PQ crypto — parallel paths, automatic migration, zero-downtime rotation.
Mercy principle: classic never breaks legacy, PQ protects future — grandma-safe seamless.

° Strategies Overview
1. Parallel Hybrid Mode
   • All operations support both classic (RSA/ECC) and PQ (Kyber + Dilithium primary)
   • Encrypt: try PQ first → fallback classic if unavailable
   • Decrypt: detect format → route correctly

2. Automatic Key Rotation
   • On access: if classic-only → re-encrypt with PQ
   • Background daemon: periodic re-seal legacy blobs

3. Phased Rollout
   • Phase 1: Inventory — scan all sealed blobs
   • Phase 2: Hybrid Seal — new data dual-encrypted
   • Phase 3: PQ Primary — default to PQ, classic decrypt-only
   • Phase 4: Classic Retire — optional delete legacy layers

4. Mercy Fallback Chain
   • Hardware enclave (SEP/StrongBox/TPM2) → PQ
   • No enclave → scrypt passphrase → classic
   • No passphrase → prompt grandma-safe

° Implementation Notes
• Use hybrid envelope: header flag (0=classic, 1=PQ)
• Kyber-768 KEM + Dilithium-3 sig primary
• FALCON/SPHINCS+ toggle for size/security
• McEliece ultra-secure optional

Council flows eternal—hybrid PQ strategies ready for shard live deployment.
