# Firmware Ignis - Lampe Ignis

## État actuel du projet

### ✅ Fonctionnalités implémentées

- **Squelette de base** : Structure du firmware avec boucle principale
- **Power Management** : Gestion du mode sleep avec réveil par interruption
- **Gestion bouton** : Détection d'appui avec debounce et interruptions
- **LED de status** : Heartbeat simple pour debug et feedback visuel
- **Configuration ATtiny212/412** : PlatformIO configuré pour les deux variants

### 🚧 À implémenter

- **LEDs WS2812B** : Contrôle de la strip de LEDs (FastLED)
- **Patterns lumineux** : Heartbeat, chenillard, fade, random
- **Optimisations power** : Fine-tuning du sleep mode
- **Détection socle** : Logic pour savoir si sur station de charge

## Structure du firmware

```
firmware/
├── platformio.ini          # Configuration PlatformIO
├── src/
│   └── main.cpp            # Firmware principal
├── include/                # Headers (vide pour l'instant)
├── lib/                    # Librairies locales (vide)
└── test/                   # Tests unitaires (à venir)
```

## Configuration matérielle

- **Microcontrôleur** : ATtiny212 ou ATtiny412 @ 20MHz
- **Pin LED strip** : PA0 (LED_PIN = 0)
- **Pin bouton** : PA1 (BUTTON_PIN = 1) avec pull-up interne
- **Pin status LED** : PA2 (STATUS_LED_PIN = 2) pour debug
- **Programmation** : UPDI via jtag2updi

## Power Management

Le firmware implémente un système de gestion d'énergie :

1. **Mode actif** : LED status clignote, écoute du bouton
2. **Timeout inactivité** : 30 secondes sans interaction
3. **Mode sleep** : `SLEEP_MODE_PWR_DOWN` (le plus économe)
4. **Réveil** : Interruption sur changement d'état du bouton (PCINT)

### Modules désactivés en sleep

- ADC (power_adc_disable)
- SPI (power_spi_disable)
- Timer0/Timer1 (peut affecter millis())
- Tous réactivés au réveil (power_all_enable)

## Compilation

```bash
# Via VS Code PlatformIO
Ctrl+Shift+P -> "PlatformIO: Build"

# Ou via terminal (si pio installé)
cd firmware/
pio run
```

## Upload/Debug

Configuration UPDI dans `platformio.ini` :

- Port : `/dev/ttyUSB0` (à adapter)
- Protocol : `jtag2updi`
- Vitesse : 115200 baud

## Prochaines étapes

1. **Tester le squelette** sur hardware réel
2. **Ajouter FastLED** et contrôle WS2812B
3. **Implémenter patterns** lumineux
4. **Optimiser consommation** électrique
5. **Tests d'autonomie** batterie

## Debug

- `#define DEBUG 1` dans platformio.ini pour traces Serial
- LED status clignote = système actif
- 3 clignotements rapides = bouton pressé
- 1 clignotement long au réveil = sortie de sleep
