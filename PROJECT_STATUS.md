# 🔥 Status Projet Ignis

**Date de mise à jour** : 1 septembre 2025  
**Phase actuelle** : IgnisV2 PCB finalisé - Production lancée chez JLCPCB

## ✅ Réalisé

### 🏗️ Workspace & Configuration

- [x] Workspace VS Code configuré avec dossiers structurés
- [x] Extension PlatformIO installée et fonctionnelle
- [x] Configuration PlatformIO pour ATtiny212/412
- [x] Tasks VS Code pour build/upload/monitor
- [x] Fichiers de configuration centralisés (`config.h`)

### 📚 Documentation & Branding

- [x] Datasheets téléchargées pour tous les composants (BQ24073, 74LVC1G17,
      WS2812B, etc.)
- [x] Documentation technique complète avec calculs électriques détaillés
- [x] Calculs de filament pour l'impression 3D des boîtiers
- [x] Structure de documentation organisée et formatée
- [x] Copilot instructions intégrées au projet
- [x] **Logo TheoToys finalisé** avec police Dongle Bold
- [x] **Système automatisé** de génération des variantes de logo (Makefile)
- [x] **Rebranding complet** des worksheets KiCAD

### 🔌 Hardware - IgnisV2 PCB

- [x] **Schéma électrique finalisé** avec architecture BQ24073 + BQ51013B
- [x] **PCB layout complet** avec optimisation JLCPCB
- [x] **Composants sourcés** et vérifiés sur JLCPCB (disponibilité + footprints)
- [x] **Migration MCP73871 → BQ24073** pour meilleure intégration
- [x] **Test points ajoutés** pour debug et validation
- [x] **Solder jumpers** pour corrections post-fabrication
- [x] **Placeholders composants** pour tweaking sans nouveau PCB
- [x] **Fichiers de production générés** (Gerbers, BOM, P&P)
- [x] **Commande passée chez JLCPCB** (advanced PCBA pour WS2812B)
- [x] **Issues critiques résolues** : vias <0.3mm, pinout WS2812B-2020

### 🎨 Mécanique & 3D

- [x] **Modèles 3D de visualisation** : PCB, batterie, bobine de charge
- [x] **Flamme4 avec design dual-wall** créée dans Blender
- [x] **STL exportés** pour impression (outer + inner shell)
- [x] **Modèles CAD FreeCAD** pour l'assemblage complet
- [x] **SW-18030 footprint + modèle 3D** créés et intégrés
- [x] **Correction des couleurs STEP** pour KiCAD (éviter #000000 transparent)

### 💾 Firmware

- [x] Squelette minimal compilable
- [x] Configuration pins de base
- [x] LED status fonctionnelle
- [x] Structure modulaire prête pour extensions

## 🚧 En cours / Prochaines étapes

### Phase 1 : Attente Production PCB ⏳

- [ ] **Réception PCBs JLCPCB** (délai estimé : 2-3 semaines)
- [ ] **Contrôle qualité** des PCBs assemblés
- [ ] **Validation assemblage PCBA** (composants bien soudés)
- [ ] **Tests électriques préliminaires** (continuité, alimentations)

### Phase 2 : Validation Hardware

- [ ] **Premier power-on** avec alimentation externe
- [ ] **Test chargeur sans fil** (BQ51013B + bobine IWAS3010)
- [ ] **Validation power-path** (USB/Qi vers batterie)
- [ ] **Test boost converter** (FP6277 : 3.7V → 5V)
- [ ] **Programmation UPDI** sur PCB final
- [ ] **Test SW-18030** (détecteur de choc avec filtre RC)

### Phase 3 : Firmware sur Hardware Réel

- [ ] **Port du firmware** sur IgnisV2 PCB
- [ ] **Test WS2812B-2020** (vérification pinout corrigé)
- [ ] **Patterns LED de base** (heartbeat, fade, chenillard)
- [ ] **Gestion power management** (sleep modes)
- [ ] **Détection choc** pour réveil/mode interactif

### Phase 4 : Intégration Mécanique

- [ ] **Impression 3D** des boîtiers Flamme4
- [ ] **Assemblage complet** PCB + batterie + boîtier
- [ ] **Test d'étanchéité** (charge sans fil)
- [ ] **Validation ergonomique** et esthétique
- [ ] **Tests de durabilité** (cycles charge/décharge)

### Phase 5 : Finalisation Produit

- [ ] **Patterns flamme avancés** (simulation réaliste)
- [ ] **Interface utilisateur** (détection socle, modes)
- [ ] **Optimisation autonomie** (2-3h d'éclairage)
- [ ] **Documentation utilisateur** finale
- [ ] **Préparation petite série** si validation OK

## 📋 Architecture Technique IgnisV2

```
Chargeur Qi (BQ51013B) → BQ24073 → Batterie LiPo 320mAh
                            ↓
                     Power-path management
                            ↓
                       FP6277 Boost → 5V système
                            ↓
                    ATtiny212 @ 20MHz
                       ↙     ↓     ↘
               SW-18030    WS2812B    LED Status
              (réveil)   (×6-8 LEDs)   (debug)
```

### 🔧 Composants Finaux Sélectionnés

- **MCU** : ATtiny212-SSN (SOIC-8, compatible JLCPCB basic)
- **Chargeur** : BQ24073RGTR (Texas Instruments, advanced PCBA)
- **Récepteur Qi** : BQ51013B + bobine IWAS3010AZEB130KF1
- **Boost** : FP6277XR-G1 (3.7V→5V, 1A capable)
- **LEDs** : WS2812B-2020 (6-8 pcs, pinout corrigé)
- **Détecteur choc** : SW-18030 + 74LVC1G17 (filtre RC intégré)
- **Batterie** : EEMB LP402535 (320mAh, protection intégrée)

## 🔧 Configuration Actuelle

- **MCU** : ATtiny212 @ 20MHz, framework Arduino
- **Pins** :
  - PA0 : WS2812B strip (futur)
  - PA1 : Bouton avec pull-up
  - PA2 : LED status/debug
- **Toolchain** : PlatformIO + jtag2updi
- **Debug** : Serial @ 9600 baud (si activé)

## ⚠️ Points d'attention & Lessons Learned

### 🚨 Issues Critiques Résolues

1. **WS2812B pinout** : WS2812B standard ≠ WS2812B-2020 (pins différents) ✅
   Corrigé
2. **Vias undersized** : <0.3mm = surcoût JLCPCB ✅ Redimensionnés
3. **Couleurs STEP** : #000000 = transparent dans KiCAD ✅ Modifié vers #010101+

### ⚡ Design Électrique

1. **Courant batterie** : 8 LEDs = 485mA (1.5C), limitation logicielle requise
2. **Charge Qi** : Efficacité ~70%, prévoir marge thermique
3. **Power-path** : BQ24073 gère automatiquement priorité Qi vs batterie
4. **Test points** : Indispensables pour debug hardware complexe

### 🏭 Production JLCPCB

1. **Advanced PCBA** : Nécessaire pour WS2812B (backing requis)
2. **Sourcing** : Vérifier dispo composants avant finalisation
3. **DRC** : Rules JLCPCB intégrées dès le design
4. **Solder jumpers** : Sécurité pour corrections post-fab

## 🎯 Objectifs Immédiats

### 📦 Attente Livraison (2-3 semaines)

1. **Surveiller commande JLCPCB** : Progression fabrication + assemblage
2. **Préparer banc test** : Alimentation, oscilloscope, multimètre
3. **Finaliser programmateur UPDI** : Breadboard temporaire OK
4. **Préparation firmware test** : Version minimale pour validation

### 🔬 Dès Réception PCB

1. **Inspection visuelle** : Qualité soudure, alignement composants
2. **Tests électriques** : Continuité, court-circuits, tensions
3. **Premier power-on** : Charge Qi + mesure consommations
4. **Programming** : Upload firmware test via UPDI

## 📊 Métriques Projet

- **Phase** : Production hardware (attente fabrication)
- **PCB** : IgnisV2 finalisé et commandé chez JLCPCB
- **Datasheets** : 8/8 composants documentés (100%)
- **Calculs** : Électriques + mécaniques + filament (complets)
- **Modèles 3D** : 4 variantes Flamme + modèles assemblage
- **Documentation** : Formatée et structurée
- **Budget** : ~50-70€ pour 5 PCBs assemblés (estimation)

---

**Next milestone** : Réception et validation IgnisV2 PCB ⏳
