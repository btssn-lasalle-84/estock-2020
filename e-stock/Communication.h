#ifndef COMMUNICATION_H
#define COMMUNICATION_H

/**
 * @file    Communication.h
 * @brief   Définition de la classe Communication
 * @details La classe Communication permet de communiquer avec le port série
 * @author  TRANCHAT Joffrey
 * @author  LEGGER Pierre-Antoine
 * @version 1.0
 */

#include <QObject>
#include <QString>
#include <QSerialPort>

//#define DEBUG_COMMUNICATION

/**
 * @def SERIAL_PORT_NAME
 * @brief Définit le nom du port série associé au SE
 */
//#define SERIAL_PORT_NAME "/dev/se"
#define SERIAL_PORT_NAME "/dev/ttyUSB0"

// Protocole e-stock
#define EN_TETE             QString("CASIERS")
#define DELIMITEUR_CHAMP    QString(";")
#define DELIMITEUR_FIN      QString("\r\n")

// Types de trames
#define TRAME_OUVERTURE     QString("1")
#define TRAME_ETAT          QString("2")
#define TRAME_POIDS         QString("3")

/**
 * @class Communication
 *
 * @brief La classe Communication permet de communiquer avec le port série
 *
 * @author Tranchat Joffrey
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date jeudi 12 Mars 2020
 */
class Communication : public QObject
{
    Q_OBJECT

public:

    Communication(QObject *parent = nullptr);
    ~Communication();

    void demarrerCommunicationPort();
    void arreterCommunicationPort();
    void configurerPort();
    void ouvrirPort();
    void setNomPort(QString nouveauPortSerie);
    void envoyerTrame(QString trame);
    void envoyerRequetePoids(QString numeroCasier = 0);

public slots:
    void recevoirTrame();
    void envoyerRequeteOuvertureCasier(QString numeroCasier);
    void envoyerRequeteEtatCasiers(QString numeroCasier);

private:
     QSerialPort *port; //!< Variable pointeur sur le port
     QString trameBrute; //!< Variable qui contient la trame brute
     QString nomPort; //!< Variable qui contient le nom du port serie

     bool verifierTrame(QString trame);
     void traiterTrame(QString trame);

signals:
     void envoieTrameOuverture(QString trame);
     void envoieTrameEtat(QString trame);
     void envoieTramePoids(QString trame);
};

#endif // COMMUNICATION_H
