# Analysis of failing inductive charging on IgnisV2.2 PCB

## Resonnance Frequency Adjustments: Step 1

Upon receiving the IgnisV2.2 PCB, the inductive charging did not function as
expected. One possible explanation is that the original documentation assumed
the coil inductance would increase to $16\,\mu\text{H}$ when installed
(including PCB, battery, etc.), but this may not be the case. For
troubleshooting, calculations are performed using the datasheet value:
$L_S = L_S' = 12\,\mu\text{H}$. All subsequent capacitance calculations are
based on this adjusted inductance.

### Series Capacitance

The equation:

$$
C_1 = \frac{1}{(2\pi f_S)^2 L_S'}
$$

where:

- $f_S = 100\,\text{kHz} +5/-10\%$
- $L_S' = 12\,\mu\text{H}$

Calculation:

$$
C_1 = \frac{1}{(2\pi \times 100\,000)^2 \times 12 \times 10^{-6}}
$$

Result:

$$
C_1 \approx 211.09\,\text{nF}
$$

### Parallel Capacitance

The equation:

$$
C_2 = \frac{1}{(2\pi f_D)^2 L_S' - 1/C_1}
$$

where:

- $f_D = 1\,\text{MHz} \pm 10\%$
- $L_S = 12\,\mu\text{H}$

Calculation:

$$
C_2 = \frac{1}{(2\pi \times 1\,000\,000)^2 \times 12 \times 10^{-6} - \frac{1}{200 \times 10^{-9}}}
$$

Result:

$$
C_2 \approx 2.13\,\text{nF}
$$

### COMM1/COMM2 Capacitors

It seems that some designs are successful with $22\,\text{nF}$ capacitors on
COMM1 and COMM2 rather than $47\,\text{nF}$.

### New Capacitor Values Summary

| Capacitor | Original Value   | New Value        | Adaption Action                                                     |
| --------- | ---------------- | ---------------- | ------------------------------------------------------------------- |
| C_1       | $165\,\text{nF}$ | $211\,\text{nF}$ | Solder 47nF capacitor on placeholder => $165 + 47 = 212\,\text{nF}$ |
| C_2       | $2\,\text{nF}$   | $2\,\text{nF}$   | _No changes required_                                               |

### Results

These modifications did not resolve the issue. Oscilloscope analysis shows that
coil voltage actually decreased. The `RECT` capacitor voltage remains around 3V,
while at least 5V (ideally 7V) is expected. It’s possible that soldering the
capacitor affected resonance, as a THT ceramic capacitor was used instead of an
0805 SMD. The coil frequency measured is 140kHz, not the expected 100kHz, and no
modulation occurs (COMM capacitors are not yet involved, so changing them is
unnecessary at this stage). The problem appears to be earlier in the circuit.

## Resonnance Frequency Adjustments: Step 2

The coil frequency is 140kHz instead of the expected 100kHz. To adjust, the
series capacitor $C_1$ needs to be recalculated for the new frequency.

$$
C_1 = \frac{1}{(2\pi \times 140\,\text{kHz})^2 \times 12\,\mu\text{H}} \approx 107\,\text{nF}
$$

| Adaption Action                                                             | New Value of $C_1$          | Result                                                                                                                 |
| --------------------------------------------------------------------------- | --------------------------- | ---------------------------------------------------------------------------------------------------------------------- |
| Remove original 15nF capacitor                                              | $165 - 15 = 150\,\text{nF}$ | Not much better                                                                                                        |
| Remove both 150nf and 15nF capacitors and add 87nF capacitor on placeholder | $87\,\text{nF}$             | Vpp on coil is higher (5V) but still not sufficient to start charging. It seems that communication is tried but fails. |
| Try more values on the 0402 footprint with 87nF still on the placeholder    | _Several values_            | Best result with a total of ~130nF with highest Vpp on coil, but still not sufficient to start charging.               |

## Modification of Transmitter

To better understand the IKEA Livboj transmitter, the unit was disassembled and
tested with the receiver coil placed directly on top of the transmitter coil.
When powered, charging was initiated only if the coils were precisely aligned.
Notably, varying the receiver-side capacitance (using either 130 nF or 212 nF)
did not affect charging performance. This suggests that the receiver circuit
design is fundamentally sound, but the coupling between the transmitter and
receiver coils is insufficient.

Further research indicates that geometry and size should match between the
receiver and transmitter coil. If this is true, the Vishay receiver coil,
constrained to 30 × 10 mm and featuring a rounded rectangular shape, could
feature an inefficient coupling with the IKEA Livboj’s 43 mm diameter circular
transmitter coil.

**Solution to try:** larger coil, possibly round.

<div align="center">

[![Test of inductive charge without casing](https://img.youtube.com/vi/-Y1U7MM0c8Y/0.jpg)](https://youtu.be/-Y1U7MM0c8Y)

</div>
