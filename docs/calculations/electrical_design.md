# Calculs Électriques - Projet Ignis

## 1. Calculs Batterie et Consommation

### Batterie EEMB LP402535 (320 mAh)

- Capacité nominale : 320 mAh
- Tension nominale : 3.7V
- Impédance interne : 160 mΩ
- Charge :
  - Courant maximum : 320 mA (1C)
  - Tension maximum : 4.2 V
  - Courant en fin de charge : 6.4 mA
- Décharge :
  - Courant maximum : 2C = 640 mA
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
| MCP73871 (standby)| 18 µA        | Très faible en veille      |
| BQ51013B (standby)| 30 µA        | Récepteur Qi inactif       |
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

## 2. Récepteur Qi - BQ51013B

### Caractéristiques générales

- **Type** : Récepteur Qi WPC v1.2 (BPP - Baseline Power Profile)
- **Puissance** : Jusqu'à 5W
- **Tension de sortie** : 5V régulé
- **Courant max** : 1A
- **Fréquence** : 100-250 kHz (bande WPC)
- **Communication** : ASK/FSK avec transmetteur
- **FOD** : Foreign Object Detection intégré

### Bobine réceptrice - IWAS3010AZEB130KF1 (Vishay)

**Spécifications** :
- Dimensions : 30mm × 10mm (forme rectangulaire)
- Inductance : 12.9 µH @ 100 kHz
- Facteur de qualité Q : 11
- Résistance DC : 780 mΩ
- Puissance max : 3.5W
- Courant max : 800 mA
- Compliance Qi : WPC v1.2

### Calcul des condensateurs de résonance

Le BQ51013B nécessite deux condensateurs de résonance :

**C1 (série)** - Condensateur de résonance principale :
$$C_1 = \frac{1}{(2\pi \times f_S)^2 \times L_S^\prime}$$

Avec $L_S^\prime \approx 15\,\mu H$ dans notre cas (à confirmer) et $f_S = 100\,\text{kHz}$ (donné par la documentation du BQ51013B) :

$$C_1 \approx 168,868\,\text{nF}$$

On peut choisir $C_1 = 150\,\text{nF}$
ou $C_1 = 150\,\text{nF} + 15\,\text{nF} = 165\,\text{nF}$, à ce stade c'est une question d'implémentation. Dans une certaine mesure, la version à 2 condensateurs peut offrir une meilleure adaptabilité, surtout que $L_S^\prime$ n'est pas entièrement connue à ce stade. L'erreur en fréquence serait à ce moment $\leq 2.7\%$.



**C2 (parallèle)** - Condensateur de mise au point :

Avec $L_S = 12.9\,\mu H$ (inductance libre, donné par la datasheet de l'inductance) et $f_D = 1\,\text{MHz}$ (donné par la datasheet du BQ51013B) :

$$C_2 = \frac{1}{(2\pi \times f_D)^2 \times L_S - \frac{1}{C_1}}$$

soit $C_2 \approx 1,987\,\text{nF} \text{ à } 1,989\,\text{nF}$ selon que l'on choisisse $C_1 = 165\,\text{nF}$ ou $C_1 = 150\,\text{nF}$, autant dire qu'il n'y a aucune différence et on peut choisir $C_2 = 2\,\text{nF}$ ou $C_2 = 1\,\text{nF} + 1\,\text{nF}$.

### Calcul R_ILIM

La limitation de courant du BQ51013B se règle via la résistance $R_{ILIM}$ selon :

$$
R_{ILIM} = \frac{K_{ILIM}}{I_{MAX}}
$$

où $K_{ILIM}$ est donné par la datasheet ($614\,\Omega\cdot A$), et $I_{MAX}$ est le courant maximal souhaité.

La datasheet précise que le courant de limitation réel est :

$$
I_{ILIM} = 1.2 \times I_{MAX} = \frac{K_{ILIM}}{R_{ILIM}}
$$

$R_{ILIM}$ est la somme de deux résistances :

$$
R_{ILIM} = R_1 + R_{FOD}
$$

avec $R_{FOD}$ par défaut à $196\,\Omega$.

Pour $I_{MAX} \geq 600\,\text{mA}$, $I_{ILIM} = 720\,\text{mA}$

$$
R_{ILIM} = \frac{614}{0.72} \approx 436\,\Omega \implies R_1 = 240\,\Omega
$$

En pratique, avec $R_{FOD} = 196\,\Omega$, on choisit $R_1 = 220\,\Omega$ (valeur standard) :

$$
R_{ILIM} = 220 + 196 = 416\,\Omega \implies I_{ILIM} \approx 754\,\text{mA} \implies I_{MAX} \approx 629\,\text{mA}
$$

### Composants BQ51013B

- C1a (série) : 150 nF, 25V, X7R
- C1b (série) : 15 nF, 25V, X7R
- C2a (parallèle) : 1 nF, 25V, X7R
- C2b (parallèle) : 1 nF, 25V, X7R
- C3 (RECT) : 10 µF + 10µF + 0.1µF, 16V
- C4 (OUT) : 10 µF + 0.1µF, 16V
- C5 (BOOT) : 10 nF, 25V
- C6 (CLAMP) : 470 nF, 25V
- C7 (COMM) : 47 nF, 25V
- R1 (ILIM) : 220 Ω
- Rfod (FOD) : 196 Ω
- Ros (RECT) : 20 kΩ

### Configuration des broches

- **AC1/AC2** : Connexion bobine réceptrice
- **RECT** : Tension rectifiée (~8-10V)
- **OUT** : Sortie régulée 5V
- **AD** : Lié à GND via 1µF (pas de gestion dual-power)
- **EN** : Enable, pullup 10kΩ vers OUT
- **TS/CTRL** : Configuration, voir datasheet

## 3. Chargeur de batterie - MCP73871

### Avantages du MCP73871

Le MCP73871 remplace avantageusement le combo TP4056 + DMP1045U car il intègre :
- Chargeur Li-ion/Li-Po avec power-path management
- Commutation automatique entre sources (VIN/USB)
- Protection contre surcharge/décharge
- Indication de statut (LEDs)
- Régulation thermique

### Configuration power-path

**Principe** :
- **VIN** : Alimentation principale (sortie BQ51013B, 5V)
- **VBAT** : Connexion batterie Li-Po
- **VOUT** : Sortie système (vers boost FP6277)
- **Power-path** : Priorité VIN > batterie automatique

### Programmation courant de charge

Le courant de charge se programme via résistance RPROG :
$$I_{CHARGE} = \frac{1000\text{V}}{R_{PROG}}$$

Pour 320 mA (1C optimal) :
$$R_{PROG} = \frac{1000\text{V}}{320\text{mA}} = 3.125\,\text{k}\Omega \approx 3.3\,\text{k}\Omega$$

### Limitation courant USB (si applicable)

Résistance RUSB pour limiter courant d'entrée :
$$I_{USB} = \frac{515\text{V}}{R_{USB}}$$

Pour 500 mA max :
$$R_{USB} = \frac{515\text{V}}{500\text{mA}} = 1.03\,\text{k}\Omega \approx 1\,\text{k}\Omega$$

**Composants MCP73871** :
- RPROG : 3.3 kΩ (charge 303 mA)
- RUSB : 1 kΩ (limite 515 mA si USB)
- C1 (VIN) : 4.7 µF, 16V
- C2 (VBAT) : 4.7 µF, 16V
- C3 (VOUT) : 4.7 µF, 16V

## 4. Configuration FP6277 Boost

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

La limitation de courant du FP6277 se règle via une résistance entre la broche OC et la masse :

$$
I_{OCP} = \frac{180\,000}{R_{OC}} + 0.2
$$

Pour programmer $I_{OCP} = 0.64\,\text{A}$ (640 mA, max batterie) :

$$
R_{OC} = \frac{180\,000}{0.44} = 409\,\text{k}\Omega \approx 390\,\text{k}\Omega
$$

**Composants FP6277** :
- L1 : Inductance 3.3 µH, courant sat > 1A
- C1 (Vin) : 22 µF, 16V
- C2 (Vout) : 22 µF, 16V
- C3 (Vout) : 100 µF, 16V
- R1 : 75 kΩ (feedback high)
- R2 : 10 kΩ (feedback low)
- R3 : 390 kΩ (OC)

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

### Échauffement MCP73871

Le MCP73871 étant un chargeur switching (vs linéaire), les pertes sont réduites :
```
Pertes ≈ 0.1W @ 320mA charge (efficiency ~90%)
ΔT ≈ 0.1W / 8mW/°C ≈ 12°C élévation
```

## 6. Protection et sécurité

### Seuils de protection

- **Tension batterie min** : 3.0V (coupure boost)
- **Courant max** : 500mA (limite software)
- **Température max** : 70°C (monitoring si capteur ajouté)
- **FOD Qi** : Détection objets étrangers automatique

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

1. **Chargeur Qi** : Couplage avec TX standard, mesure puissance
2. **Résonance** : Vérifier fréquences avec condensateurs calculés
3. **Power-path** : Commutation automatique VIN/batterie
4. **Consommation patterns** : Valider calculs théoriques
5. **Autonomie réelle** : Test décharge complète
6. **Thermique** : Température max en fonctionnement

### Instrumentations

- Multimètre pour tensions/courants DC
- Oscilloscope pour ripples et switching
- Analyseur de spectre pour fréquences Qi
- Caméra thermique pour hotspots
- Charge électronique pour tests batterie

## 8. Brainstorming - Historique des choix de design

### Problématique initiale : Chargeur sans fil

**Motivations pour le Qi** :
- Étanchéité du boîtier (pas de port USB)
- Expérience utilisateur premium
- Compatibilité avec chargeurs commerciaux

**Défis identifiés** :
- Modules commerciaux trop chers (>11€)
- Puissance excessive (15W vs besoin 1.5-3W)
- Encombrement des bobines
- Complexité d'intégration PCB

### Choix de l'IC récepteur

**Options évaluées** :
- **BQ51003** : 2.5W, plus simple mais limite de puissance
- **BQ51013B** : 5W, plus de marge, documentation TI excellente
- **P9025AC (Renesas)** : Moins de documentation publique

**Décision** : BQ51013B pour la  documentation et la dispo LCSC (JLCPCB)

### Évolution du système de charge

**Version 1 (obsolète)** : TP4056 + DMP1045U
- Avantages : Composants connus, schémas de référence disponibles
- Inconvénients : Power-path artisanal, deux ICs séparés

**Version 2 (actuelle)** : MCP73871
- Avantages : Power-path intégré, plus propre, thermiquement meilleur
- Inconvénients : Changement de BOM, légèrement plus cher

### Choix de la bobine

**Critères** :
- Compliance Qi WPC v1.2
- Dimensions compatibles (≤35mm)
- Puissance suffisante (3W+)
- Disponibilité distribueurs

**IWAS3010AZEB130KF1 retenue** :
- 30×10mm : compact pour le boîtier flamme
- 3.5W : marge pour pics de consommation
- Q=11 : compromis efficacité/bande passante
- Vishay : marque fiable, stock Mouser/Digikey

### Philosophie power-path

**Deux niveaux identifiés** :
1. **Amont batterie** : Sélection sources (USB/Qi/AC)
   - Non applicable ici (une seule source Qi)
   - AD pin du BQ51013B à GND
2. **Aval batterie** : Système vs batterie
   - Critique pour éviter décharge batterie pendant charge
   - MCP73871 gère automatiquement

### Compromis et décisions finales

**Backfeed protection** : Finalement non nécessaire grâce au power-path intégré du MCP73871

**Sécurité** : Préférer composants avec protections intégrées (FOD, thermal shutdown) vs circuits externes

**BOM** : Privilégier intégration vs coût (MCP73871 vs TP4056+DMP1045)

---

## 9. Composants obsolètes (design précédent)

### TP4056 - Chargeur linéaire Li-ion

**Remplacé par MCP73871**

Programmation courant de charge via Rprog :
$$R_{PROG} = \frac{1100\,\text{V}}{I_{BAT}}$$

Pour 320 mA : $R_{PROG} = 3.9\,\text{k}\Omega$

**Inconvénients** :
- Chargeur linéaire → dissipation thermique
- Pas de power-path intégré
- Nécessite circuit externe pour commutation

### DMP1045U - P-MOSFET power-path

**Remplacé par MCP73871**

Circuit de commutation automatique entre USB et batterie :
- Gate relié à USB via diviseur résistif
- Source sur batterie, drain vers boost
- Logique : USB présent = MOSFET OFF, absent = MOSFET ON

**Inconvénients** :
- Circuit discret complexe
- Réglages critiques
- Pas de protection intégrée

---

**Dernière mise à jour** : Août 2025  
**Statut validation** : 🔧 Design finalisé - Prototypage requis
