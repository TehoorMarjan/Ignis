# 🧊 STL & Exports Imprimables - Projet Ignis

Ce dossier regroupe les fichiers STL prêts pour l'impression 3D, issus des modèles artistiques du projet "Flamme dans le sable".

## 📚 Contexte

Les modèles originaux sont créés sous Blender (`art/<model>.blend`), puis exportés en STL (`art/<model>.stl`). Pour garantir une impression fiable et des unités correctes, les fichiers STL sont retravaillés sous OrcaSlicer :

- **Import STL Blender**
- **Conversion unités en mm**
- **Simplification et nettoyage du maillage**
- **Export final** : `stl/<model>.stl`

Pour plus d'informations sur le contexte artistique, voir le [README du dossier art](../art/README.md).

## 🛠️ Workflow d'export

1. **Modélisation** : Blender (`art/<model>.blend`)
2. **Export STL** : Blender (`art/<model>.stl`)
3. **Nettoyage** : OrcaSlicer (`stl/<model>.stl`)
   - Conversion unités
   - Simplification du maillage
   - Vérification de l'intégrité

> Un remaillage avancé sous Blender serait probablement plus puissant, mais OrcaSlicer offre une solution rapide et efficace pour préparer les fichiers à l'impression.

## 📁 Organisation

- `stl/Flamme1.stl` : Export final du design Flamme1
- `stl/Flamme2.stl` : Export final du design Flamme2

---

**Pour toute information sur les modèles originaux ou le contexte artistique, consultez le README du dossier art.**
