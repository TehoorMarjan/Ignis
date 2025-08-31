# Calculs Filament iSANMATE Mat PLA+ Gradient Sunset pour Lampe Ignis

## Objectif

Documenter les mesures et calculs liés à l'utilisation du filament iSANMATE Mat
PLA+ Gradient Sunset pour la fabrication de la lampe Ignis, notamment la
longueur de filament nécessaire pour obtenir un motif de gradient orange/brun à
jaune.

## Calibration

- **Type de filament** : PLA+ (couleurs : orange/brun à jaune, gradient sunset)
- **Marque** : iSANMATE
- **Finition** : Matte
- **Densité** : 1.24 g/cm³
- **Diamètre** : 1.75 mm

### Temperature

- **Méthode** : Temperature Tower
- **Résultat** : 190°C optimum (test "temperature tower" montre de très bons
  résultats entre 210°C et 180°C, choix difficile entre 200°C et 180°C)

### Débit

- **Méthode** : Flowrate YOLO
- **Résultat** : +0.02 (à rajouter au rapport de débit de base de 1.00)
  difficile d'identifier de sérieuses différences entre +0.00 et +0.03

### Débit (perfectionnist)

- **Méthode** : Flowrate YOLO (perfectionnist)
- **Résultat 1** : Surface difficile à interpréter, filament très stable, choix
  final : -0.04 (meilleure surface observée)
- **Remarque** : Correction de débit passant de 1.00 à 1.02 puis à 0.98, à
  l'inverse du test précédent.

### Débit (perfectionnist) 2

- **Méthode** : Flowrate YOLO (perfectionnist)
- **Résultat 2** : Plus facile à voir avec la spirale d'Archimède. Sélectionné
  -0.010, pour un total de 0.970.

### Pressure Advance

- **Méthode** : Tower
- **Paramètres**:
  - Type d'extrudeur: Bowden
  - Début : 0
  - Fin : 1
  - Pas : 0.02
- **Résultat** : Pas tellement de différence, prise à 1cm => 0.2

### Retraction

- **Méthode** : Dual Tower
- **Paramètres**:
  - Début : 2
  - Fin : 6
  - Pas : 0.1
- **Résultat** : Non concluant (mais excellente qualité)

### Retraction 2

- **Méthode** : Dual Tower
- **Paramètres**:
  - Début : 0
  - Fin : 3
  - Pas : 0.1
- **Résultat** : 20mm mesuré => 2mm de rétractation

## Mesures à effectuer

1. **Longueur totale du filament inséré avant impression**
2. **Longueur de filament consommée jusqu'à la première répétition du motif**
3. **Poids du filament utilisé (optionnel, pour vérification croisée)**

## Données d'utilisation du filament

- **Temperature Tower** :
  - Longueur de filament utilisée : 4.71m
  - Poids du filament utilisé : 14.04g
  - Couleur: 🟧 (apparemment le plus foncé, à vérifier)
- **Flowrate YOLO** :
  - Longueur de filament utilisée : 5.00m
  - Poids de filament utilisé : 14.91g
- **Flowrate YOLO (perfectionnist) 1** :
  - Longueur de filament utilisée : 7.30m
  - Poids de filament utilisé : 21.76g
- **Flowrate YOLO (perfectionnist) 2** :
  - Longueur de filament utilisée : 7.01m
  - Poids de filament utilisé : 20.91g
  - **Note** : La couleur jaune la plus claire a été atteinte durant ce test, à
    environ 3/4 de l'impression.
  - Couleur: 🟨 (le plus clair)
  - Pré-calcul :
    $1/2\,\text{motif} = 4.71,\text{m} + 5.00,\text{m} + 7.30,\text{m} + 7.01,\text{m} \times 75\,\% = 22,26\,\text{m}$
    soit $\text{motif} \approx 45\,\text{m}$
- **Pressure Advance** :
  - Longueur de filament utilisée : 3.80m
  - Poids de filament utilisé : 11.32g
- **Retraction**
  - Longueur de filament utilisée : 0.56m
  - Poids de filament utilisé : 1.66g
- **Retraction 2**
  - Longueur de filament utilisée : 0.46m
  - Poids de filament utilisé : 1.36g
- **Octopus v6**
  - Longueur de filament utilisée : 11.96m
  - Poids de filament utilisé : 35.66g
- **Flamme 4 x2**
  - Longueur de filament utilisée : 24.33m
  - Poids de filament utilisé : 72.57g
  - **Note** : Durant cette impression, la couleur la plus rouge a été atteinte,
    mais pas de retour au jaune. Le calcul prédictif n'était pas bon. Il est
    possible que la distance du rouge au jaune puis du jaune au rouge ne soit
    pas identique.
  - Couleur: 🟧
  - Distance totale : 65.13m
- **Cylindre Test**
  - Longueur de filament utilisée : 1.01m
  - Poids de filament utilisé : 3.02g
  - **Note** : Il semble que le cylindre ait la même couleur que l'item 2/7 de
    la Temperature Tower. On pourrait donc calculer la distance du reste: 5/7 de
    la Temperature Tower.
  - Pré-calcul :
    $\left(\frac{4}{7} \times 4.71\,\text{m}\right) + 5\,\text{m} + 7.30\,\text{m} + 7.01\,\text{m} + 3.80\,\text{m} + 0.56\,\text{m} + 0.46\,\text{m} + 11.96\,\text{m} + 24.33\,\text{m} + 1.01\,\text{m} = 64.121\,\text{m}$
  - Hypothèse : Le motif se répète tous les $65\,\text{m}$
  - Note : Quantité de filament utilisée jusqu'ici = 197.21g
- **Flamme 4 x4**
  - Longueur de filament utilisée : 48.79m
  - Poids de filament utilisé : 145.51g
