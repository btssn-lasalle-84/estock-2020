#ifndef RFID_H
#define RFID_H

/**
 * @file Rfid.h
 *
 * @brief Déclaration de la classe Rfid
 *
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date Mercredi 4 Mars 2020
 *
 */

//#define DEBUG_RFID
#define ERREUR_BADGE_INVALIDE "Erreur badge invalide" //!< Message d'erreur pour un basge invalide

#include <QObject>

/**
 * @class Rfid
 *
 * @brief La classe Rfid traite la trame reçue d'un lecteur Rfid
 *
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date Mercredi 4 Mars 2020
 *
 */

class Rfid : public QObject
{
    Q_OBJECT

public:
    Rfid(QObject *parent = nullptr);
    ~Rfid();

    QString corrigerBadge(QString badge);

    void setBadge(QString badge);
    void setUid(QString uid);

private slots:
    void traiterBadge(QString trameBadge);

signals:
    void erreurBadgeInvalide(QString message);
    void nouveauUidBadge(QString badge);

private:
    QString badge; //!< trame reçue d'un badge
    QString uid; //!< l'UID extrait de la trame badge

    void extraireUid();
};

#endif // RFID_H
