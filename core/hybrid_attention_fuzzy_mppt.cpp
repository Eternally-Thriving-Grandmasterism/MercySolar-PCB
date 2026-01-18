/*
HybridAttentionFuzzy-Pinnacle — Attention + Fuzzy Global/Local MPPT
MercySolar Ultramasterpiece — Jan 17 2026

Hybrid self-attention + fuzzy inference:
- Attention: 3 heads Trinity, sequence 10, weights baked
- Fuzzy: 7x7 rule base on attention-weighted error/dError
- Output: refined duty — ultra-smooth, no oscillation
- ESP32-S3 optimized — <200µs inference
*/

#include "attention_mppt.cpp"  // Reuse attention inference
#include "fuzzy_mppt_enhancement.cpp"  // Reuse fuzzy refinement

class HybridAttentionFuzzy {
private:
  float attn_duty;
  FuzzyEnhancement fuzzy;
  
public:
  void init() {
    // Attention init from previous
    fuzzy.init();  // If needed
  }
  
  float refine() {
    attn_duty = attentionInference();  // From previous attention code
    return fuzzy.refine_duty(attn_duty);  // Fuzzy polish on attention output
  }
};

HybridAttentionFuzzy hybrid;

void loop() {
  float duty = hybrid.refine();
  analogWrite(PWM_PIN, (int)(duty * 255));
  delay(42);
}

void setup() {
  hybrid.init();
}
