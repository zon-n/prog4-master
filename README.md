# Projet d'intégration SIM H25

Algorithme de contrôle de traction pour optimiser la performance et le pilotage d'un véhicule imprimé en 3D.

## Description du projet
Le projet a pour objectif de concevoir et d’optimiser une voiture RC. Basée sur un châssis de voiture de Formule 1 et intégrant un accéléromètre et gyroscope, ce dernier nous permettra de programmer un algorithme de contrôle de traction pour optimiser la performance et le pilotage d'un véhicule imprimé en 3D. Ce véhicule utilise une approche modulaire et open-source.

## Nos objectifs
* Concevoir et CAD un châssis rigide inspiré de la Formule 1.
* Imprimer en 3D et assembler le véhicule avec des composants pertinents.
* Incorporer un accéléromètre et gyroscope pour analyser la dynamique du véhicule.

* À l'aide des analyses recueillies, développer un algorithme de contrôle de traction afin d'améliorer le pilotage et la stabilité dans les virages.

## Matériel utilisé
* Capteurs : Accéléromètre Adafruit MPU6050
* Microcontrôleur : Raspberry Pi Pico RP2040
* Moteur : Brushless 3650 4000Kv
* Alimentation : Batterie LiPo 5200mAh
* Programmation: Framework Arduino avec C++ et Pico SDK
* Matériau d'impression 3D: ABS, ASA et PETG

## Algorithme de contrôle de traction
L’algorithme utilise les données capturées par notre accéléromètre pour détecter la perte de traction et ajuster adéquatement la puissance du moteur à travers un contrôle PID.

* Analyse des données : Capture de la vitesse angulaire en x, y, z et des accélérations.

* Détection du dérapage : Comparaison de la variation de l'accélération.

* Correction en temps réel : Ajustement de la puissance du moteur pour éviter le dérapage et regagner une adhérence optimale.

## Installation prérequis
Visual Studio Code avec PlatformIO
### Bibliothèques:
Adafruit MPU6050, Adafruit Sensor, Wire, Arduino, Servo

