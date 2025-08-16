# 🔥 Projet Ignis - Lampe Ignis

Une lampe/veilleuse autonome en forme de flamme, dont le socle contient du sable (provenant d'un lieu particulier), ce qui lui confère un aspect mystérieux et précieux, pilotée par microcontrôleur avec anneau de LEDs WS2812B.

## 🎯 Objectifs

- **Autonomie** : 2-3 heures sur batterie LiPo (320-500 mAh)
- **Patterns LED** : Heartbeat, chenillard, fade, effets aléatoires
- **Contrôle** : Bouton pour changer de pattern (actif sur socle uniquement)
- **Charge** : USB avec protection batterie et power-path management

## 🔧 Composants Principaux

| Fonction   | Composant           | Spécifications                    |
| ---------- | ------------------- | --------------------------------- |
| MCU        | ATtiny212/ATtiny412 | Ultra-low-power, 5V, UPDI         |
| LEDs       | WS2812B             | 6-8 LEDs, ~60mA max/LED           |
| Chargeur   | TP4056              | Charge LiPo, courant programmable |
| Boost      | FP6277              | Boost synchrone 5V, >1A           |
| Power-path | DMP1045U            | P-MOSFET pour switch auto         |
| Batterie   | EEMB LP402535       | 320 mAh, max 1C continu           |

## 📁 Structure du Projet

```
ignis/
├── firmware/          # Code PlatformIO (ATtiny212)
│   ├── src/
│   ├── platformio.ini
│   └── lib/
├── hardware/          # Schémas et PCB
│   ├── schematic/
│   ├── pcb/
│   └── gerber/
├── docs/              # Documentation
│   ├── datasheets/
│   ├── calculations/
│   └── references/
├── mechanical/        # Conception 3D
│   ├── cad/
│   └── stl/
└── tests/            # Tests et validation
```

## ⚡ Architecture Électrique

```
USB 5V → TP4056 → DMP1045U → FP6277 → VSYS (5V)
                     ↓
                  Batterie LiPo
                     ↓
VSYS → ATtiny212 → WS2812B LEDs
    → Bouton (via socle)
```

## 📊 Consommations Estimées

| Configuration | LEDs | Courant     | Commentaire                    |
| ------------- | ---- | ----------- | ------------------------------ |
| 6 LEDs full   | 6    | ~365 mA     | Légèrement > 1C, OK temporaire |
| 8 LEDs full   | 8    | ~485 mA     | < 2C max, batterie chauffe     |
| PWM 50%       | 6-8  | ~180-240 mA | Optimal pour longévité         |

## 🛠️ Développement

### Prérequis

- **OS** : Linux
- **IDE** : VS Code + PlatformIO
- **Programmeur** : UPDI (JTAG2updi ou similaire)

### Installation

```bash
# Installer PlatformIO Core
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -o get-platformio.py
python3 get-platformio.py

# Cloner et compiler
cd firmware/
pio build
```

## 📚 Documentation Technique

Tous les datasheets et références sont disponibles dans `docs/datasheets/` :

- [ATtiny212 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/40001954C.pdf)
- [WS2812B Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf)
- [TP4056 Datasheet](https://www.mouser.fr/datasheet/2/268/TP4056-1525712.pdf)
- [FP6277 Datasheet](https://www.farnell.com/datasheets/3363611.pdf)
- [DMP1045U Datasheet](https://www.farnell.com/datasheets/1924636.pdf)

## 🚀 Mise en Route

1. **Hardware** : Assembler le PCB selon schémas dans `hardware/`
2. **Firmware** : Compiler et flasher avec PlatformIO
3. **Test** : Vérifier patterns LED et charge batterie
4. **Intégration** : Monter dans boîtier imprimé 3D

## 📝 TODO

- [ ] Finaliser schéma électrique avec power-path
- [ ] Calculer valeurs composants (Rprog TP4056, R6 FP6277)
- [ ] Implémenter patterns LED avancés
- [ ] Optimiser power management (sleep modes)
- [ ] Tests de validation batterie et sécurité
- [ ] Conception boîtier 3D
- [ ] Documentation utilisateur

## 🔒 Sécurité

- Respect des limites courant batterie (1C nominal, 2C max)
- Protection surchauffe (monitoring température)
- Coupure automatique en cas de batterie faible
- Isolation galvanique charge USB

---

**Statut** : 🚧 En développement  
**Dernière mise à jour** : Août 2025
