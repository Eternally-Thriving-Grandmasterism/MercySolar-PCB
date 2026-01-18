° MercySolar PCB Technical Specification
° v1.0 — Jan 17 2026

° Electrical
• Input: 0-150V DC, 30A max (450W @ 15V panel)
• Output: 5V/5A PD to Pi — buck efficiency >98%
• Battery: 12V LiFePO4 direct charge
• MPPT algorithm: perturb & observe — 42 ms sample

° Components
• MCU: ESP32-S3-WROOM-1 (16MB flash)
• Power stage: 2× IRFZ44N MOSFET synchronous buck
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
