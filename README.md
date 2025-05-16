# Projet d'intégration SIM H25

## Table des matières

1. [Présentation](#présentation)  
2. [Objectifs du projet](#objectifs-du-projet)  
3. [Matériel utilisé](#matériel-utilisé)  
4. [Architecture technique](#architecture-technique)  
5. [Installation et assemblage](#installation-et-assemblage)  
6. [Utilisation](#utilisation)  
7. [Description de l’algorithme de contrôle de traction](#description-de-lalgorithme-de-contrôle-de-traction)  
8. [Tests et résultats](#tests-et-résultats)  
9. [Améliorations supplémentaires](#améliorations-supplémentaires)
10. [Ressources supplémentaires](#ressources-supplémentaires)  

---

## Présentation

### Algorithme de contrôle de traction pour optimiser la performance et le pilotage d'un véhicule imprimé en 3D

Ce projet vise à concevoir une voiture téléguidée optimisée à l'aide d'un algorithme de contrôle de traction. Le véhicule, inspiré du design de la Formule 1, utilise un châssis imprimé en 3D et est équipé d'un accéléromètre, d'un gyroscope et d'un magnétomètre. L'architecture du projet est modulable et open-source, permettant une évolution facile et une personnalisation par la communauté. Ce projet a pour but d'améliorer l'adhérence en virage et d'optimiser la performance globale du véhicule à travers un contrôle précis du moteur en fonction des conditions de conduite.

---

## Objectifs du projet

Les objectifs principaux du projet sont les suivants :

- Concevoir un châssis rigide et léger via un logiciel de CAO, adapté à un modèle de véhicule téléguidé de type Formule 1.
- Imprimer en 3D et assembler un véhicule fonctionnel, prêt à être testé sur différentes surfaces et en virage.
- Intégrer des capteurs de mouvement (accéléromètre, gyroscope, magnétomètre) pour analyser la dynamique du véhicule en temps réel.
- Développer un algorithme de contrôle PID pour améliorer l'adhérence en virage et optimiser les performances du véhicule sur différents types de surface (asphalte, gravier, etc.).

---

## Matériel utilisé

La liste complète du matériel utilisé est disponible dans le fichier `BOM.xlsx`. Voici un aperçu du matériel principal :

- **Châssis** : Imprimé en 3D (modèle basé sur la Formule 1)
- **Capteurs** : MPU9250 (accéléromètre, gyroscope, magnétomètre)
- **Moteur** : Moteur à courant continu contrôlé par PWM
- **Microcontrôleur** : ESP32 (pour le contrôle des moteurs et la communication Wi-Fi)
- **Batterie** : Li-Po 7.4V pour alimenter les moteurs et la carte électronique
- **Module Wi-Fi** : ESP32 intégré pour la communication sans fil
- **Structure et composants supplémentaires** : Roues, suspensions, pièces imprimées en 3D, etc.

---

## Installation et assemblage

### Installation des logiciels et librairies

Le projet est développé à l'aide du framework **PlatformIO** pour le développement embarqué et des librairies Arduino.

1. **Installation de PlatformIO** : [https://platformio.org/install](https://platformio.org/install)  
2. **Installation de l'Arduino IDE** : [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
3. **Librairies nécessaires** :
   - `MPU9250` : Pour l'interface avec le capteur de mouvement.
   - `PID_v1` : Pour la gestion de l'algorithme PID.
   - `WiFi` : Pour la communication sans fil.
   #### Voir les [ressources supplémentaires](#ressources-supplémentaires) pour plus d'informations

---

## Architecture technique

### Modules principaux

1. **Acquisition des données**  
   - **Capteur MPU9250** : Lecture des valeurs d'accélération, de vitesse angulaire et de champ magnétique pour déterminer la position, la vitesse et l'orientation du véhicule. La combinaison des senseurs est éffectuée par la librairie [MPU9250](https://github.com/hideakitai/MPU9250)

2. **Traitement des données**  
   - **Filtrage et analyse** : Comparaison des données captées par les capteurs avec les objectifs de conduite de l'utilisateur (vitesse, angle de virage, etc.).
   - **Correction de l'entrée de l'utilisateur** : Utilisation des données pour ajuster les actions du système de contrôle PID.

3. **Contrôle PID du moteur**  
   - Réglage dynamique de la puissance moteur en fonction des entrées du système PID.

4. **Communication**  
   - **Transmission Wi-Fi** : Envoi des données de télémétrie à une page Web en temps réel pour visualiser les performances du véhicule.
   - **Envoi de la correction par I2C** : Communication bidirectionnelle pour envoyer des corrections à l'algorithme de contrôle du moteur en fonction des ajustements faits par l'utilisateur ou le système.
   - **Requête http**: Envoi des requêtes pour ajuster les paramètres de l'algorithme PID en temps réel.

5. **Interface utilisateur (UI)**  
   - **Graphique de télémétrie**: Affichage graphique de la puissance des moteurs en temps réel
   - **Réglage PID à distance** : Interface Web permettant de modifier les paramètres du PID en temps réel.
   - **Courbe d'analyse** : Affichage graphique des entrées et sorties du système PID pour analyser la performance du véhicule.

---

### Protocoles de communication

Le projet utilise plusieurs protocoles pour gérer les différents composants :

1. **PWM** : Utilisé pour contrôler la vitesse des moteurs en ajustant la largeur des impulsions électriques.
2. **I2C** : Utilisé pour la communication avec le capteur MPU9250. Établit la connection d'envoi binaire avec un appareil [slave](https://github.com/zon-n/prog4-slave)
3. **Wi-Fi** : Pour la communication sans fil entre le véhicule et la page Web pour afficher la télémétrie et ajuster les paramètres du système.
4. **HTTP** : Pour la requête de données et la calibration en temps réel à travers l'interface web. 
5. **Radio 2.4Ghz** : Communication des commandes d'utilisateurs à longue portée.

---

## Utilisation

Le véhicule est contrôlé à distance via une manette radio et par interface Web. L'utilisateur peut :

#### - Par radio

- Envoyer les commandes de mouvement à l'appareil

#### - Par interface Web
- Visualiser en temps réel les données télémétriques (vitesses, orientation, correction par PID)
- Ajuster les paramètres PID pour optimiser la performance du véhicule sur différentes surfaces.
- Observer les courbes d'analyse et les performances de l'algorithme PID.

---

## Description de l’algorithme de contrôle de traction

L'algorithme de contrôle de traction utilise un **PID** (Proportionnel-Intégral-Dérivé) pour réguler la vitesse des moteurs du véhicule et optimiser son adhérence pendant les virages. Voici un aperçu de son fonctionnement :

1. **Proportionnel** : La réponse est directement proportionnelle à l'écart entre la vitesse mesurée et la vitesse souhaitée.
2. **Intégral** : Permet de compenser les erreurs accumulées sur le temps pour corriger les déviations lentes.
3. **Dérivé** : Réagit aux changements rapides de l'écart, ajustant la puissance du moteur pour éviter les oscillations.

L'algorithme ajuste dynamiquement la puissance des moteurs en fonction de l'adhérence de la surface, de la vitesse et de l'angle de virage.

---

## Tests et résultats

### Tests effectués

- **Tests de vitesse** : Validation de la capacité du véhicule à atteindre et maintenir une vitesse constante.
- **Tests en virage** : Vérification de la performance du contrôle PID en fonction de différents angles de virage et types de surface.
- **Tests de contrôle par interface Web** : Vérification de la performance de la platforme Web hébergé sur le ESP32. L'accès local permet une connection facile à la page web

### Résultats obtenus

- Le télémétrie montre une déviation entre l'entrée d'utilisateur et la correction
- Le véhicule roule droit lors des test élémentaires
- La télémétrie en temps réel a permis de visualiser l'impact des réglages PID sur la performance du véhicule

### Améliorations supplémentaires
- **Calibration des consantes PID**: Jusqu'à maintenant la calibration des constantes proportionnelles, intégrales et dérivées ont été faites à la main. Dans certains cas, la performance était inférieure après le contrôle PID que sans. Il serait donc envisageable de développer un algorithme de calibration automatique pour trouver les constantes PID optimales pour des conditions variées. 
- **Incorporation de plus d'information télémétriques pour analyse

---

## Ressources supplémentaires

- **Documentation sur l'ESP32** : [https://docs.espressif.com/](https://docs.espressif.com/)
- **Documentation sur PlatformIO**: [https://platformio.org/](https://platformio.org/)
- **Docoumentation sur Arduino**: [https://www.arduino.cc/en/software/](https://www.arduino.cc/en/software/)

- **Hyperliens vers les répositoires Github des libraires utilisées**
   * [hideakitai/MPU9250@](https://github.com/hideakitai/MPU9250)
   * [br3ttb/PID@](https://github.com/br3ttb/Arduino-PID-Library)
   * [esp32async/AsyncTCP](https://github.com/ESP32Async/AsyncTCP)
   * [esp32async/ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer)
   * [arduino/ArduinoJSON](https://github.com/arduino-libraries/Arduino_JSON)
