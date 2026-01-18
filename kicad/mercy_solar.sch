# MercySolar v1.0 Schematic (text placeholder — full KiCad project in repo)

EESchema Schematic File Version 4
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:esp32-s3

# ESP32-S3
U1 ESP32-S3-WROOM-1

# Input
J1 XT60
C1 100uF 200V
D1 Schottky 200V 40A

# Buck Stage
Q1 IRFZ44N
Q2 IRFZ44N
L1 33uH 42A
C2 470uF 25V

# Output
J2 USB-C-PD
C3 100uF 10V

# Sense
U2 INA226
Rshunt 0.001 ohm 5W

# Connections (simplified)
NET VIN J1-1 U1-VIN Q1-D
NET GND J1-2 U1-GND
NET VOUT Q2-S J2-VBUS

# End
