# Projet Ignis - Instructions Copilot

Ce projet concerne le développement d'une lampe/veilleuse autonome "Lampe Ignis" (lampe en forme de flamme dont le socle contient du sable) avec les spécifications suivantes :

## Architecture Technique

- **Microcontrôleur** : ATtiny212/ATtiny412 (ultra-low-power, 5V, UPDI)
- **LEDs** : WS2812B (6-8 LEDs, contrôle digital)
- **Batterie** : LiPo 320-500 mAh (EEMB LP402535 recommandée)
- **Charge** : TP4056 avec power-path management
- **Boost** : FP6277 pour sortie 5V stable
- **Switch** : DMP1045U P-MOSFET pour power-path

## Structure du Projet

- `firmware/` : Code PlatformIO pour ATtiny212
- `hardware/` : Schémas électriques et PCB
- `docs/` : Documentation technique et datasheets
- `mechanical/` : Conception 3D du boîtier

## Contraintes Électriques

- Courant max batterie : 320mA (1C) recommandé, 640mA (2C) absolu
- LEDs : ~60mA max par LED, PWM pour économie d'énergie
- Autonomie cible : 2-3 heures

## Développement

- Toolchain : PlatformIO sur Linux
- Programmation : UPDI pour ATtiny212
- Patterns LED : heartbeat, chenillard, fade, random
- Power management : sleep modes pour économie batterie


## Convention de Commit

- Tous les messages de commit doivent commencer par une emoji parlante, amusante ou sympa, si possible différente des précédents (éviter de réutiliser les deux dernières emoji utilisées, sauf si le contexte l'impose).
- Le message doit être rédigé **en anglais** pour assurer la cohérence du projet.
- Le titre doit décrire simplement les changements apportés, sans être exhaustif sur les détails mineurs pour ne pas surcharger la relecture. Privilégier un ton humain et synthétique.
- Pour les changements complexes, utiliser des **bullet points** dans le corps du message pour détailler les modifications de manière claire et organisée.
- Exemple de format :
  ```
  🔧 Add component with footprint and 3D model
  
  - Created KiCAD footprint for new component
  - Designed 3D model in FreeCAD
  - Integrated STEP model into library
  ```

✅ Clarify Project Requirements - Projet embarqué ATtiny212 avec LEDs WS2812B
