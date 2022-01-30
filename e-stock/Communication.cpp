#include "Communication.h"
#include <QDebug>
#include <QObject>

/**
 * @file    Communication.cpp
 *
 * @brief   Déclaration de la classe Communication
 *
 * @author  TRANCHAT Joffrey
 * @author  LEGGER Pierre-Antoine
 *
 * @version 1.0
 *
 * @date jeudi 12 Mars2020
 */

/**
 * @brief Constructeur de la classe Communication
 * @details initialise un objet Communication
 * @param parent
 */
Communication::Communication(QObject *parent) : QObject(parent), port(new QSerialPort(this)), trameBrute("\0"), nomPort(SERIAL_PORT_NAME)
{
    #ifdef DEBUG_COMMUNICATION
        qDebug() << Q_FUNC_INFO;
    #endif
}

/**
 * @brief Destructeur de la classe Communication
 * @details Détruit uun objet Communication et ferme le port série
 */
Communication::~Communication()
{
    port->close();
    #ifdef DEBUG_COMMUNICATION
        qDebug() << Q_FUNC_INFO;
    #endif
}

/**
 * @brief Définition de la méthode demarrerCommunicationPort
 * @details Méthode qui démarre la configuration du port serie et ouvre le port serie
 */
void Communication::demarrerCommunicationPort()
{
    #ifdef DEBUG_COMMUNICATION
        qDebug() << Q_FUNC_INFO;
    #endif
    configurerPort();
    ouvrirPort();
}

/**
 * @brief Définition de la méthode arreterCommunicationPort
 * @details Méthode qui ferme le port série
 */
void Communication::arreterCommunicationPort()
{
    #ifdef DEBUG_COMMUNICATION
        qDebug() << Q_FUNC_INFO;
    #endif
    port->close();
}

/**
 * @brief Définition de la méthode configurerPort
 * @details Méthode qui configure le port serie par défaut
 */
void Communication::configurerPort()
{
    #ifdef DEBUG_COMMUNICATION
        qDebug() << Q_FUNC_INFO;
    #endif
    port->setPortName(nomPort);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
}

/**
 * @brief Définition de la métohde ouvrirPort
 * @details Méthode qui ouvre le port serie en lecture et écriture
 */
void Communication::ouvrirPort()
{
    if (port->open(QIODevice::ReadWrite))
    {
        #ifdef DEBUG_COMMUNICATION
            qDebug() << Q_FUNC_INFO << "connecté au port" << nomPort;
        #endif
        connect(port, SIGNAL(readyRead()), this, SLOT(recevoirTrame()));
    }
    else
    {
        #ifdef DEBUG_COMMUNICATION
            qDebug() << Q_FUNC_INFO << "erreur ouverture du port" << port->error();
        #endif
    }
}

/**
 * @brief Définition de la méthode setNomPort
 * @details Méthode qui permet de définir le nom du port série à utiliser
 * @param nouveauPortSerie
 */
void Communication::setNomPort(QString nouveauPortSerie)
{
    nomPort = nouveauPortSerie;
    #ifdef DEBUG_COMMUNICATION
        qDebug() << Q_FUNC_INFO << nomPort;
    #endif
}

/**
 * @brief Définition de la méthode envoyerTrame
 * @details Méthode qui permet d'envoyer une trame via le port série
 * @param trame
 */
void Communication::envoyerTrame(QString trame)
{
    if (port->isOpen())
    {
       port->write(trame.toLatin1());
    }
}

/**
 * @brief Définition de la méthode recevoirTrame
 * @details Méthode qui permet de recevoir une trame via le port série
 */
void Communication::recevoirTrame()
{
    trameBrute = "\0";

    while (port->waitForReadyRead(500))
    {
        trameBrute.append(port->readAll());
    }    

    if(verifierTrame(trameBrute))
        traiterTrame(trameBrute);
}

/**
 * @brief Définition de la méthode verifierTrame
 * @details Méthode qui vérifie si la trame respecte le protocole
 * @param trame
 * @return un booleen qui indique si la trame est correct ou nom
 */
bool Communication::verifierTrame(QString trame)
{
    #ifdef DEBUG_COMMUNICATION
        qDebug() << Q_FUNC_INFO << trame;
    #endif
    if(!trame.startsWith(EN_TETE))
    {
        return false;
    }
    if(!trame.endsWith(DELIMITEUR_FIN))
    {
        return false;
    }

    return true;
}

/**
 * @brief Définition de la méthode TraiterTrame
 * @details Méthode qui signale le type de trame reçue
 * @param trame
 */
void Communication::traiterTrame(QString trame)
{
    if(trame.startsWith(EN_TETE + DELIMITEUR_CHAMP + TRAME_OUVERTURE + DELIMITEUR_CHAMP))
    {
        emit envoieTrameOuverture(trame);
    }
    else if(trame.startsWith(EN_TETE + DELIMITEUR_CHAMP + TRAME_ETAT + DELIMITEUR_CHAMP))
    {
        emit envoieTrameEtat(trame);
    }
    else if(trame.startsWith(EN_TETE + DELIMITEUR_CHAMP + TRAME_POIDS + DELIMITEUR_CHAMP))
    {
        emit envoieTramePoids(trame);
    }
}

/**
 * @brief Définition de la méthode envoyerRequetePoids
 * @details Méthode qui permet d'envoyer une requête pour peser un casier
 * @param numeroCasier
 */
void Communication::envoyerRequetePoids(QString numeroCasier)
{
    QString trame = "CASIERS;3;" + numeroCasier + ";\r\n";
    envoyerTrame(trame);
}

/**
 * @brief Définition de la méthode envoyerRequeteOuvertureCasier
 * @details Méthode qui permet d'envoyer une requête pour ouvrire un casier
 * @param numeroCasier
 */
void Communication::envoyerRequeteOuvertureCasier(QString numeroCasier)
{
    QString trame = "CASIERS;1;" + numeroCasier + ";\r\n";
    envoyerTrame(trame);
}

/**
 * @brief Définition de la méthode envoyerRequeteEtatCasiers
 * @details Méthode qui permet d'envoyer une requête pour demande l'état d'un ou plusieurs casiers
 * @param numeroCasier
 */
void Communication::envoyerRequeteEtatCasiers(QString numeroCasier)
{
    QString trame = "CASIERS;2;" + numeroCasier + ";\r\n";
    envoyerTrame(trame);
}
