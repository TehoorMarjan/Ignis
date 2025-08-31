# 🔥 Projet Ignis - Lampe Ignis

Une lampe/veilleuse autonome en forme de flamme, dont le socle contient du sable
(provenant d'un lieu particulier), ce qui lui confère un aspect mystérieux et
précieux, pilotée par microcontrôleur avec anneau de LEDs WS2812B et **charge
sans fil Qi intégrée**.

<p align="center">
  <img src="mechanical/art/Flamme4/Flamme4_0001.png" alt="Aperçu Flamme4" style="max-width:600px;">
</p>

## 🎯 Objectifs

- **Autonomie** : 2-3 heures sur batterie LiPo (320 mAh)
- **Patterns LED** : Heartbeat, chenillard, fade, effets flamme réalistes
- **Contrôle** : Détecteur de choc SW-18030 pour interaction
- **Charge** : **Sans fil Qi** + power-path management automatique
- **Étanchéité** : Pas de port USB, charge par induction uniquement

## 🔧 Composants Principaux (IgnisV2)

| Fonction     | Composant              | Spécifications                        |
| ------------ | ---------------------- | ------------------------------------- |
| MCU          | ATtiny212-SSN          | Ultra-low-power, 5V, UPDI             |
| LEDs         | WS2812B-2020           | 6-8 LEDs, ~60mA max/LED               |
| Chargeur     | **BQ24073RGTR**        | Li-ion switching charger + power-path |
| Récepteur Qi | **BQ51013B**           | 5W, compatible WPC v1.2               |
| Bobine Qi    | **IWAS3010AZEB130KF1** | 30×10mm, 12.9µH, facteur Q=11         |
| Boost        | FP6277XR-G1            | Boost synchrone 5V, >1A               |
| Détecteur    | **SW-18030**           | Capteur choc + filtre 74LVC1G17       |
| Batterie     | EEMB LP402535          | 320 mAh, protection intégrée          |

## 📁 Structure du Projet

```
ignis/
├── firmware/          # Code PlatformIO (ATtiny212)
│   ├── src/
│   ├── platformio.ini
│   └── lib/
├── hardware/          # Schémas et PCB
│   ├── IgnisV1-TP4056/    # Design obsolète (référence)
│   ├── IgnisV2/           # Design actuel (BQ24073 + Qi)
│   └── Libraries/         # Footprints et modèles 3D
├── docs/              # Documentation
│   ├── datasheets/
│   ├── calculations/
│   └── references/
├── mechanical/        # Conception 3D
│   ├── art/               # Modèles Blender (Flamme4)
│   ├── cad/               # FreeCAD (assemblage)
│   └── stl/               # Fichiers impression
├── assets/            # Logos et branding
│   └── logo/
└── tests/            # Tests et validation
```

## ⚡ Architecture Électrique IgnisV2

```
Chargeur Qi (BQ51013B) → BQ24073 → Batterie LiPo 320mAh
   ↑ 5W @ 125kHz           ↓
Bobine IWAS3010         Power-path management
                            ↓
                    FP6277 Boost → VSYS (5V)
                            ↓
                    ATtiny212 @ 20MHz
                       ↙     ↓     ↘
               SW-18030    WS2812B-2020   LED Status
              (réveil)     (6-8 LEDs)      (debug)
```

## 📊 Consommations & Performance

| Configuration | LEDs | Courant     | Autonomie | Commentaire               |
| ------------- | ---- | ----------- | --------- | ------------------------- |
| 6 LEDs full   | 6    | ~365 mA     | ~50 min   | Luminosité maximale       |
| 8 LEDs full   | 8    | ~485 mA     | ~40 min   | Mode spectacle temporaire |
| PWM 30%       | 6-8  | ~110-145 mA | ~2-3h     | **Mode nominal optimal**  |
| Sleep mode    | 0    | ~1-3 mA     | >100h     | Veille avec réveil choc   |

### ⚡ Charge Sans Fil

- **Puissance** : 5W max (BQ51013B)
- **Efficacité** : ~70% (récepteur + chargeur)
- **Temps charge** : 1.5-2h pour 320mAh @ 1C
- **Compatible** : Chargeurs Qi standard WPC v1.2

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

### Composants Principaux

- [ATtiny212 Datasheet](docs/datasheets/ATtiny212_datasheet.pdf)
- [BQ24073 Datasheet](docs/datasheets/bq24073.pdf) - Chargeur switching
- [BQ51013B Datasheet](docs/datasheets/BQ51013B_datasheet.pdf) - Récepteur Qi
- [WS2812B Datasheet](docs/datasheets/WS2812B_datasheet.pdf)
- [74LVC1G17 Datasheet](docs/datasheets/74LVC1G17_datasheet.pdf) - Buffer
  Schmitt

### Composants Passifs

- [FP6277 Datasheet](docs/datasheets/FP6277_datasheet.pdf) - Boost converter
- [IWAS3010 Datasheet](docs/datasheets/IWAS3010AZEB130KF1_datasheet.pdf) -
  Bobine Qi
- [SW-18030 Datasheet](docs/datasheets/SW-18010.PDF) - Détecteur vibration
- [EEMB LP402535 Datasheet](docs/datasheets/EEMB_LP402535_datasheet.pdf) -
  Batterie

### Calculs & Références

- [Calculs électriques complets](docs/calculations/electrical_design.md)
- [Estimation filament 3D](docs/calculations/filament.md)
- [Schémas de référence](docs/references/schematics_references.md)

## 🚀 Mise en Route

### 🏭 Production Hardware

1. **PCB IgnisV2** : Fabriqué chez JLCPCB avec assemblage PCBA
2. **Composants** : Tous sourcés et vérifiés (basic + advanced PCBA)
3. **Validation** : Issues critiques résolues (pinout WS2812B-2020, vias)

### 💾 Firmware

1. **Environnement** : VS Code + PlatformIO configuré
2. **Compilation** : `pio build` dans `/firmware/`
3. **Programming** : UPDI via programmateur externe (breadboard)

### 🔧 Tests & Validation

1. **Hardware** : Vérifier alimentations et continuités
2. **Charge Qi** : Tester avec chargeur compatible WPC v1.2
3. **LEDs** : Valider patterns et limitation courant
4. **Détecteur** : Vérifier réveil sur choc SW-18030

### 🏠 Assemblage Final

1. **Boîtier** : Imprimer Flamme4 (dual-wall design)
2. **Intégration** : PCB + batterie + bobine Qi
3. **Étanchéité** : Validation charge sans contact

## 📝 Status & Roadmap

### ✅ **Réalisé (IgnisV2)**

- [x] **Schéma électrique finalisé** (BQ24073 + BQ51013B + SW-18030)
- [x] **PCB layout complet** avec optimisation JLCPCB
- [x] **Production lancée** (advanced PCBA, délai 2-3 semaines)
- [x] **Modèles 3D** (Flamme4 dual-wall + assemblage complet)
- [x] **Documentation complète** (calculs, datasheets, références)
- [x] **Branding** (logo TheoToys avec génération automatisée)

### 🚧 **En cours**

- [ ] **Réception PCBs** (attente fabrication JLCPCB)
- [ ] **Validation hardware** (premier power-on, tests électriques)
- [ ] **Firmware sur hardware réel** (port vers IgnisV2)
- [ ] **Tests charge Qi** (efficacité, compatibilité chargeurs)

### 🎯 **Prochaines étapes**

- [ ] **Patterns flamme avancés** (simulation réaliste avec WS2812B-2020)
- [ ] **Power management optimisé** (sleep modes, détection choc)
- [ ] **Assemblage mécanique** (impression 3D + intégration complète)
- [ ] **Tests durabilité** (cycles charge/décharge, autonomie réelle)
- [ ] **Documentation utilisateur** et préparation petite série

## 🔒 Sécurité & Qualité

### ⚡ **Protection Électrique**

- **Courant batterie** : Limitation logicielle (1C nominal, protection hardware)
- **Protection thermique** : BQ24073 avec thermal shutdown intégré
- **Power-path intelligent** : Priorité automatique Qi vs batterie
- **Seuils sécurité** : Monitoring tensions et détection batterie faible

### 🛡️ **Qualité Design**

- **Test points** : Debug et validation post-assemblage
- **Solder jumpers** : Corrections possibles sans nouveau PCB
- **Composants qualifiés** : Sourcing JLCPCB avec vérification dispo
- **Issues résolues** : Pinout WS2812B-2020, vias undersized, couleurs STEP

### 🧪 **Validation**

- **DRC JLCPCB** : Rules de fabrication intégrées dès le design
- **Simulation** : Calculs électriques et thermiques validés
- **3D visualization** : Assemblage mécanique pré-validé
- **Documentation** : Traçabilité complète composants et références

---

**Statut** : 🏭 **Production hardware en cours**  
**Dernière mise à jour** : Septembre 2025  
**Next milestone** : Réception et validation IgnisV2 PCB
