#include "CodeBarre.h"
#include "Bdd.h"
#include <QDebug>

/**
 * @file CodeBarre.cpp
 *
 * @brief Définition de la classe CodeBare
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 12 Février 2020
 *
 */

/**
 * @brief Définition du constructeur de la classe CodeBare
 * @param parent
 * @details initialise un objet CodeBarre
 */
CodeBarre::CodeBarre(QObject *parent) : QObject(parent), etatPrendreOuAjouter(false), quantiteObjet(0)
{
    #ifdef DEBUG_CODE_BARRE
        qDebug() << Q_FUNC_INFO;
    #endif
    bdd = Bdd::getInstance();
}

/**
 * @brief Définition du destructeur de la classe CodeBare
 * @details Détruit un objet CodeBarre
 */
CodeBarre::~CodeBarre()
{
    Bdd::detruireInstance();
    #ifdef DEBUG_CODE_BARRE
        qDebug() << Q_FUNC_INFO;
    #endif
}

/**
 * @brief Définition de la méthode setEtatPrendreOuAjouter
 * @details mutateur de la variable de classe EtatPrendreOuAjouter
 * @param boolean etat
 */
void CodeBarre::setEtatPrendreOuAjouter(bool etat)
{
    this->etatPrendreOuAjouter = etat;

    #ifdef DEBUG_CODE_BARRE
        qDebug() << Q_FUNC_INFO << "etatPrendreOuAjouter" << this->etatPrendreOuAjouter ;
    #endif
}

/**
 * @brief Définition de la méthode setQuantiteObjet
 * @details mutateur de la variable de classe quantiteObjet
 * @param unsigned int quantite
 */
void CodeBarre::setQuantiteObjet(unsigned int quantite)
{
    quantiteObjet = quantite;
}

/**
 * @brief Définition de la méthode prendreOuAjouter
 * @details slot appeller quand on clique sur le bouton ajouter ou le bouton prendre de la page codeBarre
 * @param boolean etat
 */
void CodeBarre::prendreOuAjouter(bool etat)
{
    setEtatPrendreOuAjouter(etat);
}

/**
 * @brief Définition de la méthode changerQuantiteObjet
 * @details slot permettant de modifier la valeur de la variable de classe quantiteObjet
 * @param quantite
 */
void CodeBarre::changerQuantiteObjet(int quantite)
{
    setQuantiteObjet(quantite);

    #ifdef DEBUG_CODE_BARRE
        qDebug() << Q_FUNC_INFO << "quantiteObjet" << quantiteObjet;
    #endif
}

/**
 * @brief Définition de la méthode traiterCodeBarre
 * @details slot appeller quand on a scanné un code barres
 * @param QString codeBarre
 */
void CodeBarre::traiterCodeBarre(QString codeBarre)
{
    #ifdef DEBUG_CODE_BARRE
        qDebug() << Q_FUNC_INFO << "codeBarre" << codeBarre;
    #endif
    QString codeBarreCorriger = corrigerCodeBarre(codeBarre);

    if(!etatPrendreOuAjouter)
    {
        // prendre objet
        emit prendreObjet(codeBarreCorriger);
    }
    else
    {
        // ajouter objet
        emit ajouterObjet(codeBarreCorriger);
    }
}

/**
 * @brief Définition de la méthode getQuantiteObjet
 * @details accesseur de la variable de classe quantiteObjet
 * @return unsigned int quantiteObjet
 */
unsigned int CodeBarre::getQuantiteObjet()
{
    return quantiteObjet;
}

/**
 * @brief Définition de la méthode corrigerCodeBarre
 * @details méthode permettant de corriger le code barre en AZERTY au cas où ce dernier aurait était écrit en QWERTY
 * @param QString codeBarre
 */
QString CodeBarre::corrigerCodeBarre(QString codeBarre)
{
    QString codeBarreCorrige = "";

    if(!codeBarre.isEmpty())
    {
        // effectue les remplacements des touches QWERTY en touches AZERTY
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("à"), "0");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("&"), "1");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("é"), "2");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("\""), "3");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("'"), "4");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("("), "5");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("-"), "6");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("è"), "7");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("_"), "8");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("ç"), "9");
        codeBarreCorrige = codeBarre.replace(QString::fromUtf8("\n"), "");
    }
    #ifdef DEBUG_CODE_BARRE
        qDebug() << Q_FUNC_INFO << "codeBarreCorrige" << codeBarreCorrige;
    #endif
    return codeBarreCorrige;
}

/**
 * @brief Définition de la méthode recupererQuantiteMaxParNumeroCasier
 * @details méthode permettant de récupérer la quantité maximum d'un objet dans un casier avec son numéro
 * @param QString numeroCasier
 * @return un unsigned int qui correspond à la quantité maximum possible dans un casier
 */
unsigned int CodeBarre::recupererQuantiteMaxParNumeroCasier(QString numeroCasier)
{
    QString requete = "SELECT Stock.Quantite FROM Stock WHERE Stock.numeroCasier = '" + numeroCasier + "'";

    QString donnees;
    bdd->recuperer(requete, donnees);

    return donnees.toUInt();
}

/**
 * @brief Définition de la méthode recupererQuantiteDisponibleParNumeroCasier
 * @details méthode permettant de récupérer la quantité disponible d'un objet dans un casier avec son numéro
 * @param QString numeroCasier
 * @return un unsigned int qui correspond à la quantité disponible possible dans un casier
 */
unsigned int CodeBarre::recupererQuantiteDisponibleParNumeroCasier(QString numeroCasier)
{
    QString requete = "SELECT Stock.Disponible FROM Stock WHERE Stock.numeroCasier = '" + numeroCasier + "'";

    QString donnees;
    bdd->recuperer(requete, donnees);

    return donnees.toUInt();
}

/**
 * @brief Définition de la méthode recupererIdArticleAvecCodeBarres
 * @details méthode permettant de récupérer la quantité disponible d'un objet dans un casier avec son numéro
 * @param QString codeBarre
 * @return un unsigned int qui correspond à l'idArticle
 */
unsigned int CodeBarre::recupererIdArticleAvecCodeBarres(QString codeBarre)
{
    QString requete = "SELECT idArticle FROM Article WHERE Code = '" + codeBarre + "'";

    QString donnees;
    bdd->recuperer(requete, donnees);

    return donnees.toUInt();
}
