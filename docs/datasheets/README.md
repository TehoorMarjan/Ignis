# 📋 Datasheets - Projet Ignis

Index des datasheets téléchargées pour le projet lampe "Flamme dans le sable".

## 🔗 Composants Principaux

| Composant         | Fichier                                                      | Description                                |
| ----------------- | ------------------------------------------------------------ | ------------------------------------------ |
| **ATtiny212**     | [`ATtiny212_datasheet.pdf`](ATtiny212_datasheet.pdf)         | Microcontrôleur principal, ultra-low-power |
| **WS2812B**       | [`WS2812B_datasheet.pdf`](WS2812B_datasheet.pdf)             | LEDs adressables RGB, 5V                   |
| **TP4056**        | [`TP4056_datasheet.pdf`](TP4056_datasheet.pdf)               | Contrôleur de charge LiPo                  |
| **FP6277**        | [`FP6277_datasheet.pdf`](FP6277_datasheet.pdf)               | Boost converter synchrone 5V               |
| **DMP1045U**      | [`DMP1045U_datasheet.pdf`](DMP1045U_datasheet.pdf)           | P-MOSFET pour power-path management        |
| **EEMB LP402535** | [`EEMB_LP402535_datasheet.pdf`](EEMB_LP402535_datasheet.pdf) | Batterie LiPo 320mAh                       |

## 📊 Caractéristiques Clés

### ATtiny212

- **Architecture** : AVR 8-bit RISC
- **Flash** : 2KB
- **RAM** : 128 bytes
- **EEPROM** : 64 bytes
- **Fréquence** : jusqu'à 20 MHz
- **Alimentation** : 1.8V - 5.5V
- **Programmation** : UPDI

### WS2812B

- **Type** : LED RGB intégrée avec driver
- **Alimentation** : 5V ± 0.5V
- **Courant** : ~60mA max par LED
- **Interface** : Signal digital unique
- **Protocole** : PWM temporisé

### TP4056

- **Type** : Contrôleur de charge Li-Ion/LiPo
- **Tension charge** : 4.2V ± 1%
- **Courant** : Programmable via résistance (jusqu'à 1A)
- **Protection** : Surtension, surintensité, thermique
- **Package** : SOP-8

### FP6277

- **Type** : Boost converter synchrone
- **Tension entrée** : 0.9V - 5V
- **Tension sortie** : 5V fixe
- **Courant** : jusqu'à 1.5A
- **Efficacité** : >90%
- **Fréquence commutation** : 1.2MHz

### DMP1045U

- **Type** : P-Channel Enhancement MOSFET
- **VDS max** : -30V
- **ID max** : -4.2A
- **RDS(on)** : 45mΩ @ -4.5V
- **Package** : SOT-23

### EEMB LP402535

- **Capacité nominale** : 320mAh
- **Tension nominale** : 3.7V
- **Courant décharge max** : 2C (640mA)
- **Courant recommandé** : 1C (320mA)
- **Cycles** : >500 cycles
- **Dimensions** : 35×25×4.0mm

## 🔧 Notes de Conception

### Courant Total Estimé

- 8 LEDs × 60mA = 480mA max
- ATtiny212 = ~5mA
- **Total** : ~485mA (1.5C de la batterie)

⚠️ **Attention** : Prévoir limitation logicielle pour rester sous 1C (320mA) en usage continu

### Efficacité Boost

- Perte boost converter ~10%
- Courant batterie estimé : 485mA / 0.9 ≈ 540mA
- **Autonomie estimée** : 320mAh / 540mA ≈ 35 minutes à pleine puissance

### Recommandations

1. Limiter brightness des LEDs (PWM ~50%)
2. Patterns optimisés (pas toutes LEDs simultanément)
3. Sleep mode agressif quand inactif

## 📁 Organisation

```
docs/datasheets/
├── ATtiny212_datasheet.pdf      # MCU principal
├── WS2812B_datasheet.pdf        # LEDs RGB
├── TP4056_datasheet.pdf         # Chargeur LiPo
├── FP6277_datasheet.pdf         # Boost 5V
├── DMP1045U_datasheet.pdf       # Power-path MOSFET
├── EEMB_LP402535_datasheet.pdf  # Batterie LiPo
└── README.md                    # Ce fichier
```

---

**Dernière mise à jour** : 15 août 2025  
**Status** : Datasheets complètes ✅
