# 🔥 Status Projet Ignis

**Date de mise à jour** : 15 août 2025  
**Phase actuelle** : Setup workspace et architecture de base

## ✅ Réalisé

### 🏗️ Workspace & Configuration

- [x] Workspace VS Code configuré avec dossiers structurés
- [x] Extension PlatformIO installée et fonctionnelle
- [x] Configuration PlatformIO pour ATtiny212/412
- [x] Tasks VS Code pour build/upload/monitor
- [x] Fichiers de configuration centralisés (`config.h`)

### 📚 Documentation

- [x] Datasheets téléchargées pour tous les composants
- [x] README technique dans `/docs/datasheets/`
- [x] Structure de documentation organisée
- [x] Copilot instructions intégrées au projet

### 💾 Firmware

- [x] Squelette minimal compilable
- [x] Configuration pins de base
- [x] LED status fonctionnelle
- [x] Structure modulaire prête pour extensions

## 🚧 En cours / Prochaines étapes

### Phase 1 : Validation Hardware

- [ ] Test du squelette sur hardware réel (ATtiny212 + LED)
- [ ] Validation programmation UPDI
- [ ] Test base bouton avec pull-up

### Phase 2 : Contrôle LEDs

- [ ] Réintégration FastLED (après résolution problème config)
- [ ] Test simple allumage WS2812B
- [ ] Implémentation patterns de base (heartbeat, fade)
- [ ] Limitation courant pour protection batterie

### Phase 3 : Power Management

- [ ] Implémentation sleep mode avec réveil par interruption
- [ ] Optimisation consommation (désactivation modules)
- [ ] Gestion timeout inactivité
- [ ] Tests autonomie batterie

### Phase 4 : Hardware Complet

- [ ] Conception schéma électrique PCB
- [ ] Intégration TP4056 + FP6277 + DMP1045U
- [ ] Power-path management
- [ ] Protection batterie

### Phase 5 : Patterns Avancés

- [ ] Patterns flamme réalistes (chenillard, random, scintillement)
- [ ] Transitions fluides entre patterns
- [ ] Détection socle pour bouton
- [ ] Interface utilisateur complète

## 📋 Architecture Technique Validée

```
USB 5V → TP4056 → DMP1045U → FP6277 → 5V système
                     ↓
                 Batterie LiPo 320mAh
                     ↓
         ATtiny212 @ 20MHz ← Bouton (PA1)
             ↓
         WS2812B × 8 LEDs (PA0)
             ↓
         LED Status (PA2)
```

## 🔧 Configuration Actuelle

- **MCU** : ATtiny212 @ 20MHz, framework Arduino
- **Pins** :
  - PA0 : WS2812B strip (futur)
  - PA1 : Bouton avec pull-up
  - PA2 : LED status/debug
- **Toolchain** : PlatformIO + jtag2updi
- **Debug** : Serial @ 9600 baud (si activé)

## ⚠️ Points d'attention

1. **Courant batterie** : 8 LEDs = 485mA (1.5C), prévoir limitation logicielle
2. **FastLED** : Problème de configuration à résoudre avant réintégration
3. **Sleep mode** : Timer0 désactivé peut affecter millis(), à tester
4. **UPDI** : Programmateur externe nécessaire (jtag2updi + /dev/ttyUSB0)

## 🎯 Objectifs Immédiats

1. **Tester hardware** : Valider squelette sur ATtiny212 réel
2. **Résoudre FastLED** : Configuration pour ATtiny212
3. **Premier pattern** : LED simple qui pulse/clignote
4. **Power test** : Mesure consommation réelle

## 📊 Métriques Projet

- **Lignes de code** : ~35 (squelette minimal)
- **Datasheets** : 6/6 téléchargées (100%)
- **Taux compilation** : 100% (squelette)
- **Configuration** : Complète

---

**Next milestone** : Hardware validation + FastLED integration
