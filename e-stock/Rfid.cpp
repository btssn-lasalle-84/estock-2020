#include "Rfid.h"
#include <QDebug>

/**
 * @file Rfid.cpp
 *
 * @brief Définition de la classe Rfid
 *
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date 14 Février 2020
 */

/**
 * @brief Définition du constructeur de la classe Rfid
 * @fn Rfid::Rfid(QObject *parent)
 * @details Initialise un objet Rfid
 * @param parent l'objet QObject parent
 */
Rfid::Rfid(QObject *parent) : QObject(parent), badge("")
{
}

/**
 * @brief Définition du destructeur de la classe Rfid
 * @fn Rfid::~Rfid()
 * @details Détruitun objet Rfid
 */
Rfid::~Rfid()
{

}

/**
 * @brief Définition de la méthode corrigerBadge(QString badge)
 * @fn Rfid::corrigerBadge(QString badge)
 * @details La trame reçue provient de l'émulation d'un clavier QWERTY qu'il faut traduire en AZERTY
 * @param badge la trame badge en format QWERTY
 * @return la trame badge en format AZERTY
 */
QString Rfid::corrigerBadge(QString badge)
{
    QString badgeCorrige = "";

    if(!badge.isEmpty())
    {
        // effectue les remplacements des touches QWERTY en touches AZERTY
        badgeCorrige = badge.replace(QString::fromUtf8("Q"), "A");
        badgeCorrige = badge.replace(QString::fromUtf8("W"), "Z");
        badgeCorrige = badge.replace(QString::fromUtf8("q"), "q");
        badgeCorrige = badge.replace(QString::fromUtf8("w"), "z");
        badgeCorrige = badge.replace(QString::fromUtf8("M"), ":");
        badgeCorrige = badge.replace(QString::fromUtf8("à"), "0");
        badgeCorrige = badge.replace(QString::fromUtf8("&"), "1");
        badgeCorrige = badge.replace(QString::fromUtf8("é"), "2");
        badgeCorrige = badge.replace(QString::fromUtf8("\""), "3");
        badgeCorrige = badge.replace(QString::fromUtf8("'"), "4");
        badgeCorrige = badge.replace(QString::fromUtf8("("), "5");
        badgeCorrige = badge.replace(QString::fromUtf8("-"), "6");
        badgeCorrige = badge.replace(QString::fromUtf8("è"), "7");
        badgeCorrige = badge.replace(QString::fromUtf8("_"), "8");
        badgeCorrige = badge.replace(QString::fromUtf8("ç"), "9");
    }
    return badgeCorrige;
}

/**
 * @brief Définition de la méthode setBadge(QString badge)
 * @fn Rfid::setBadge(QString badge)
 * @details Accesseur set de l'attribut badge
 * @param badge
 */
void Rfid::setBadge(QString badge)
{
    this->badge = badge;
}

/**
 * @brief Définition de la méthode setUid(QString uid)
 * @fn Rfid::setUid(QString uid)
 * @details Accesseur set de l'attribut uid
 * @param uid
 */
void Rfid::setUid(QString uid)
{
    this->uid = uid;
}

/**
 * @brief Définition de la méthode Rfid::traiterBadge(QString trameBadge)
 * @fn Rfid::traiterBadge(QString trameBadge)
 * @details Extrait l'UID d'une trame badge valide
 * @param trameBadge la trame reçue du lecteur Rfid
 */
void Rfid::traiterBadge(QString trameBadge)
{
    /*
     * Format trame reçue : RFID:xxxxxxxx
     * xxxxxxxx -> uid du badge
     */
    setBadge(corrigerBadge(trameBadge));    

    #ifdef DEBUG_RFID
        qDebug() << Q_FUNC_INFO << "Badge" << badge;
    #endif

    // Vérifier si la trame est valide
    if(badge.startsWith("RFID:"))
    {
        extraireUid();

        emit nouveauUidBadge(uid);
        #ifdef DEBUG_RFID
            qDebug() << Q_FUNC_INFO << "UID" << uid;
        #endif
    }
    else
    {
        emit erreurBadgeInvalide(ERREUR_BADGE_INVALIDE);
    }
}

/**
 * @brief Définition de la méthode Rfid::extraireUid()
 * @fn Rfid::extraireUid()
 * @details Extrait l'UID de la trame badge
 */
void Rfid::extraireUid()
{
    setUid(badge.section(':',1,1));
}
