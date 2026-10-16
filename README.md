# Calculateur d'itinéraire de métro en C

## Description

Ce projet est un programme développé en langage C qui permet de calculer le plus court chemin entre deux stations de métro à l'aide de l'algorithme de Dijkstra.

Le programme utilise un fichier `metro.txt` contenant les stations, les lignes de métro et les temps de trajet.

## Fonctionnalités

- Affichage des stations disponibles
- Recherche d'un itinéraire entre deux stations
- Calcul du temps de trajet minimal
- Affichage des lignes de métro empruntées
- Détection des correspondances
- Gestion des stations inconnues

## Technologies utilisées

- Langage C
- Algorithme de Dijkstra
- Fichiers texte

## Structure du projet

`metro-itineraire/

├── metro_itineraire.c

├── metro.txt

└── README.md`

## Compilation
Compiler le programme avec GCC :


bash
-gcc main.c -o metro

## Exécution
Sous Linux ou macOS :


bash
./metro

Sous Windows :


bash
metro.exe

Le fichier metro.txt doit être présent dans le répertoire d'exécution du programme.

## Format du fichier metro.txt
Les données sont organisées selon le format suivant :

station1;station2;ligne;temps

Chaque ligne représente une connexion entre deux stations, avec le numéro de la ligne de métro et le temps de trajet en minutes.

## Auteur
Projet réalisé par `Akretche Tarek` dans le cadre de l'apprentissage du langage C et des algorithmes de graphes.
