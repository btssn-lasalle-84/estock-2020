#include "Utilisateur.h"
#include <QDebug>

/**
 * @file Utilisateur.cpp
 *
 * @brief Définition de la classe Utilisateur
 *
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date mercredi 04 Mars 2020
 *
 */

/**
 * @brief Constructeur de la classe Utilisateur
 * @details Initialise un objet Utilisateur
 * @param parent
 */
Utilisateur::Utilisateur(QObject *parent) : QObject(parent)
{
    #ifdef DEBUG_UTILISATEUR
        qDebug() << Q_FUNC_INFO;
    #endif
    idUtilisateur = "";
    idProfil = "";
    idGroupe = "";
    nom = "";
    prenom = "";
    dateValidite = "";
    identifiant = "";
    motDePasse = "";
    badge = "";
    email = "";
}

/**
 * @brief Constructeur de la classe Utilisateur
 * @details Initialise un objet Utilisateur
 * @param donnees
 * @param parent
 */
Utilisateur::Utilisateur(QStringList donnees, QObject *parent) : QObject(parent)
{
    #ifdef DEBUG_UTILISATEUR
        qDebug() << Q_FUNC_INFO << donnees;
    #endif
    idUtilisateur = donnees.at(TABLE_UTILISATEUR_ID_UTILISATEUR);
    idProfil = donnees.at(TABLE_UTILISATEUR_ID_PROFIL);
    idGroupe = donnees.at(TABLE_UTILISATEUR_ID_GROUPE);
    nom = donnees.at(TABLE_UTILISATEUR_NOM);
    prenom = donnees.at(TABLE_UTILISATEUR_PRENOM);
    dateValidite = donnees.at(TABLE_UTILISATEUR_DATE_VALIDITE);
    identifiant = donnees.at(TABLE_UTILISATEUR_IDENTIFIANT);
    motDePasse = donnees.at(TABLE_UTILISATEUR_MOT_DE_PASSE);
    badge = donnees.at(TABLE_UTILISATEUR_BADGE);
    email = donnees.at(TABLE_UTILISATEUR_EMAIL);
}

/**
 * @brief Destructeur de la classe Utilisateur
 * @fn Utilisateur::~Utilisateur()
 * @details Détruit un objet Utilisateur
 */
Utilisateur::~Utilisateur()
{

}

/**
 * @brief Définition de la méthode getIdentifiantUtilisateur
 * @fn Utilisateur::getIdentifiantUtilisateur()
 * @details retourne les identifiant de l'utilisateur
 * @return les identifiants
 */
QString Utilisateur::getIdentifiantUtilisateur()
{
    return nom + " " + prenom;
}

/**
 * @brief Définition de la méthode getIdUtilisateur
 * @details retourne l'es identifiant de l'utilisateur'id de l'utilisateur
 * @return QString idUtilisateur
 */
QString Utilisateur::getIdUtilisateur()
{
    return idUtilisateur;
}
