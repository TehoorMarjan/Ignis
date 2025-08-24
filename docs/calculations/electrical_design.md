# Calculs Électriques - Projet Ignis

## Calculs Batterie et Consommation

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

| Composant          | Courant (mA) | Notes                      |
| ------------------ | ------------ | -------------------------- |
| ATtiny212 (actif)  | 3-5          | @ 20 MHz, 5V               |
| ATtiny212 (sleep)  | 0.1-1        | Power-down mode            |
| WS2812B (par LED)  | 0.3-60       | 0.3mA off, 60mA full white |
| MCP73871 (standby) | 18 µA        | Très faible en veille      |
| BQ51013B (standby) | 30 µA        | Récepteur Qi inactif       |
| FP6277 (standby)   | 50-100 µA    | Très faible en veille      |

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

## Récepteur Qi - BQ51013B

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

où $K_{ILIM}$ est donné par la datasheet ($614\,\Omega\text{A}$), et $I_{MAX}$ est le courant maximal souhaité.

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

## Chargeur de batterie - BQ24073

Le BQ24073 de Texas Instruments est un chargeur Li-ion/Li-Po switching avec power-path management intégré :

- Chargeur switching haute efficacité (vs linéaire)
- Power-path management automatique
- Compatible avec récepteur Qi BQ51013B (même fabricant)
- Protection thermique et électrique intégrée
- Indication de statut via broches CHG et PGOOD
- Gestion du courant d'entrée via EN1/EN2

### Programmation de la charge

Le courant de charge se programme via résistance $R_{ISET}$ connectée de la broche ISET vers VSS :
$$R_{ISET} = \frac{K_{ISET}}{I_{CHARGE}}$$

$$
K_{ISET} = 890\,\Omega\text{A} \implies R_{ISET} = \frac{890}{0.32} \approx 2781\,\Omega \approx \boxed{2.7\,k\Omega}
$$

$$
R_{TMR} = \frac{t_{MAXCHG}}{10 \times K_{TMR}}\,\text{avec}\, K_{TMR} = 48 \text{s}/\text{k}\Omega
$$

Pour une charge à 1 C (320 mA), la durée théorique est de 1 h. En pratique, la charge s'effectue en deux phases :

- **Phase CC (courant constant)** : 320 mA jusqu'à environ 4,2 V, soit 40 à 50 min pour atteindre 70 à 80 % de la capacité.
- **Phase CV (tension constante)** : la tension reste à 4,2 V, le courant diminue progressivement jusqu'au seuil de fin (généralement 0,1 C à 0,05 C), ce qui ajoute 20 à 50 min.

En règle générale :
- Arrêt à 0,1 C (32 mA) : durée totale de 1,2 à 1,4 h.
- Arrêt à 0,05 C (16 mA) : durée totale de 1,4 à 1,8 h.

Une pré-charge à faible courant (0,05–0,1 C) peut ajouter 10 à 20 min si la cellule est très déchargée.

On retient donc une durée de charge typique comprise entre 1,2 et 1,6 h à 1 C, selon le seuil d'arrêt, la température et l'état initial de la batterie.

On choisit $t_{MAXCHG}$ à deux fois la durée attendue pour couvrir les cas de courant réduit :  

$$
t_{MAXCHG} = 2 \times 1,6\,\text{h} = 3,2\,\text{h} = 11\,520\,\text{s} \\
\implies R_{TMR} = \frac{11 520}{10 \times 48} = \boxed{24\,\text{k}\Omega}
$$

### Limitation courant d'entrée

Pour ce projet, on utilise le mode programmable (EN2=1, EN1=0) pour ajuster la limite d'entrée selon la capacité du récepteur Qi. Calculer $R_{ILIM}$ pour fixer le courant max à ≤ 629 mA.

$$R_{ILIM} = \frac{K_{ILIM}}{I_{IN\_MAX}}$$

$$
K_{ILIM} = 1550\,\Omega\text{A} \implies R_{ILIM} = \frac{1550}{0.629} \approx 2.46\,k\Omega \approx \boxed{2.4\,k\Omega}
$$

### Composants BQ24073 (à calculer)

- C_IN : 1µF, 16V, X7R
- C_BAT : 4.7µF, 16V, X7R
- C_OUT : 4.7µF, 16V, X7R
- R_TS : 10kΩ (fixe, remplace thermistance)
- R_ILIM : 2.4kΩ
- R_ISET : 2.7kΩ
- R_TMR : 24kΩ

## Configuration FP6277 Boost

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

## Calcul du filtre pour le SW18030

Le SW18030 est un capteur de choc utilisant un ressort enroulé autour d'une tige métallique. Lors d'un choc ou d'une accélération, le ressort vient brièvement toucher la tige, générant un contact très court. Pour prolonger ce signal et le rendre exploitable, on ajoute un circuit RC qui étire la durée de l'impulsion. Afin d'obtenir un signal carré propre, compatible avec le réveil de l'ATtiny en mode sommeil, on place ensuite un Bascule de Schmitt non-inverseur à la sortie du filtre RC.

Lors d'un choc, le SW18030 ferme le contact pendant ~1 ms. Pour obtenir une impulsion exploitable, la constante de temps du filtre RC doit être bien inférieure à cette durée.

**Filtre RC (charge rapide) :**

- Résistance série : $R_s$
- Capacité : $C$
- Constante de temps : $\tau = R_s \times C$

Pour ce projet :

- $R_s = 470\,\Omega$
- $C = 22\,\text{nF}$
- $\tau = 470\,\Omega \times 22\,\text{nF} = 10{,}3\,\mu\text{s} \ll 1\,\text{ms}$

Le condensateur se charge quasi-instantanément, permettant au signal d’atteindre rapidement le seuil haut du Schmitt ($V_T^+$).

**Décharge (étirement du signal) :**

- Résistance de décharge : $R_d$
- Durée d'impulsion : $t = R_d \cdot C \cdot \ln\left(\frac{V_{CC}}{V_T^-}\right)$

Pour ce projet :

- On souhaite $t \approx 15\,\text{ms}$
- $R_d = 680\,\text{k}\Omega$ (le calcul donne $R_d \approx 667\,\text{k}\Omega$)
- $V_{CC} = 5\,\text{V}$
- $V_T^- = 1.8\,\text{V}$ (donné par la datasheet)
- $t = 680\,\text{k}\Omega \times 22\,\text{nF} \times \ln\left(\frac{5}{1.8}\right) \approx 15.3\,\text{ms}$ => OK

Ce temps garantit une impulsion suffisamment longue pour réveiller l’ATtiny en mode sommeil, sans rebonds parasites.

**Composants SW18030** :

- Rs : 470 Ω
- Cs : 22 nF
- R_d : 680 kΩ
- 74LVC1G17 : Bascule de Schmitt avec tampon (non-inverseur).

## Bilan thermique

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

### Échauffement batterie

Résistance interne LiPo ≈ 100-200 mΩ

```
Pertes batterie = I² × Ri = 0.4² × 0.15 = 24mW
ΔT ≈ 24mW / 5mW/°C = 5°C élévation
```

## Protection et sécurité

### Seuils de protection

- **Tension batterie min** : 3.0V (coupure boost)
- **Courant max** : 500mA (limite software)
- **Température max** : 70°C (monitoring si capteur ajouté)
- **FOD Qi** : Détection objets étrangers automatique

## Brainstorming - Historique des choix de design

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

**Décision** : BQ51013B pour la documentation et la dispo LCSC (JLCPCB)

### Évolution du système de charge

**Version 1 (obsolète)** : TP4056 + DMP1045U

- Avantages : Composants connus, schémas de référence disponibles
- Inconvénients : Power-path artisanal, deux ICs séparés

**Version 2 (obsolète)** : MCP73871

- Avantages : Power-path intégré, plus propre, thermiquement meilleur
- Inconvénients : Disponibilité, coût

**Version 3 (actuelle)** : BQ24073

- Avantages : Même fabricant que BQ51013B (TI), switching efficiency, power-path intégré
- Inconvénients : Complexité de configuration initiale

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

**Backfeed protection** : Non necessaire car pas de sources de recharges multiples (uniquement Qi)

**Sécurité** : Préférer composants avec protections intégrées (FOD, thermal shutdown) vs circuits externes

**BOM** : Privilégier intégration vs coût (MCP73871 vs TP4056+DMP1045)

---

## Composants obsolètes (design précédent)

### MCP73871 - Chargeur avec power-path intégré

**Remplacé par BQ24073**

Avantages du MCP73871 :
- Chargeur Li-ion/Li-Po avec power-path management
- Commutation automatique entre sources (VIN/USB)
- Protection contre surcharge/décharge
- Indication de statut (LEDs)
- Régulation thermique

Configuration power-path :
- **VIN** : Alimentation principale (sortie BQ51013B, 5V)
- **VBAT** : Connexion batterie Li-Po
- **VOUT** : Sortie système (vers boost FP6277)
- **Power-path** : Priorité VIN > batterie automatique

Programmation courant de charge :
$$I_{CHARGE} = \frac{1000\text{V}}{R_{PROG}}$$

Pour 320 mA (1C optimal) : $R_{PROG} = 3.3\,\text{k}\Omega$

Limitation courant USB :
$$I_{USB} = \frac{515\text{V}}{R_{USB}}$$

Pour 500 mA max : $R_{USB} = 1\,\text{k}\Omega$

**Composants MCP73871** :
- RPROG : 3.3 kΩ (charge 303 mA)
- RUSB : 1 kΩ (limite 515 mA si USB)
- C1 (VIN) : 4.7 µF, 16V
- C2 (VBAT) : 4.7 µF, 16V
- C3 (VOUT) : 4.7 µF, 16V

**Raisons du changement** : Simplicité du BQ24073, meilleure intégration avec récepteur Qi BQ51013B (même fabricant)

### TP4056 - Chargeur linéaire Li-ion

**Remplacé par BQ24073**

Programmation courant de charge via Rprog :
$$R_{PROG} = \frac{1100\,\text{V}}{I_{BAT}}$$

Pour 320 mA : $R_{PROG} = 3.9\,\text{k}\Omega$

**Inconvénients** :

- Chargeur linéaire → dissipation thermique
- Pas de power-path intégré
- Nécessite circuit externe pour commutation

### DMP1045U - P-MOSFET power-path

**Remplacé par BQ24073**

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
