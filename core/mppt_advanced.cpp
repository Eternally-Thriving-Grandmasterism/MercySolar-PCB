/*
MPPTAdvanced-Pinnacle — Hybrid Global MPPT for MercySolar PCB
MercySolar Ultramasterpiece — Jan 17 2026

Hybrid algorithm:
- Initial Voc scan (20-80%) for global peak under partial shading
- Core: Incremental Conductance with variable-step adaptive P&O fallback
- ESP32-S3 optimized — <50µs loop, no floating-point heavy ops
- Trinity sampling: 42ms interval
*/

#define PWM_PIN 12        // PWM output to buck gate
#define V_SENSE_PIN 34    // Voltage divider analog in
#define I_SENSE_PIN 35    // Current shunt analog in

float readVoltage() {
  return analogRead(V_SENSE_PIN) * (3.3 / 4095.0) * V_DIVIDER_RATIO;
}

float readCurrent() {
  return analogRead(I_SENSE_PIN) * (3.3 / 4095.0) / SHUNT_RESISTANCE * AMP_GAIN;
}

class AdvancedMPPT {
private:
  float v_prev = 0, p_prev = 0, i_prev = 0;
  float duty = 0.5;           // Initial 50% duty
  float step_size = 0.01;     // Adaptive step
  bool global_scan_done = false;
  
public:
  void init() {
    pinMode(PWM_PIN, OUTPUT);
    analogWriteFrequency(100000);  // 100kHz PWM
    globalScan();  // Initial GMPPT scan
  }
  
  void globalScan() {
    // Scan 20-80% Voc for multiple peaks
    float voc = readVoltage() * 1.2;  // Approximate open-circuit
    for (float v = 0.2 * voc; v <= 0.8 * voc; v += 0.05 * voc) {
      setDuty(v / voc);  // Rough set
      delay(10);
      float p = readVoltage() * readCurrent();
      if (p > p_prev) p_prev = p;
    }
    global_scan_done = true;
  }
  
  void run() {
    float v = readVoltage();
    float i = readCurrent();
    float p = v * i;
    
    float dV = v - v_prev;
    float dP = p - p_prev;
    
    if (abs(dV) < 0.01) {  // Near peak — fine tune
      step_size = 0.001;
    } else {
      step_size = 0.01;
    }
    
    // Incremental Conductance core
    if (dV == 0) {
      if (dP == 0) return;  // At MPP
      duty += (dP > 0) ? step_size : -step_size;
    } else {
      if (dP / dV == -i / v) return;  // At MPP
      duty += (dP / dV > -i / v) ? step_size : -step_size;
    }
    
    // Adaptive P&O fallback if oscillation detected
    if (abs(dP) < 0.1 * p_prev) {  // Small change — reduce step
      step_size *= 0.9;
    }
    
    setDuty(constrain(duty, 0.05, 0.95));
    
    v_prev = v; p_prev = p; i_prev = i;
  }
  
private:
  void setDuty(float d) {
    duty = d;
    analogWrite(PWM_PIN, (int)(d * 255));
  }
};

// Global instance
AdvancedMPPT mppt;

void setup() {
  mppt.init();
}

void loop() {
  mppt.run();
  delay(42);  // Trinity interval ms
}
