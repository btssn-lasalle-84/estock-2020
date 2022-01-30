#ifndef UTILISATEUR_H
#define UTILISATEUR_H

/**
 * @file Utilisateur.h
 *
 * @brief Déclaration de la classe Utilisateur
 *
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date mercredi 04 Mars 2020
 *
 */

#include <QObject>

//#define DEBUG_UTILISATEUR

/**
 * @enum ChampsUtilisateur
 * @brief Définit les différents champs de la table Utilisateur
 */
enum ChampsUtilisateur
{
    TABLE_UTILISATEUR_ID_UTILISATEUR,       //!<Emplacment de l'idUtilisateur
    TABLE_UTILISATEUR_ID_PROFIL,            //!<Emplacment de L'idProfil
    TABLE_UTILISATEUR_ID_GROUPE,            //!<Emplacment de l'idGroupe
    TABLE_UTILISATEUR_NOM,                  //!<Emplacment du nom
    TABLE_UTILISATEUR_PRENOM,               //!<Emplacment du prenom
    TABLE_UTILISATEUR_DATE_VALIDITE,        //!<Emplacment de la date de validite
    TABLE_UTILISATEUR_IDENTIFIANT,          //!<Emplacment de l'identifiant
    TABLE_UTILISATEUR_MOT_DE_PASSE,         //!<Emplacment du mot de passe
    TABLE_UTILISATEUR_BADGE,                //!<Emplacment du badge
    TABLE_UTILISATEUR_EMAIL                 //!<Emplacment du mail
};

/**
 * @class Utilisateur
 *
 * @brief La classe Utilisateur gère les données relative à l'utilisateur
 *
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date mercredi 04 Mars 2020
 *
 */

class Utilisateur : public QObject
{
    Q_OBJECT
public:
    Utilisateur(QObject *parent = nullptr);
    Utilisateur(QStringList donnees, QObject *parent = nullptr);
    ~Utilisateur();

    QString getIdentifiantUtilisateur();
    QString getIdUtilisateur();

private slots:

signals:

private:
     QString idUtilisateur; //!< idUtilisateur de l'utilisateur connecté
     QString idProfil;      //!< idProfil de l'utilisateur connecté
     QString idGroupe;      //!< idGroupe de l'utilisateur connecté
     QString nom;           //!< nom de l'utilisateur connecté
     QString prenom;        //!< prenom de l'utilisateur connecté
     QString dateValidite;  //!< dateValidite du compte
     QString identifiant;   //!< identifiant de l'utilisateur connecté
     QString motDePasse;    //!< motDePasse de l'utilisateur connecté
     QString badge;         //!< badge de l'utilisateur connecté
     QString email;         //!< email de l'utilisateur connecté
};

#endif // UTILISATEUR_H
