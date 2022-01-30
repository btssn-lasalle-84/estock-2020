--
-- Base de données: `e-stock`
--

DROP DATABASE IF EXISTS `e-stock`;

CREATE DATABASE IF NOT EXISTS `e-stock`;

USE `e-stock`;

-- --------------------------------------------------------

-- Création du compte d'accès à la base de données e-stock

-- CREATE USER 'estock'@'%' IDENTIFIED BY 'password';
-- GRANT ALL PRIVILEGES ON `e-stock`.* TO 'estock'@'%';
-- FLUSH PRIVILEGES;

-- --------------------------------------------------------

--
-- Structure de la table `Profil`
--

CREATE TABLE IF NOT EXISTS `Profil` (
  `idProfil` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idProfil`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `Profil` (`Nom`) VALUES
('Administrateur'),
('Gestionnaire'),
('Utilisateur');

-- --------------------------------------------------------

--
-- Structure de la table `Groupe`
--

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

--
-- Structure de la table `Utilisateur`
--

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
  CONSTRAINT Unique_Utilisateur UNIQUE (`Badge`),
  CONSTRAINT Utilisateur_fk_1 FOREIGN KEY (`idProfil`) REFERENCES Profil(`idProfil`) ON DELETE CASCADE,
  CONSTRAINT Utilisateur_fk_2 FOREIGN KEY (`idGroupe`) REFERENCES Groupe(`idGroupe`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Utilisateur` (`idProfil`,`idGroupe`,`Nom`,`Prenom`,`DateValidite`,`Identifiant`,`MotDePasse`,`Badge`,`Email`) VALUES('1','1','Vaira','Thierry','2020-07-01','admin','5f4dcc3b5aa765d61d8327deb882cf99','','tvaira@free.fr');
INSERT INTO `Utilisateur` (`idProfil`,`idGroupe`,`Nom`,`Prenom`,`DateValidite`,`Identifiant`,`MotDePasse`,`Badge`,`Email`) VALUES('2','1','Vaira','Thierry','2020-07-01','tvaira','5f4dcc3b5aa765d61d8327deb882cf99','','tvaira@free.fr');
INSERT INTO `Utilisateur` (`idProfil`,`idGroupe`,`Nom`,`Prenom`,`DateValidite`,`Identifiant`,`MotDePasse`,`Badge`,`Email`) VALUES('2','1','Beaumont','Jerome','2020-07-01','jbeaumont','','','beaumont@lasalle84.org');
INSERT INTO `Utilisateur` (`idProfil`,`idGroupe`,`Nom`,`Prenom`,`DateValidite`,`Identifiant`,`MotDePasse`,`Badge`,`Email`) VALUES('3','3','LEGGER','Pierre-Antoine','2020-07-01','legger.pa','','5022A783','');
INSERT INTO `Utilisateur` (`idProfil`,`idGroupe`,`Nom`,`Prenom`,`DateValidite`,`Identifiant`,`MotDePasse`,`Badge`,`Email`) VALUES('3','3','TRANCHAT','Joffrey','2020-07-01','tranchat.j','','02BE0261','');
INSERT INTO `Utilisateur` (`idProfil`,`idGroupe`,`Nom`,`Prenom`,`DateValidite`,`Identifiant`,`MotDePasse`,`Badge`,`Email`) VALUES('3','3','BOUNOIR','Fabien','2020-07-01','bounoir.f','','30DDA983','');
INSERT INTO `Utilisateur` (`idProfil`,`idGroupe`,`Nom`,`Prenom`,`DateValidite`,`Identifiant`,`MotDePasse`,`Badge`,`Email`) VALUES('3','3','DEVINE','Vincent','2020-07-01','devine.v','','62A3F560','');

-- --------------------------------------------------------

--
-- Structure de la table `Armoire`
--

CREATE TABLE IF NOT EXISTS `Armoire` (
  `idArmoire` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(255) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `AdresseIP` varchar(15) DEFAULT NULL,
  `nbCasiers` int(11) NOT NULL DEFAULT 8,
  PRIMARY KEY (`idArmoire`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Armoire` (`Nom`,`Description`,`AdresseIP`,`nbCasiers`) VALUES('Armoire C01','','192.168.52.1','2');
INSERT INTO `Armoire` (`Nom`,`Description`,`AdresseIP`,`nbCasiers`) VALUES('Armoire C02','','192.168.52.2','2');
INSERT INTO `Armoire` (`Nom`,`Description`,`AdresseIP`,`nbCasiers`) VALUES('Armoire A01','','192.168.52.3','2');

-- --------------------------------------------------------

--
-- Structure de la table `Type`
--

CREATE TABLE IF NOT EXISTS `Type` (
  `idType` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `Type` (`Nom`) VALUES
('Equipement'),
('Consommable');

-- --------------------------------------------------------

--
-- Structure de la table `Unite`
--

CREATE TABLE IF NOT EXISTS `Unite` (
  `idUnite` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idUnite`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `Unite` (`Nom`) VALUES
('Metre'),
('Piece'),
('Pourcentage'),
('Poids g'),
('Poids kg');

-- --------------------------------------------------------

--
-- Structure de la table `Comptage`
--

CREATE TABLE IF NOT EXISTS `Comptage` (
  `idComptage` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idComptage`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `Comptage` (`Nom`) VALUES
('Aucun'),
('Automatique'),
('CodeBarre');

-- --------------------------------------------------------

--
-- Structure de la table `Action`
--

CREATE TABLE IF NOT EXISTS `Action` (
  `idAction` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idAction`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `Action` (`Nom`) VALUES
('Entree'),
('Sortie');

-- --------------------------------------------------------

--
-- Structure de la table `Article`
--

CREATE TABLE IF NOT EXISTS `Article` (
  `idArticle` int(11) NOT NULL AUTO_INCREMENT,
  `idType` int(11) NOT NULL,  
  --   `Type` enum('Equipement','Consommable'),
  `Nom` varchar(255) NOT NULL,
  `Code` varchar(255) NOT NULL,
  `Designation` varchar(255) NOT NULL,
  PRIMARY KEY (`idArticle`),
  CONSTRAINT Article_fk_1 FOREIGN KEY (`idType`) REFERENCES Type(`idType`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Article` (`idType`, `Nom`, `Code`, `Designation`) VALUES('2','Vis six pans creux M2 8mm','','M2*8');
INSERT INTO `Article` (`idType`, `Nom`, `Code`, `Designation`) VALUES('2','Vis tête cylindrique M2 8mm','','M2*8');
INSERT INTO `Article` (`idType`, `Nom`, `Code`, `Designation`) VALUES('2','Vis six pans creux M2 12mm','','M2*12');
INSERT INTO `Article` (`idType`, `Nom`, `Code`, `Designation`) VALUES('2','Vis tête cylindrique M2 12mm','','M2*12');
INSERT INTO `Article` (`idType`, `Nom`, `Code`, `Designation`) VALUES('1','Fluke i30s','2584935','Amperemetre AC/DC');
INSERT INTO `Article` (`idType`, `Nom`, `Code`, `Designation`) VALUES('1','Fluke 179','','Multimetre');

-- --------------------------------------------------------

--
-- Structure de la table `Stock`
--

CREATE TABLE IF NOT EXISTS `Stock` (
  `idStock` int(11) NOT NULL AUTO_INCREMENT,
  `idArmoire` int(11) NOT NULL,
  `idArticle` int(11) NOT NULL,
  `idComptage` int(11) NOT NULL,
  `idUnite` int(11) NOT NULL,
  `Quantite` int(11) DEFAULT 0,
  `Disponible` int(11) DEFAULT 0,
  `numeroCasier` int(11) NOT NULL,
  PRIMARY KEY (`idStock`),
  CONSTRAINT Unique_NumeroCasier UNIQUE (`numeroCasier`),
  CONSTRAINT Stock_fk_1 FOREIGN KEY (`idArmoire`) REFERENCES Armoire(`idArmoire`) ON DELETE CASCADE,
  CONSTRAINT Stock_fk_2 FOREIGN KEY (`idArticle`) REFERENCES Article(`idArticle`) ON DELETE CASCADE,
  CONSTRAINT Stock_fk_3 FOREIGN KEY (`idComptage`) REFERENCES Comptage(`idComptage`) ON DELETE CASCADE,
  CONSTRAINT Stock_fk_4 FOREIGN KEY (`idUnite`) REFERENCES Unite(`idUnite`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Stock` (`idArmoire`,`idArticle`,`idComptage`,`idUnite`,`Quantite`,`Disponible`,`numeroCasier`) VALUES('1','1','2','2','100','100','1');
INSERT INTO `Stock` (`idArmoire`,`idArticle`,`idComptage`,`idUnite`,`Quantite`,`Disponible`,`numeroCasier`) VALUES('1','2','2','2','100','100','2');
INSERT INTO `Stock` (`idArmoire`,`idArticle`,`idComptage`,`idUnite`,`Quantite`,`Disponible`,`numeroCasier`) VALUES('1','3','2','2','100','100','3');
INSERT INTO `Stock` (`idArmoire`,`idArticle`,`idComptage`,`idUnite`,`Quantite`,`Disponible`,`numeroCasier`) VALUES('1','4','2','2','100','100','4');
INSERT INTO `Stock` (`idArmoire`,`idArticle`,`idComptage`,`idUnite`,`Quantite`,`Disponible`,`numeroCasier`) VALUES('1','5','2','2','8','6','5');
INSERT INTO `Stock` (`idArmoire`,`idArticle`,`idComptage`,`idUnite`,`Quantite`,`Disponible`,`numeroCasier`) VALUES('1','6','3','2','2','2','6');

-- --------------------------------------------------------

--
-- Structure de la table `Mouvement`
--

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

-- --------------------------------------------------------
