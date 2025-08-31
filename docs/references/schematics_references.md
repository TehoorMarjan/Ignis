# Références Schémas - Projet Ignis

## Schémas de Référence

### 1. FP6277 Boost Converter

**Source** :
[LiPo Power Board FP6277 - wagiminator](https://github.com/wagiminator/Power-Boards/blob/master/LiPo_Power_Board_FP6277/LiPo_Power_Board_FP6277_schematic.pdf)

**Modifications pour Ignis** :

- **R6** : Changer de 27kΩ à 225kΩ pour limiter courant à 1A
- **Suppression DW01** : Pas de protection batterie intégrée (gérée par TP4056)
- **Sortie 5V** : R1=75kΩ, R2=10kΩ pour Vout=5V

### 2. TP4056 avec Load Sharing

**Source** :
[LiPo Charger LS TP4056 - wagiminator](https://github.com/wagiminator/Power-Boards/blob/master/LiPo_Charger_LS_TP4056/LiPo_Charger_LS_TP4056_schematic.pdf)

**Adaptations** :

- **Rprog** : 3.9kΩ pour 308mA de charge
- **Power-path** : Via DMP1045U au lieu de circuit intégré
- **Connecteur USB** : USB-C pour charge moderne

## Architecture Power-Path Ignis

```
     USB-C 5V
         │
         ├─── TP4056 (Charge) ─── VBAT (3.7V)
         │                          │
         │                          │
    DMP1045U ──────────────────────┘
      │
      └─── FP6277 (Boost) ─── VSYS (5V)
                                 │
                                 ├─── ATtiny212
                                 └─── WS2812B LEDs
```

## Pinout ATtiny212

```
                    ATtiny212
                  ┌─────────────┐
    VCC (5V) ──── │1  ●    8│ ──── GND
    PA6/UPDI ──── │2       7│ ──── PA3
    PA7      ──── │3       6│ ──── PA2
    PA1      ──── │4       5│ ──── PA0 (LED_DATA)
                  └─────────────┘
```

**Affectation pins** :

- **PA0 (Pin 5)** : DATA WS2812B
- **PA1 (Pin 4)** : BUTTON (avec pull-up)
- **PA2 (Pin 6)** : Réservé (expansion)
- **PA3 (Pin 7)** : Réservé (ADC batterie ?)
- **PA6 (Pin 2)** : UPDI (programmation)
- **PA7 (Pin 3)** : Réservé

## Connecteur Socle

### Pogo Pins (recommandé)

```
Socle → Bloc
[1] GND     ─── GND
[2] VSYS    ─── Alimentation 5V
[3] BUTTON  ─── PA1 (ATtiny212)
[4] UPDI    ─── PA6 (prog optionnelle)
```

### Alternative : Connecteur magnétique

- Utiliser connecteurs magnétiques 4 pins
- Polarité assurée par aimants
- Plus robuste que pogo pins

## Circuit de Détection Socle

```cpp
// Détection présence sur socle
bool isOnDock() {
  // Méthode 1 : Pull-up sur pin bouton
  // Si flottant (pas sur socle) → HIGH permanent
  // Si sur socle → possibilité LOW (bouton pressé)

  pinMode(BUTTON_PIN, INPUT);  // Pas de pull-up
  delay(1);
  bool floating = digitalRead(BUTTON_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Remettre pull-up

  return !floating;  // Si pas flottant = sur socle
}
```

## Schéma Bloc Complet

```
    USB-C           TP4056          DMP1045U         FP6277
 ┌─────────┐    ┌────────────┐   ┌────────────┐   ┌──────────┐
 │  VBUS   ├────┤ VCC    BAT ├───┤ S      D   ├───┤ VIN  OUT ├─── VSYS (5V)
 │  D+     │    │            │   │  \    /    │   │          │
 │  D-     │    │        OUT ├─┐ │   ┘  ┌     │   │          │
 │  GND    ├────┤ GND    GND ├─┼─┤ GND  G     │   │ GND      ├─── GND
 └─────────┘    └────────────┘ │ └─────┬──────┘   └──────────┘
                                │       │ 10kΩ                    │
                           LiPo │       └──────                   │
                      320mAh ──┘              │                   │
                                              GND                 │
                                                                  │
                      ATtiny212                                   │
                 ┌────────────────┐                               │
          PA0 ───┤ LED_DATA   VCC ├───────────────────────────────┘
          PA1 ───┤ BUTTON     GND ├─── GND
          PA6 ───┤ UPDI           │
                 └────────────────┘
                        │
                        │ LED_DATA
                 ┌──────▼──────┐
                 │  WS2812B    │
                 │  Ring 8LEDs │
                 └─────────────┘
```

## Liste des Composants (BOM)

### Actifs

| Ref    | Composant | Package | Valeur | Fournisseur | Prix (€) |
| ------ | --------- | ------- | ------ | ----------- | -------- |
| U1     | ATtiny212 | SOIC-8  | -      | Mouser      | 1.20     |
| U2     | TP4056    | SOP-8   | -      | LCSC        | 0.30     |
| U3     | FP6277    | SOP-8   | -      | LCSC        | 0.80     |
| Q1     | DMP1045U  | SOT-23  | -      | Mouser      | 0.60     |
| LED1-8 | WS2812B   | 5050    | -      | AliExpress  | 1.00     |

### Passifs

| Ref   | Composant    | Package | Valeur | Notes          |
| ----- | ------------ | ------- | ------ | -------------- |
| R1    | Résistance   | 0805    | 75kΩ   | Feedback boost |
| R2    | Résistance   | 0805    | 10kΩ   | Feedback boost |
| R3    | Résistance   | 0805    | 3.9kΩ  | Rprog TP4056   |
| R4,R5 | Résistance   | 0805    | 10kΩ   | Diviseur gate  |
| C1    | Condensateur | 0805    | 22µF   | Input boost    |
| C2    | Condensateur | 0805    | 47µF   | Output boost   |
| C3,C4 | Condensateur | 0603    | 100nF  | Découplage     |
| L1    | Inductance   | 4040    | 10µH   | Boost, Isat>1A |

### Connectiques

- USB-C femelle pour charge
- Connecteur batterie JST-PH 2 pins
- Pogo pins 4 contacts (socle)

**Coût total estimé** : ~15€ en composants

---

**Prochaines étapes** :

1. Dessiner schéma complet sous KiCad
2. Routage PCB compact (≤ 40x40mm)
3. Génération Gerber et commande PCB
4. Tests de validation électrique
