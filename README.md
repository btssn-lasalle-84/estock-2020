# Le projet ESTOCK 2020

- [Le projet ESTOCK 2020](#le-projet-estock-2020)
  - [Auteurs](#auteurs)
  - [Présentation](#présentation)
  - [Base de données MySQL](#base-de-données-mysql)
  - [Recette](#recette)
  - [Productions](#productions)
  - [Licence GPL](#licence-gpl)

e-stock est un système de gestion de stock automatisé qui permettra :

* de contrôler et gérer l'utilisation de produits stockés dans une armoire sensible
* d'assurer la traçabilité de l'attribution du matériel et des consommables stockés
* de sécuriser l'accès par un contrôle d'accès par badge RFID

## Auteurs

- Pierre-Antoine Legger <pierreantoinelegger@gmail.com>
- Joffrey Tranchat <joffrey.tranchat@gmail.com>

## Présentation

__e-stock__ est un système de gestion de stock automatisé qui permettra :

* de contrôler et gérer l'utilisation de produits stockés dans une armoire sensible
* d'assurer la traçabilité de l'attribution du matériel et des consommables stockés
* de sécuriser l'accès par un contrôle d'accès par badge RFID

Une armoire sera composée de 8 casiers maximum. Chaque casier sera équipé :

* d'une gâche électrique afin d'assurer son ouverture/fermeture ;
* d'une balance pour assurer le comptage automatique des articles.

Le comptage automatique de la quantité est déterminé en fonction du poids unitaire et du poids mesuré sur la balance.

Un lecteur de badge RFID est intégré à chaque armoire pour contrôler l'accès. L'exploitation de l'armoire ​ e-stock​ est possible à partir de l'écran tactile intégré.

On distinguera deux type d'articles :

* les « consommables » qui sortent définitivement du stock
* les « empruntables » qui peuvent être restitués après leur utilisation

## Base de données MySQL

![](./sql/e-stock-v0.4.png)

```sql
DROP DATABASE IF EXISTS `e-stock`;
CREATE DATABASE IF NOT EXISTS `e-stock`;
USE `e-stock`;

-- Création du compte d'accès à la base de données e-stock
-- CREATE USER 'estock'@'%' IDENTIFIED BY 'password';
-- GRANT ALL PRIVILEGES ON `e-stock`.* TO 'estock'@'%';
-- FLUSH PRIVILEGES;
-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Profil` (
  `idProfil` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idProfil`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Profil` (`Nom`) VALUES
('Administrateur'),
('Gestionnaire'),
('Utilisateur');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Groupe` (
  `idGroupe` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idGroupe`),
  CONSTRAINT Unique_Groupe UNIQUE (`Nom`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `Groupe` (`Nom`) VALUES
('PROFESSEUR'),
('1-BTS-SN'),
('T-BTS-SN');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Utilisateur` (
  `idUtilisateur` int(11) NOT NULL AUTO_INCREMENT,
  `idProfil` int(11) NOT NULL,
  `idGroupe` int(11) NOT NULL,
  `Nom` varchar(64) NOT NULL,
  `Prenom` varchar(64) NOT NULL,
  `DateValidite` date NOT NULL,
  `Identifiant` varchar(255) DEFAULT NULL,
  `MotDePasse` varchar(255) DEFAULT NULL,
  `Badge` varchar(11) NOT NULL,
  `Email` varchar(64) NOT NULL,  
  PRIMARY KEY (`idUtilisateur`),
--  CONSTRAINT Unique_Utilisateur UNIQUE (`Badge`),
  CONSTRAINT Utilisateur_fk_1 FOREIGN KEY (`idProfil`) REFERENCES Profil(`idProfil`) ON DELETE CASCADE,
  CONSTRAINT Utilisateur_fk_2 FOREIGN KEY (`idGroupe`) REFERENCES Groupe(`idGroupe`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Armoire` (
  `idArmoire` int(11) NOT NULL,
  `Nom` varchar(255) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `nbCasiers` int(11) NOT NULL DEFAULT 8,
  PRIMARY KEY (`idArmoire`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Armoire` (`idArmoire`,`Nom`,`Description`,`nbCasiers`) VALUES('1','B22','Atelier','2');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Type` (
  `idType` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Type` (`Nom`) VALUES
('Equipement'),
('Consommable');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Unite` (
  `idUnite` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idUnite`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Unite` (`Nom`) VALUES
('Metre'),
('Piece'),
('Pourcentage'),
('Poids g'),
('Poids kg');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Comptage` (
  `idComptage` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idComptage`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Comptage` (`Nom`) VALUES
('Aucun'),
('Automatique'),
('CodeBarre');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Action` (
  `idAction` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idAction`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Action` (`Nom`) VALUES
('Entree'),
('Sortie');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Article` (
  `idArticle` int(11) NOT NULL AUTO_INCREMENT,
  `idType` int(11) NOT NULL,  
  --   `Type` enum('Equipement','Consommable'),
  `Nom` varchar(255) NOT NULL,
  `Code` varchar(255) NOT NULL,
  `Designation` varchar(255) NOT NULL,
  `Poids` int(11) NOT NULL,  
  PRIMARY KEY (`idArticle`),
  CONSTRAINT Article_fk_1 FOREIGN KEY (`idType`) REFERENCES Type(`idType`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Stock` (
  `idStock` int(11) NOT NULL AUTO_INCREMENT,
  `idArticle` int(11) NOT NULL,
  `idComptage` int(11) NOT NULL,
  `idUnite` int(11) NOT NULL,
  `Quantite` int(11) DEFAULT 0,
  `Disponible` int(11) DEFAULT 0,
  `Tare` int(11) NOT NULL,  
  `numeroCasier` int(11) NOT NULL,
  PRIMARY KEY (`idStock`),
  CONSTRAINT Unique_NumeroCasier UNIQUE (`numeroCasier`),
  CONSTRAINT Stock_fk_2 FOREIGN KEY (`idArticle`) REFERENCES Article(`idArticle`) ON DELETE CASCADE,
  CONSTRAINT Stock_fk_3 FOREIGN KEY (`idComptage`) REFERENCES Comptage(`idComptage`) ON DELETE CASCADE,
  CONSTRAINT Stock_fk_4 FOREIGN KEY (`idUnite`) REFERENCES Unite(`idUnite`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Mouvement` (
  `idMouvement` int(11) NOT NULL AUTO_INCREMENT, 
  `idUtilisateur` int(11) NOT NULL,
  `idStock` int(11) NOT NULL,
  `idAction` int(11) NOT NULL,
  --   `Action` enum('Entree','Sortie'),
  `Quantite` int(11) NOT NULL,
  `Horodatage` datetime NOT NULL,
  PRIMARY KEY (`idMouvement`),
  CONSTRAINT Mouvement_fk_1 FOREIGN KEY (`idUtilisateur`) REFERENCES Utilisateur(`idUtilisateur`) ON DELETE CASCADE,
  CONSTRAINT Mouvement_fk_2 FOREIGN KEY (`idStock`) REFERENCES Stock(`idStock`) ON DELETE CASCADE,
  CONSTRAINT Mouvement_fk_3 FOREIGN KEY (`idAction`) REFERENCES Action(`idAction`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

## Recette

- Pierre-Antoine Legger
    * S'authentifier
    * Rechercher un article
    * Consulter le stock
    * Communiquer avec le SE pour :
        * Commander l'ouverture/fermeture des casiers
        * Afficher l'état ouvert/fermé des casiers

- Joffrey Tranchat
    * Prendre et rapporter un article
    * Mettre à jour le stock et les mouvements
    * Consulter les mouvements
    * Communiquer avec le SE pour :
    * Récupérer les pesées des casiers
    * Assurer le comptage automatique

## Productions

- [dossier-estock-ir-2020.pdf](dossier-estock-ir-2020.pdf)
- [refman-estock.pdf](refman-estock.pdf)
- [presentation-legger-2020.pptx](presentation-legger-2020.pptx)
- [presentation-tranchat-2020.pptx](presentation-tranchat-2020.pptx)

## Licence GPL

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
