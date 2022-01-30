#include "Casier.h"

/**
 * @file Casier.cpp
 *
 * @brief Définition de la classe Casier
 *
 * @author Legger Pierre-Antoine
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date samedi 28 Mars 2020
 */


/**
 * @brief Définition de la méthode Casier
 * @param numero
 * @param parent
 * @details initialise un objet Casier
 */
Casier::Casier(int numero, QWidget *parent) : QPushButton(parent), numero(numero), ouvert(false)
{
    qDebug() << Q_FUNC_INFO << numero << this;
    setText("Casier " + QString::number(numero));

    /**
     * @todo Définir une constante pour une taille minimum du casier dans l'IHM
     */
    setMaximumHeight(100);
    setContentsMargins(10, 0, 10, 0); // Marges : Gauche Haut Droite Bas
    /**
     * @todo Gérer les différentes couleurs de fond par rapport à l'état (ouvert/fermé, vide, ...)
     */
    //setStyleSheet("background-color: rgb(85, 85, 85);font-size: 18px;"); // inconnu
    setStyleSheet("background-color: rgb(239, 41, 41);font-size: 18px;"); // fermé

    /**
     * @todo Connecter signal/slot si nécessaire
     */
    connect(this, SIGNAL(clicked(bool)), this, SLOT(gererEtat()));
}

/**
 * @brief Définition du destructeur de la classe Casier
 * @details Détruit un objet Casier
 */
Casier::~Casier()
{
    qDebug() << Q_FUNC_INFO << numero << this;
}

/**
 * @brief Définition de la méthode getNumero
 * @details renvoie le numero du caiser
 * @return numero du casier
 */
int Casier::getNumero() const
{
    return numero;
}

/**
 * @brief Définition de la méthode estOuvert
 * @details renvoie l'état ouvert/fermer du casier
 * @return état du casier
 */
bool Casier::estOuvert() const
{
    return ouvert;
}

/**
 * @brief Définition de la méthode setOuvert
 * @details modifie l'état puvert ou fermé du casier
 * @param bool ouvert
 */
void Casier::setOuvert(bool ouvert)
{
    if(this->ouvert != ouvert)
    {
        this->ouvert = ouvert;
        if(ouvert)
            setStyleSheet("background-color: rgb(115, 210, 22);font-size: 18px;"); // ouvert
        else
            setStyleSheet("background-color: rgb(239, 41, 41);font-size: 18px;"); // fermé
        emit estOuvert(numero, ouvert);
    }
}

/**
 * @brief Définition de la méthode ouvrir
 * @details envoie la trame d'ouverture du casier
 */
void Casier::ouvrir()
{
    /**
     * @todo Envoyer trame ouverture
     */

    // simule le casier ouvert
    setOuvert(true);
}

/**
 * @todo Créer les méthode pour gérer un casier, notamment les signaux et les slots
 */

/**
 * @brief Définition de la méthode gererEtat
 * @details gère l'état ouvert ou fermer du casier
 */
void Casier::gererEtat()
{
    qDebug() << Q_FUNC_INFO << numero << this;
    if(!ouvert)
    {
        ouvrir();
    }
    #ifdef SIMULATION_CASIER
    else
    {
        // simule le casier fermé
        setOuvert(false);
    }
    #endif
}
