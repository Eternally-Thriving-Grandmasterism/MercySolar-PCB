° MercySolar PCB Technical Specification
° v2.2 — Jan 18 2026

° Electrical
• Input: 0-150V DC, 30A max (450W @ 15V panel)
• Output: 5V/5A PD to Pi — buck efficiency >98%
• Battery: 12V LiFePO4 direct charge
• MPPT algorithm: hybrid swarm + fuzzy refinement

° Security — Post-Quantum Deepened
• Hybrid PQ-classic crypto
• Key encapsulation: Kyber-768 (NIST PQC Round 3)
• Digital signatures: Dilithium-3 (NIST PQC standardized)
• Fallback: ChaCha20-Poly1305 + scrypt passphrase
• Enclave/TPM sealed PQ keys — zero plaintext exposure
• Forward secrecy — ephemeral Kyber per session
• Overhead: <5% CPU, <100µs key exchange

° Components
• MCU: ESP32-S3-WROOM-1 (16MB flash)
• Power stage: 2× IRFZ44N synchronous buck
• Inductor: 33 µH shielded — 42A saturation
• Current sense: INA226 — I2C to ESP32
• Temperature: NTC thermistor — auto-derate >70°C

° Physical
• Board size: 100×60 mm — 2-layer FR-4
• Mounting: 4× M3 holes — Pi HAT compatible standoffs
• Connectors: XT60 input, Anderson Powerpole battery, USB-C Pi output

° Open-Source
• KiCad 8 project — full schematics/PCB/layout
• BOM: < $35 in 1000-unit
• License: CERN-OHL-W v2 — weak copyleft for hardware
