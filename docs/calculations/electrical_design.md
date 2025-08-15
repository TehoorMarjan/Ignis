# Calculs Électriques - Projet Ignis

## 1. Calculs Batterie et Consommation

### Batterie EEMB LP402535 (320 mAh)

- **Capacité nominale** : 320 mAh
- **Tension nominale** : 3.7V
- **Courant continu max recommandé** : 1C = 320 mA
- **Courant max absolu** : 2C = 640 mA
- **Température de fonctionnement** : -20°C à +60°C

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

```
Icharge = 1200V / Rprog
```

Pour 320 mA (1C optimal) :

```
Rprog = 1200V / 0.32A = 3750 Ω ≈ 3.9 kΩ (valeur standard)
Icharge réel = 1200 / 3900 = 0.308A = 308 mA ✓
```

**Composants TP4056** :

- Rprog : 3.9 kΩ (0805, 1%)
- C1 (VCC) : 10 µF (tant ou céram X7R)
- C2 (BAT) : 10 µF
- LED charge : Rouge/Verte avec résistances 1 kΩ

## 3. Configuration FP6277 Boost

### Réglage tension sortie (5V)

Tension de sortie définie par diviseur résistif :

```
Vout = 0.6V × (1 + R1/R2)
```

Pour Vout = 5V :

```
5V = 0.6V × (1 + R1/R2)
R1/R2 = 7.33

Si R2 = 10 kΩ → R1 = 73.3 kΩ ≈ 75 kΩ
```

**Composants FP6277** :

- L1 : Inductance 10 µH, courant sat > 1A (ex: Würth 744773110)
- C1 (Vin) : 22 µF, 10V (tant ou céram)
- C2 (Vout) : 47 µF, 10V
- R1 : 75 kΩ (feedback high)
- R2 : 10 kΩ (feedback low)
- Diode Schottky : SS54 ou équivalent

### Calcul inductance optimale

```
L = Vin(max) × (Vout - Vin(max)) / (ΔIL × fsw × Vout)

Avec :
- Vin(max) = 4.2V (batterie pleine)
- Vout = 5V
- ΔIL = 0.3A (ripple 30% du courant moyen)
- fsw = 1.4 MHz (fréquence FP6277)

L = 4.2 × (5 - 4.2) / (0.3 × 1.4×10⁶ × 5) = 1.6 µH
```

**Inductance recommandée** : 10 µH (marge de sécurité)

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
