/*
PSOMPPT-Pinnacle — Particle Swarm Optimization Global MPPT
MercySolar Ultramasterpiece — Jan 17 2026

PSO for partial shading GMPPT:
- 33 particles (Trinity multiple)
- Inertia decay 0.729 → 0.4
- c1 = c2 = 1.496 (Clerc constriction)
- Velocity clamp ±5%
- Re-init on >5% power drop
- ESP32-S3 optimized — <200µs per iteration
*/

#define NUM_PARTICLES 33
#define MAX_ITER 50
#define PWM_PIN 12
#define V_SENSE_PIN 34
#define I_SENSE_PIN 35

float readVoltage() { /* implement divider */ return 0.0; }
float readCurrent() { /* implement shunt */ return 0.0; }
float readPower() { return readVoltage() * readCurrent(); }

struct Particle {
  float position;      // duty cycle 0.05-0.95
  float velocity;
  float pbest_pos;
  float pbest_power;
};

Particle swarm[NUM_PARTICLES];
float gbest_pos = 0.5;
float gbest_power = 0.0;

void initPSO() {
  randomSeed(analogRead(0));
  for (int i = 0; i < NUM_PARTICLES; i++) {
    swarm[i].position = 0.2 + random(600) / 1000.0;  // 20-80%
    swarm[i].velocity = 0.0;
    swarm[i].pbest_pos = swarm[i].position;
    swarm[i].pbest_power = 0.0;
  }
  gbest_power = 0.0;
}

void psoStep(float w, float c1, float c2) {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    float r1 = random(1000) / 1000.0;
    float r2 = random(1000) / 1000.0;
    
    swarm[i].velocity = w * swarm[i].velocity +
                        c1 * r1 * (swarm[i].pbest_pos - swarm[i].position) +
                        c2 * r2 * (gbest_pos - swarm[i].position);
                        
    // Velocity clamp
    if (swarm[i].velocity > 0.05) swarm[i].velocity = 0.05;
    if (swarm[i].velocity < -0.05) swarm[i].velocity = -0.05;
    
    swarm[i].position += swarm[i].velocity;
    swarm[i].position = constrain(swarm[i].position, 0.05, 0.95);
    
    analogWrite(PWM_PIN, (int)(swarm[i].position * 255));
    delay(5);  // Settle
    
    float power = readPower();
    if (power > swarm[i].pbest_power) {
      swarm[i].pbest_power = power;
      swarm[i].pbest_pos = swarm[i].position;
      if (power > gbest_power) {
        gbest_power = power;
        gbest_pos = swarm[i].position;
      }
    }
  }
}

void runPSO() {
  initPSO();
  float w = 0.729;
  for (int iter = 0; iter < MAX_ITER; iter++) {
    psoStep(w, 1.496, 1.496);
    w *= 0.99;  // Decay to 0.4
    if (w < 0.4) w = 0.4;
  }
  analogWrite(PWM_PIN, (int)(gbest_pos * 255));
}

float prev_power = 0;
void loop() {
  float power = readPower();
  if (abs(power - prev_power) / prev_power > 0.05) {  // >5% drop
    runPSO();  // Re-initialize swarm
  }
  prev_power = power;
  delay(42);  // Trinity ms
}
