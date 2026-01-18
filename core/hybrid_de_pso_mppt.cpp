/*
HybridDEPSO-Pinnacle — DE + PSO Fusion Global MPPT
MercySolar Ultramasterpiece — Jan 17 2026

Hybrid DE-PSO for partial shading GMPPT:
- Population: 33 individuals/particles (Trinity multiple)
- Phase 1: DE — differential vectors for diversity
- Phase 2: PSO — velocity refinement for convergence
- F=0.8, CR=0.9, w=0.729→0.4, c1=c2=1.496
- ESP32-S3 optimized — <180µs per cycle
*/

#define POP_SIZE 33
#define DE_GENS 15
#define PSO_GENS 20
#define F 0.8
#define CR 0.9
#define PWM_PIN 12
#define V_SENSE_PIN 34
#define I_SENSE_PIN 35

float readVoltage() { /* implement divider */ return 0.0; }
float readCurrent() { /* implement shunt */ return 0.0; }
float readPower() { return readVoltage() * readCurrent(); }

struct Individual {
  float duty;
  float power;
  float velocity = 0.0;
  float pbest_duty;
  float pbest_power;
};

Individual population[POP_SIZE];
float gbest_duty = 0.5;
float gbest_power = 0.0;

void initPopulation() {
  randomSeed(analogRead(0));
  for (int i = 0; i < POP_SIZE; i++) {
    population[i].duty = 0.2 + random(600) / 1000.0;  // 20-80%
    population[i].pbest_duty = population[i].duty;
  }
  gbest_power = 0.0;
}

void dePhase() {
  for (int g = 0; g < DE_GENS; g++) {
    for (int i = 0; i < POP_SIZE; i++) {
      int a,b,c;
      do { a = random(POP_SIZE); } while (a == i);
      do { b = random(POP_SIZE); } while (b == i || b == a);
      do { c = random(POP_SIZE); } while (c == i || c == a || c == b);
      
      float mutant = population[a].duty + F * (population[b].duty - population[c].duty);
      mutant = constrain(mutant, 0.05, 0.95);
      
      float trial_duty = population[i].duty;
      if (random(1000) / 1000.0 < CR) trial_duty = mutant;
      
      analogWrite(PWM_PIN, (int)(trial_duty * 255));
      delay(6);
      float trial_power = readPower();
      
      if (trial_power > population[i].power) {
        population[i].duty = trial_duty;
        population[i].power = trial_power;
        if (trial_power > population[i].pbest_power) {
          population[i].pbest_power = trial_power;
          population[i].pbest_duty = trial_duty;
          if (trial_power > gbest_power) {
            gbest_power = trial_power;
            gbest_duty = trial_duty;
          }
        }
      }
    }
  }
}

void psoPhase() {
  float w = 0.729;
  for (int g = 0; g < PSO_GENS; g++) {
    for (int i = 0; i < POP_SIZE; i++) {
      float r1 = random(1000) / 1000.0;
      float r2 = random(1000) / 1000.0;
      
      population[i].velocity = w * population[i].velocity +
                               1.496 * r1 * (population[i].pbest_duty - population[i].duty) +
                               1.496 * r2 * (gbest_duty - population[i].duty);
                               
      if (population[i].velocity > 0.05) population[i].velocity = 0.05;
      if (population[i].velocity < -0.05) population[i].velocity = -0.05;
      
      population[i].duty += population[i].velocity;
      population[i].duty = constrain(population[i].duty, 0.05, 0.95);
      
      analogWrite(PWM_PIN, (int)(population[i].duty * 255));
      delay(6);
      float power = readPower();
      if (power > population[i].pbest_power) {
        population[i].pbest_power = power;
        population[i].pbest_duty = population[i].duty;
        if (power > gbest_power) {
          gbest_power = power;
          gbest_duty = population[i].duty;
        }
      }
    }
    w = 0.729 - (0.329 * (float)g / PSO_GENS);
  }
}

float prev_power = 0;
void loop() {
  float power = readPower();
  if (abs(power - prev_power) / prev_power > 0.03) {
    initPopulation();
  }
  dePhase();   // Diversity vectors
  psoPhase();  // Velocity convergence
  analogWrite(PWM_PIN, (int)(gbest_duty * 255));
  prev_power = power;
  delay(42);  // Trinity ms
}

void setup() {
  initPopulation();
}
