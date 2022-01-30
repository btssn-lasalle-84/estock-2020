#include "Armoire.h"
#include "Bdd.h"
#include <QNetworkInterface>

/**
 * @file Armoire.cpp
 *
 * @brief Définition de la classe Armoire
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date 22 Mars 2020
 */

/**
 * @brief Définition du constructeur de la classe Armoire
 * @details Initialise un objet Armoire
 * @param parent l'objet QObject parent
 */
Armoire::Armoire(QObject *parent) : QObject(parent)
{
    #ifdef DEBUG_ARMOIRE
        qDebug() << Q_FUNC_INFO;
    #endif
    bdd = Bdd::getInstance();
    adresseIPArmoire = lireAdresseIP();
    recupererArmoire();
}

/**
 * @brief Définition du destructeur de la classe Armoire
 * @details Detruit un objet Armoire
 */
Armoire::~Armoire()
{
    Bdd::detruireInstance();
    #ifdef DEBUG_ARMOIRE
        qDebug() << Q_FUNC_INFO;
    #endif
}

/**
 * @brief Définition de la méthode recupererArmoire
 * @details Récupère les données de l'armoire dans la base de données
 * @param idArmoire l'id de l'armoire
 */
void Armoire::recupererArmoire(QString idArmoire)
{
    QString requeteBDD;

    if(!idArmoire.isEmpty()) // par id
    {
        requeteBDD = "SELECT idArmoire, Nom, Description, nbCasiers from Armoire where idArmoire = '" + idArmoire + "'";
        QStringList donnees;
        bdd->recuperer(requeteBDD, donnees);

        #ifdef DEBUG_ARMOIRE
            qDebug() << Q_FUNC_INFO << donnees;
        #endif

        if(donnees.size() > 0)
        {
            this->idArmoire = donnees.at(TABLE_ARMOIRE_ID_ARMOIRE);
            nom = donnees.at(TABLE_ARMOIRE_NOM);
            description = donnees.at(TABLE_ARMOIRE_DESCRIPTION);
            nbCasiers = donnees.at(TABLE_ARMOIRE_NB_CASIERS);
        }
    }    
}

/**
 * @brief Définition de la méthode getInformations
 * @details renvoie les informations de l'armoire
 * @return informations de l'armoire
 */
QStringList Armoire::getInformations()
{
    QStringList informations;

    informations << idArmoire << nom << description << nbCasiers << adresseIPArmoire;

    emit informationsArmoire(informations);

    return informations;
}

/**
 * @brief Définition de la méthode getIdArmoire
 * @details renvoie l'id de l'armoire
 * @return id de l'armoire
 */
QString Armoire::getIdArmoire() const
{
    return idArmoire;
}

/**
 * @brief Définition de la méthode getNom
 * @fn Armoire::getNom()
 * @details renvoie le nom de l'armoire
 * @return le nom de l'armoire
 */
QString Armoire::getNom() const
{
    return nom;
}

/**
 * @brief Définition de la méthode getDescripton
 * @fn Armoire::getDescripton()
 * @details renvoie la description de l'armoire
 * @return la description de l'armoire
 */
QString Armoire::getDescripton() const
{
    return description;
}

/**
 * @brief Définition de la méthode getNbCasiers
 * @fn Armoire::getNbCasiers()
 * @details renvoie le nombre de casiers dans l'armoire
 * @return le nombre de casiers dans l'armoire
 */
QString Armoire::getNbCasiers() const
{
    return nbCasiers;
}

/**
 * @brief Définition de la méthode getAdresseIPArmoire
 * @fn Armoire::getAdresseIPArmoire()
 * @details renvoie l'adresse IP de l'armoire
 * @return l'adresse IP de l'armoire
 */
QString Armoire::getAdresseIPArmoire() const
{
    return adresseIPArmoire;
}

/**
 * @brief Définition de la méthode lireAdresseIP
 * @fn Armoire::lireAdresseIP()
 * @details Récupère l'adresse IP de la Raspberry Pi
 * @return l'adresse IP de la Raspberry Pi
 */
QString Armoire::lireAdresseIP()
{
    QStringList adresses;
    foreach(QHostAddress adresse, QNetworkInterface::allAddresses())
    {
        // Filtre les adresses localhost ...
        if(adresse != QHostAddress::LocalHostIPv6
           && adresse != QHostAddress::LocalHost
           // ... APIPA ...
           && !adresse.isInSubnet(QHostAddress::parseSubnet("169.254.0.0/16"))
           // ... Lien Local IPv6
           && !adresse.isInSubnet(QHostAddress::parseSubnet("FE80::/64")))
        {
            qDebug() << Q_FUNC_INFO << adresse.toString();
            adresses << adresse.toString();
        }
    }

    foreach(QString adresse, adresses)
    {
        #ifdef DEBUG_ARMOIRE
            qDebug() << Q_FUNC_INFO << adresse;
        #endif
        if(adresse.contains("192."))
            return adresse;
    }

    /*if(adresses.count() > 0)
    {
        return adresses.at(0);
    }*/

    return QString("");
}
