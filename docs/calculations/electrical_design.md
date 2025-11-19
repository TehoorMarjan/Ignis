# Electrical Calculations - Ignis Project

## Battery and Consumption Calculations

### EEMB LP402535 Battery (320 mAh)

- Nominal capacity: 320 mAh
- Nominal voltage: 3.7 V
- Internal resistance: 160 mΩ
- Charging:
  - Maximum current: 320 mA (1C)
  - Maximum voltage: 4.2 V
  - End-of-charge current: 6.4 mA
- Discharge:
  - Maximum current: 2C = 640 mA
  - Minimum voltage: 2.75 V
- Operating temperature:
  - Discharge: -20 °C to +60 °C
  - Charge: 0 °C to +45 °C

### Current Consumption per Component

| Component          | Current (mA) | Notes                        |
| ------------------ | ------------ | ---------------------------- |
| ATtiny212 (active) | 3–5          | @ 20 MHz, 5 V                |
| ATtiny212 (sleep)  | 0.1–1        | Power-down mode              |
| WS2812B (per LED)  | 0.3–60       | 0.3 mA off, 60 mA full white |
| MCP73871 (standby) | 18 µA        | Very low in standby          |
| BQ51013B (standby) | 30 µA        | Inactive Qi receiver         |
| FP6277 (standby)   | 50–100 µA    | Very low in standby          |

### Usage Scenarios

#### Scenario 1: 6 LEDs, 50% brightness

```
LEDs: 6 × 30 mA = 180 mA
MCU: 5 mA
Boost: 2 mA (estimated losses)
TOTAL: ~187 mA

Runtime: 320 mAh / 187 mA = 1.7 hours
```

#### Scenario 2: 8 LEDs, 75% brightness

```
LEDs: 8 × 45 mA = 360 mA
MCU: 5 mA
Boost: 3 mA
TOTAL: ~368 mA

Runtime: 320 mAh / 368 mA = 0.87 hours
⚠️ Exceeds recommended 1C, OK for occasional use
```

#### Scenario 3: Standby mode (patterns off)

```
MCU (sleep): 0.5 mA
LEDs (off): 8 × 0.3 = 2.4 mA
Boost (standby): 0.1 mA
TOTAL: ~3 mA

Runtime: 320 mAh / 3 mA = 106 hours (4.4 days)
```

## Qi Receiver - BQ51013B

### General Characteristics

- **Type**: Qi receiver WPC v1.2 (BPP - Baseline Power Profile)
- **Power**: Up to 5 W
- **Output voltage**: Regulated 5 V
- **Max current**: 1 A
- **Frequency**: 100–250 kHz (WPC band)
- **Communication**: ASK/FSK with transmitter
- **FOD**: Built-in Foreign Object Detection

### Receiver Coil - IWAS3010AZEB130KF1 (Vishay)

**Specifications**:

- Dimensions: 30 mm × 10 mm (rectangular shape)
- Inductance: 12.9 µH @ 100 kHz
- Quality factor Q: 11
- DC resistance: 780 mΩ
- Max power: 3.5 W
- Max current: 800 mA
- Qi compliance: WPC v1.2

### Resonant Capacitor Calculations

The BQ51013B requires two resonant capacitors:

**C1 (series)** – Main resonant capacitor:
$$C_1 = \frac{1}{(2\pi \times f_S)^2 \times L_S^\prime}$$

With $L_S^\prime \approx 15\,\mu H$ in our case (to be confirmed) and
$f_S = 100\,\text{kHz}$ (from the BQ51013B documentation):

$$C_1 \approx 168{,}868\,\text{nF}$$

We can choose $C_1 = 150\,\text{nF}$ or
$C_1 = 150\,\text{nF} + 15\,\text{nF} = 165\,\text{nF}$. At this stage this is
an implementation detail. To some extent, the 2-capacitor version can offer
better tunability, especially since $L_S^\prime$ is not fully known at this
point. The frequency error would then be $\leq 2.7\%$.

**C2 (parallel)** – Fine-tuning capacitor:

With $L_S = 12.9\,\mu H$ (unloaded inductance, from the coil datasheet) and
$f_D = 1\,\text{MHz}$ (from the BQ51013B datasheet):

$$C_2 = \frac{1}{(2\pi \times f_D)^2 \times L_S - \frac{1}{C_1}}$$

which gives $C_2 \approx 1.987\,\text{nF} \text{ to } 1.989\,\text{nF}$
depending on whether $C_1 = 165\,\text{nF}$ or $C_1 = 150\,\text{nF}$ is chosen,
i.e. practically no difference. We can therefore choose $C_2 = 2\,\text{nF}$ or
$C_2 = 1\,\text{nF} + 1\,\text{nF}$.

### R_ILIM Calculation

The current limit of the BQ51013B is set via resistor $R_{ILIM}$ according to:

$$
R_{ILIM} = \frac{K_{ILIM}}{I_{MAX}}
$$

where $K_{ILIM}$ is given in the datasheet ($614\,\Omega\text{A}$), and
$I_{MAX}$ is the desired maximum current.

The datasheet specifies that the actual limit current is:

$$
I_{ILIM} = 1.2 \times I_{MAX} = \frac{K_{ILIM}}{R_{ILIM}}
$$

$R_{ILIM}$ is the sum of two resistors:

$$
R_{ILIM} = R_1 + R_{FOD}
$$

with $R_{FOD}$ by default at $196\,\Omega$.

For $I_{MAX} \geq 600\,\text{mA}$, $I_{ILIM} = 720\,\text{mA}$

$$
R_{ILIM} = \frac{614}{0.72} \approx 436\,\Omega \implies R_1 = 240\,\Omega
$$

In practice, with $R_{FOD} = 196\,\Omega$, we choose $R_1 = 220\,\Omega$
(standard value):

$$
R_{ILIM} = 220 + 196 = 416\,\Omega \implies I_{ILIM} \approx 754\,\text{mA} \implies I_{MAX} \approx 629\,\text{mA}
$$

### BQ51013B Components

- C1a (series): 150 nF, 25 V, X7R
- C1b (series): 15 nF, 25 V, X7R
- C2a (parallel): 1 nF, 25 V, X7R
- C2b (parallel): 1 nF, 25 V, X7R
- C3 (RECT): 10 µF + 10 µF + 0.1 µF, 16 V
- C4 (OUT): 10 µF + 0.1 µF, 16 V
- C5 (BOOT): 10 nF, 25 V
- C6 (CLAMP): 470 nF, 25 V
- C7 (COMM): 47 nF, 25 V
- R1 (ILIM): 220 Ω
- Rfod (FOD): 196 Ω
- Ros (RECT): 20 kΩ

### Pin Configuration

- **AC1/AC2**: Receiver coil connection
- **RECT**: Rectified voltage (~8–10 V)
- **OUT**: Regulated 5 V output
- **AD**: Tied to GND via 1 µF (no dual-power management)
- **EN**: Enable, 10 kΩ pull-up to OUT
- **TS/CTRL**: Configuration, see datasheet

## Battery Charger - BQ24073

The BQ24073 from Texas Instruments is a switching Li-ion/Li-Po charger with
integrated power-path management:

- High-efficiency switching charger (vs linear)
- Automatic power-path management
- Compatible with Qi receiver BQ51013B (same manufacturer)
- Built-in thermal and electrical protection
- Status indication via CHG and PGOOD pins
- Input current management via EN1/EN2

### Charge Programming

The charge current is set via resistor $R_{ISET}$ connected from the ISET pin to
VSS: $$R_{ISET} = \frac{K_{ISET}}{I_{CHARGE}}$$

$$
K_{ISET} = 890\,\Omega\text{A} \implies R_{ISET} = \frac{890}{0.32} \approx 2781\,\Omega \approx \boxed{2.7\,k\Omega}
$$

$$
R_{TMR} = \frac{t_{MAXCHG}}{10 \times K_{TMR}}\,\text{with}\, K_{TMR} = 48 \text{s}/\text{k}\Omega
$$

For a 1 C charge (320 mA), the theoretical duration is 1 h. In practice, the
charge proceeds in two phases:

- **CC phase (constant current)**: 320 mA up to around 4.2 V, i.e. 40 to 50 min
  to reach 70 to 80% of the capacity.
- **CV phase (constant voltage)**: voltage remains at 4.2 V, current gradually
  decreases down to the termination threshold (typically 0.1 C to 0.05 C),
  adding 20 to 50 min.

As a rule of thumb:

- Termination at 0.1 C (32 mA): total duration 1.2 to 1.4 h.
- Termination at 0.05 C (16 mA): total duration 1.4 to 1.8 h.

A pre-charge at low current (0.05–0.1 C) can add 10 to 20 min if the cell is
very discharged.

We therefore take a typical charge time between 1.2 and 1.6 h at 1 C, depending
on termination threshold, temperature, and initial state of the battery.

We choose $t_{MAXCHG}$ as twice the expected duration to cover cases of reduced
current:

$$
t_{MAXCHG} = 2 \times 1.6\,\text{h} = 3.2\,\text{h} = 11\,520\,\text{s} \\
\implies R_{TMR} = \frac{11 520}{10 \times 48} = \boxed{24\,\text{k}\Omega}
$$

### Input Current Limitation

For this project, we use the programmable mode (EN2=1, EN1=0) to adjust the
input limit according to the Qi receiver capability. Compute $R_{ILIM}$ to set
the maximum current to ≤ 629 mA.

$$R_{ILIM} = \frac{K_{ILIM}}{I_{IN\_MAX}}$$

$$
K_{ILIM} = 1550\,\Omega\text{A} \implies R_{ILIM} = \frac{1550}{0.629} \approx 2.46\,k\Omega \approx \boxed{2.4\,k\Omega}
$$

### BQ24073 Components (to be used)

- C_IN: 1 µF, 16 V, X7R
- C_BAT: 4.7 µF, 16 V, X7R
- C_OUT: 4.7 µF, 16 V, X7R
- R_TS: 10 kΩ (fixed, replaces thermistor)
- R_ILIM: 2.4 kΩ
- R_ISET: 2.7 kΩ
- R_TMR: 24 kΩ

## FP6277 Boost Configuration

### Output Voltage Setting (5 V)

The output voltage is defined by the feedback divider:

$$
V_{out} = 0.6\,\text{V} \times \left(1 + \frac{R_1}{R_2}\right)
$$

For $V_{out} = 5\,\text{V}$:

$$
5 = 0.6 \times \left(1 + \frac{R_1}{R_2}\right) \\
\frac{R_1}{R_2} = \frac{5}{0.6} - 1 = 7.33
$$

If $R_2 = 10\,\text{k}\Omega$ then
$R_1 = 73.3\,\text{k}\Omega \approx 75\,\text{k}\Omega$

### Optimal Inductance Calculation

The optimal inductance for the FP6277 is given by:

$$
L = \frac{V_{in(max)} \times (V_{out} - V_{in(max)})}{V_{out} \times (\Delta I_L) \times f_{sw}}
$$

With:

- $V_{in(max)} = 4.2\,V$ (full battery)
- $V_{out} = 5.0\,V$
- $\Delta I_L = 500\,\text{mA} \times 30\% = 0.15\,A$ (30% ripple of average
  current)
- $f_{sw} = 1.4\,\text{MHz}$ (FP6277 frequency)

$$
L = \frac{4.2 \times (5.0 - 4.2)}{5.0 \times 0.15 \times 1.4 \times 10^6} = 3.2\,\mu H
$$

**Recommended inductance**: 3.3 µH (matches the reference design)

### Overcurrent Protection

The current limit of the FP6277 is set via a resistor between the OC pin and
ground:

$$
I_{OCP} = \frac{180\,000}{R_{OC}} + 0.2
$$

To set $I_{OCP} = 0.64\,\text{A}$ (640 mA, battery max):

$$
R_{OC} = \frac{180\,000}{0.44} = 409\,\text{k}\Omega \approx 390\,\text{k}\Omega
$$

**FP6277 Components**:

- L1: 3.3 µH inductor, saturation current > 1 A
- C1 (Vin): 22 µF, 16 V
- C2 (Vout): 22 µF, 16 V
- C3 (Vout): 100 µF, 16 V
- R1: 75 kΩ (feedback high)
- R2: 10 kΩ (feedback low)
- R3: 390 kΩ (OC)

## SW18030 Filter Calculation

The SW18030 is a shock sensor using a spring wound around a metal rod. On impact
or acceleration, the spring briefly touches the rod, generating a very short
contact. To stretch this signal and make it usable, an RC circuit is added to
extend the pulse width. To obtain a clean square signal, compatible with waking
up the ATtiny from sleep mode, a non-inverting Schmitt trigger buffer is placed
after the RC filter.

On impact, the SW18030 closes the contact for ~1 ms. To obtain a usable pulse,
the time constant of the RC filter must be much shorter than this duration.

**RC filter (fast charge):**

- Series resistor: $R_s$
- Capacitance: $C$
- Time constant: $\tau = R_s \times C$

For this project:

- $R_s = 470\,\Omega$
- $C = 22\,\text{nF}$
- $\tau = 470\,\Omega \times 22\,\text{nF} = 10{,}3\,\mu\text{s} \ll 1\,\text{ms}$

The capacitor charges almost instantly, allowing the signal to quickly reach the
upper Schmitt threshold ($V_T^+$).

**Discharge (pulse stretching):**

- Discharge resistor: $R_d$
- Pulse duration: $t = R_d \cdot C \cdot \ln\left(\frac{V_{CC}}{V_T^-}\right)$

For this project:

- Target $t \approx 15\,\text{ms}$
- $R_d = 680\,\text{k}\Omega$ (calculation gives
  $R_d \approx 667\,\text{k}\Omega$)
- $V_{CC} = 5\,\text{V}$
- $V_T^- = 1.8\,\text{V}$ (from datasheet)
- $t = 680\,\text{k}\Omega \times 22\,\text{nF} \times \ln\left(\frac{5}{1.8}\right) \approx 15.3\,\text{ms}$
  => OK

This time guarantees a pulse long enough to wake up the ATtiny in sleep mode,
without bounce-induced glitches.

**SW18030 Components**:

- Rs: 470 Ω
- Cs: 22 nF
- Rd: 680 kΩ
- 74LVC1G17: Schmitt trigger buffer (non-inverting).

## Thermal Budget

### Boost Converter Losses

```
Losses ≈ (Vout - Vin) × Iout / η

Worst case: Vin = 3.0 V, Vout = 5 V, Iout = 400 mA, η = 85%
Losses = (5 - 3) × 0.4 / 0.85 = 0.94 W
```

**Thermal management**:

- Thermal vias under FP6277
- Large ground plane
- Copper area for heat spreading

### BQ24073 Heating

Because the BQ24073 is a switching charger (vs linear), losses are reduced:

```
Losses ≈ 0.1 W @ 320 mA charge (efficiency ~90%)
ΔT ≈ 0.1 W / 8 mW/°C ≈ 12 °C temperature rise
```

### Battery Heating

LiPo internal resistance ≈ 100–200 mΩ

```
Battery losses = I² × Ri = 0.4² × 0.15 = 24 mW
ΔT ≈ 24 mW / 5 mW/°C = 5 °C temperature rise
```

## Protection and Safety

### Protection Thresholds

- **Minimum battery voltage**: 3.0 V (boost cutoff)
- **Max current**: 500 mA (software limit)
- **Max temperature**: 70 °C (monitoring if a sensor is added)
- **Qi FOD**: Automatic foreign object detection

## Brainstorming - Design Decision History

### Initial Issue: Wireless Charger

**Motivations for Qi**:

- Watertight enclosure (no USB port)
- Premium user experience
- Compatibility with commercial chargers

**Identified challenges**:

- Off-the-shelf modules too expensive (> €11)
- Excessive power (15 W vs 1.5–3 W needed)
- Coil footprint/size
- PCB integration complexity

### Receiver IC Choice

**Options evaluated**:

- **BQ51003**: 2.5 W, simpler but limited power
- **BQ51013B**: 5 W, more margin, excellent TI documentation
- **P9025AC (Renesas)**: Less public documentation

**Decision**: BQ51013B for the documentation and availability at LCSC (JLCPCB)

### Power System Evolution

**Version 1 (obsolete)**: TP4056 + DMP1045U

- Pros: Well-known components, many reference schematics
- Cons: DIY power-path, two separate ICs

**Version 2 (obsolete)**: MCP73871

- Pros: Integrated power-path, cleaner, better thermally
- Cons: Availability, cost

**Version 3 (current)**: BQ24073

- Pros: Same manufacturer as BQ51013B (TI), switching efficiency, integrated
  power-path
- Cons: More complex initial configuration

### Coil Choice

**Criteria**:

- Qi WPC v1.2 compliance
- Suitable dimensions (≤ 35 mm)
- Sufficient power rating (3 W+)
- Distributor availability

**Chosen IWAS3010AZEB130KF1**:

- 30 × 10 mm: compact for the flame enclosure
- 3.5 W: margin for consumption peaks
- Q = 11: compromise between efficiency and bandwidth
- Vishay: reliable brand, stock at Mouser/Digikey

### Power-Path Philosophy

**Two levels identified**:

1. **Upstream of the battery**: Source selection (USB/Qi/AC)
   - Not applicable here (single Qi source)
   - BQ51013B AD pin to GND
2. **Downstream of the battery**: System vs battery
   - Critical to avoid discharging the battery during charge
   - MCP73871 handles this automatically

### Trade-offs and Final Decisions

**Backfeed protection**: Not required because there are no multiple charge
sources (Qi only)

**Safety**: Prefer ICs with integrated protections (FOD, thermal shutdown)
versus external discrete circuits

**BOM**: Prefer integration vs cost (MCP73871 vs TP4056+DMP1045)

---

## Obsolete Components (Previous Design)

### MCP73871 - Charger with Integrated Power-Path

**Replaced by BQ24073**

Advantages of MCP73871:

- Li-ion/Li-Po charger with power-path management
- Automatic source selection (VIN/USB)
- Overcharge/overdischarge protection
- Status indication (LEDs)
- Thermal regulation

Power-path configuration:

- **VIN**: Main supply (BQ51013B output, 5 V)
- **VBAT**: Li-Po battery connection
- **VOUT**: System output (to FP6277 boost)
- **Power-path**: Automatic priority VIN > battery

Charge current programming: $$I_{CHARGE} = \frac{1000\text{V}}{R_{PROG}}$$

For 320 mA (optimal 1C): $R_{PROG} = 3.3\,\text{k}\Omega$

USB current limit: $$I_{USB} = \frac{515\text{V}}{R_{USB}}$$

For 500 mA max: $R_{USB} = 1\,\text{k}\Omega$

**MCP73871 Components**:

- RPROG: 3.3 kΩ (303 mA charge)
- RUSB: 1 kΩ (515 mA USB limit)
- C1 (VIN): 4.7 µF, 16 V
- C2 (VBAT): 4.7 µF, 16 V
- C3 (VOUT): 4.7 µF, 16 V

**Reasons for change**: Simpler BQ24073, better integration with Qi receiver
BQ51013B (same manufacturer)

### TP4056 - Linear Li-ion Charger

**Replaced by BQ24073**

Charge current programming via Rprog:
$$R_{PROG} = \frac{1100\,\text{V}}{I_{BAT}}$$

For 320 mA: $R_{PROG} = 3.9\,\text{k}\Omega$

**Drawbacks**:

- Linear charger → thermal dissipation
- No integrated power-path
- Requires external circuit for power switching

### DMP1045U - P-MOSFET Power-Path

**Replaced by BQ24073**

Automatic switchover circuit between USB and battery:

- Gate connected to USB via resistor divider
- Source on battery, drain to boost
- Logic: USB present = MOSFET OFF, absent = MOSFET ON

**Drawbacks**:

- Complex discrete circuit
- Critical tuning
- No integrated protections

---

**Last update**: August 2025  
**Validation status**: 🔧 Final design - Prototyping required
