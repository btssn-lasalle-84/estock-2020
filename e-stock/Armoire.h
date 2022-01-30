#ifndef ARMOIRE_H
#define ARMOIRE_H

/**
 * @file Armoire.h
 *
 * @brief Déclaration de la classe Armoire
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Dimanche 22 Mars 2020
 *
 */

#include <QObject>
#include <QString>
#include <QDebug>

#define DEBUG_ARMOIRE

/**
 * @enum ChampArmoire
 * @brief Définit les différents champs de la table Armoire
 */
enum ChampArmoire
{
    TABLE_ARMOIRE_ID_ARMOIRE,
    TABLE_ARMOIRE_NOM,
    TABLE_ARMOIRE_DESCRIPTION,
    TABLE_ARMOIRE_NB_CASIERS
};

class Bdd;

/**
 * @class Armoire
 *
 * @brief La classe Armoire traite les articles
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Dimanche 22 Mars 2020
 *
 */
class Armoire : public QObject
{
    Q_OBJECT
public:
    Armoire(QObject *parent = nullptr);
    ~Armoire();

    void recupererArmoire(QString idArmoire="1");
    QStringList getInformations();
    QString getIdArmoire() const;
    QString getNom() const;
    QString getDescripton() const;
    QString getNbCasiers() const;
    QString getAdresseIPArmoire() const;

private:
    Bdd *bdd;                   //!< association d'un objet Bdd (accès à la base de données)    
    QString idArmoire;          //!< l'id de l'armoire
    QString nom;                //!< le nom de l'armoire
    QString description;        //!< la description de l'armoire
    QString nbCasiers;          //!< le nombre de casiers dans l'armoire
    QString adresseIPArmoire;   //!< l'adresse IP de la Raspberry Pi

    QString lireAdresseIP();

signals:
    void informationsArmoire(QStringList);
};

#endif // ARMOIRE_H
