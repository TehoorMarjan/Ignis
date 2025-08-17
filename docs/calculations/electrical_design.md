# Calculs Électriques - Projet Ignis

## 1. Calculs Batterie et Consommation

### Batterie EEMB LP402535 (320 mAh)

- Capacité nominale : 320 mAh
- Tension nominale : 3.7V
- Impédance interne : 160 mΩ
- Charge :
  - Courant maximum : 320 mA
  - Tension maximum : 4.2 V
  - Courant en fin de charge : 6.4 mA
- Décharge :
  - Courant maximum :2C = 640 mA
  - Tension minimum : 2.75 V
- Température de fonctionnement :
  - En décharge : -20°C à +60°C
  - En charge : 0°C à +45°C

### Consommations par composant

| Composant         | Courant (mA) | Notes                      |
| ----------------- | ------------ | -------------------------- |
| ATtiny212 (actif) | 3-5          | @ 20 MHz, 5V               |
| ATtiny212 (sleep) | 0.1-1        | Power-down mode            |
| WS2812B (par LED) | 0.3-60       | 0.3mA off, 60mA full white |
| TP4056 (standby)  | 2-3          | Mode veille                |
| FP6277 (standby)  | 50-100 µA    | Très faible en veille      |

### Scénarios d'utilisation

#### Scénario 1 : 6 LEDs, luminosité 50%

```
LEDs : 6 × 30 mA = 180 mA
MCU : 5 mA
Boost : 2 mA (pertes estimation)
TOTAL : ~187 mA

Autonomie : 320 mAh / 187 mA = 1.7 heures
```

#### Scénario 2 : 8 LEDs, luminosité 75%

```
LEDs : 8 × 45 mA = 360 mA
MCU : 5 mA
Boost : 3 mA
TOTAL : ~368 mA

Autonomie : 320 mAh / 368 mA = 0.87 heures
⚠️ Dépasse 1C recommandé, OK pour usage ponctuel
```

#### Scénario 3 : Mode veille (patterns off)

```
MCU (sleep) : 0.5 mA
LEDs (off) : 8 × 0.3 = 2.4 mA
Boost (standby) : 0.1 mA
TOTAL : ~3 mA

Autonomie : 320 mAh / 3 mA = 106 heures (4.4 jours)
```

## 2. Dimensionnement TP4056

### Programmation courant de charge via Rprog

$$R_{PROG} = \frac{1100\,\text{V}}{I_{BAT}}$$

Pour 320 mA (1C optimal) :

$$R_{PROG} = \frac{1100\,\text{V}}{320\,\text{mA}} = 3.4375\,\text{k}\Omega \approx 3.9\,\text{k}\Omega$$

**Composants TP4056** :

- Rprog : 3.9 kΩ (0805, 1%)
- C1 (VBUS) : 100 nF (tant ou céram X7R)
- C2 (BAT) : 10 µF
- LEDs charge : Rouge/Verte avec résistances 1.5 kΩ -> Seront peut-être plutôt remplacées par des entrées sur le uC pour utiliser les WS2812B

## 3. Configuration FP6277 Boost

### Réglage tension sortie (5V)

La tension de sortie est définie par le pont diviseur selon :

$$
V_{out} = 0.6\,\text{V} \times \left(1 + \frac{R_1}{R_2}\right)
$$

Pour $V_{out} = 5\,\text{V}$ :

$$
5 = 0.6 \times \left(1 + \frac{R_1}{R_2}\right) \\
\frac{R_1}{R_2} = \frac{5}{0.6} - 1 = 7.33
$$

Si $R_2 = 10\,\text{k}\Omega$ alors $R_1 = 73.3\,\text{k}\Omega \approx 75\,\text{k}\Omega$

### Calcul inductance optimale

L'inductance optimale pour le FP6277 se calcule avec :

$$
L = \frac{V_{in(max)} \times (V_{out} - V_{in(max)})}{V_{out} \times (\Delta I_L) \times f_{sw}}
$$

Avec :

- $V_{in(max)} = 4.2\,V$ (batterie pleine)
- $V_{out} = 5.0\,V$
- $\Delta I_L = 500\,\text{mA} \times 30\% = 0.15\,A$ (ripple 30% du courant moyen)
- $f_{sw} = 1.4\,\text{MHz}$ (fréquence FP6277)

$$
L = \frac{4.2 \times (5.0 - 4.2)}{5.0 \times 0.15 \times 1.4 \times 10^6} = 3.2\,\mu H
$$

**Inductance recommandée** : 3.3 µH (permet de rejoindre le design de référence)

### Protection contre surintensité

La limitation de courant du FP6277 se règle via une résistance entre la broche OC et la masse. La valeur de la résistance détermine le courant de déclenchement selon :

$$
I_{OCP} = \frac{180\,000}{R_{OC}} + 0.2
$$

où :

- $I_{OCP}$ : courant de déclenchement (A)
- $R_{OC}$ : résistance entre OC et GND (Ω)

Pour programmer $I_{OCP} = 0.64\,\text{A}$ (640 mA, max batterie) :

$$
R_{OC} = \frac{180\,000}{I_{OCP} - 0.2} = \frac{180\,000}{0.44} = 409\,\text{k}\Omega
$$

**Choix recommandé** : $R_{OC} = 390\,\text{k}\Omega$ pour $I_{OCP} \approx 660\,\text{mA}$

### Composants FP6277

- L1 : Inductance 3.3 µH, courant sat > 1A (ex: Würth 744773110)
- C1 (Vin) : 22 µF, 16V
- C2 (Vout) : 22 µF, 16V
- C3 (Vout) : 100 µF, 16V
- R1 : 75 kΩ (feedback high)
- R2 : 10 kΩ (feedback low)
- R3 : 390 kΩ (OC)

## 4. Power-Path avec DMP1045U

### Circuit de commutation automatique

Le P-MOSFET DMP1045U permet le basculement automatique entre USB et batterie.

**Principe** :

- Gate relié à USB via diviseur résistif
- Source sur batterie
- Drain vers boost converter
- Quand USB présent : MOSFET OFF, alimentation directe USB
- Quand USB absent : MOSFET ON, alimentation batterie

**Composants** :

- R3, R4 : Diviseur 10kΩ/10kΩ sur gate
- R5 : Pull-up 100kΩ sur gate vers batterie
- C3 : 100nF filtrage gate

## 5. Bilan thermique

### Pertes boost converter

```
Pertes ≈ (Vout - Vin) × Iout / η

Cas critique : Vin = 3.0V, Vout = 5V, Iout = 400mA, η = 85%
Pertes = (5 - 3) × 0.4 / 0.85 = 0.94W
```

**Gestion thermique** :

- Via thermique sous FP6277
- Plan de masse étendu
- Copper pour dissipation

### Échauffement batterie

Résistance interne LiPo ≈ 100-200 mΩ

```
Pertes batterie = I² × Ri = 0.4² × 0.15 = 24mW
ΔT ≈ 24mW / 5mW/°C = 5°C élévation
```

**→ Échauffement acceptable**

## 6. Protection et sécurité

### Seuils de protection

- **Tension batterie min** : 3.0V (coupure boost)
- **Courant max** : 500mA (limite software)
- **Température max** : 70°C (monitoring si capteur ajouté)

### Monitoring firmware

```cpp
#define VBAT_MIN 3000  // mV
#define IBAT_MAX 500   // mA
#define TEMP_MAX 70    // °C

void checkSafety() {
  if (getBatteryVoltage() < VBAT_MIN) {
    shutdownSystem();
  }

  if (getBatteryCurrent() > IBAT_MAX) {
    reduceLEDPower();
  }
}
```

## 7. Validation expérimentale

### Tests à effectuer

1. **Courant de charge** : Vérifier 308mA avec multimètre
2. **Tension boost** : Mesurer 5.00V ± 2% en sortie
3. **Consommation patterns** : Valider calculs théoriques
4. **Autonomie réelle** : Test décharge complète
5. **Power-path** : Basculement USB/batterie
6. **Thermique** : Température max en fonctionnement

### Instrumentations

- Multimètre pour tensions/courants DC
- Oscilloscope pour ripples et switching
- Caméra thermique pour hotspots
- Charge électronique pour tests batterie

---

**Dernière mise à jour** : Août 2025  
**Statut validation** : 🔧 Calculs théoriques - Tests requis
