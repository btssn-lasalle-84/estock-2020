#include "Article.h"
#include "Bdd.h"
#include <QtMath>

/**
 * @file Article.cpp
 *
 * @brief Définition de la classe Article
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date 11 Mars 2020
 */

Bdd* Article::bdd = Bdd::getInstance();

/**
 * @brief Définition du constructeur de la classe Article
 * @details Initialise un objet Article
 * @param parent l'objet QObject parent 
 */
Article::Article(QObject *parent) : QObject(parent)
{
    #ifdef DEBUG_ARTICLE
        qDebug() << Q_FUNC_INFO;
    #endif
    //bdd = Bdd::getInstance();
}

/**
 * @brief Définition de la méthode ~Article
 * @details Détruit un objet Article
 */
Article::~Article()
{
    //Bdd::detruireInstance();
    #ifdef DEBUG_ARTICLE
        qDebug() << Q_FUNC_INFO;
    #endif
}

/**
 * @brief Récupère les données d'un article de l'armoire dans la base de données par son idArticle
 * @param idArticle
 * @param numCasier si égal à 0, l'article est dans un seul casier sinon égal au numéro de casier
 * @return true si l'article a été récupéré sinon false
 */
bool Article::recupererDonneesArticle(QString idArticle, int numCasier)
{
    if(idArticle.isEmpty())
        return false;

    QString requeteBDD;

    if(numCasier == 0)
    {
        requeteBDD = "SELECT Stock.idStock, Article.idArticle, Article.Nom AS Article, Type.idType, Type.nom AS Type, Comptage.idComptage, Comptage.Nom AS Comptage, Article.Code, Article.Designation, Stock.Quantite, Stock.Disponible, Article.Poids, Stock.Tare, Unite.idUnite, Unite.Nom, Stock.numeroCasier FROM Stock INNER JOIN Article ON Article.idArticle=Stock.idArticle INNER JOIN Type ON Type.idType=Article.idType INNER JOIN Comptage ON Comptage.idComptage=Stock.idComptage INNER JOIN Unite ON Unite.idUnite=Stock.idUnite WHERE Article.idArticle = '" + idArticle + "'";
    }
    else
    {
        requeteBDD = "SELECT Stock.idStock, Article.idArticle, Article.Nom AS Article, Type.idType, Type.nom AS Type, Comptage.idComptage, Comptage.Nom AS Comptage, Article.Code, Article.Designation, Stock.Quantite, Stock.Disponible, Article.Poids, Stock.Tare, Unite.idUnite, Unite.Nom, Stock.numeroCasier FROM Stock INNER JOIN Article ON Article.idArticle=Stock.idArticle INNER JOIN Type ON Type.idType=Article.idType INNER JOIN Comptage ON Comptage.idComptage=Stock.idComptage INNER JOIN Unite ON Unite.idUnite=Stock.idUnite WHERE Article.idArticle = '" + idArticle + "' AND Stock.numeroCasier = '" + numCasier + "'";
    }

    QStringList donnees; // un seul casier pour cet article
    bdd->recuperer(requeteBDD, donnees);

    #ifdef DEBUG_ARTICLE
        qDebug() << Q_FUNC_INFO << donnees;
    #endif

    if(donnees.size() > 0)
    {
        this->idStock = donnees.at(TABLE_ARTICLE_ID_STOCK);
        this->idArticle = donnees.at(TABLE_ARTICLE_ID_ARTICLE);
        this->nomArticle = donnees.at(TABLE_ARTICLE_NOM_ARTICLE);
        this->idType = donnees.at(TABLE_ARTICLE_ID_TYPE);
        this->nomType = donnees.at(TABLE_ARTICLE_NOM_TYPE);
        this->idComptage = donnees.at(TABLE_ARTICLE_ID_COMPTAGE);
        this->nomComptage = donnees.at(TABLE_ARTICLE_NOM_COMPTAGE);
        this->codeBarre = donnees.at(TABLE_ARTICLE_CODE_BARRE);
        this->designation = donnees.at(TABLE_ARTICLE_DESIGNATION);
        this->quantite = donnees.at(TABLE_ARTICLE_QUANTITE);
        this->disponible = donnees.at(TABLE_ARTICLE_DISPONIBLE);
        this->poidsArticle = donnees.at(TABLE_ARTICLE_POIDS);
        this->tare = donnees.at(TABLE_ARTICLE_TARE);
        this->idUnite = donnees.at(TABLE_ARTICLE_ID_UNITE);
        this->nomUnite = donnees.at(TABLE_ARTICLE_NOM_UNITE);
        this->numeroCasier = donnees.at(TABLE_ARTICLE_NUMERO_CASIER);
        return true;
    }
    return false;
}

/**
 * @brief Récupère les données d'un article de l'armoire dans la base de données par son nomArticle
 * @param nomArticle
 * @param numCasier si égal à 0, l'article est dans un seul casier sinon égal au numéro de casier
 * @return true si l'article a été récupéré sinon false
 */
bool Article::recupererDonneesArticleParNom(QString nomArticle, int numCasier)
{
    if(nomArticle.isEmpty())
        return false;

    QString requeteBDD;

    if(numCasier == 0)
    {
        requeteBDD = "SELECT Stock.idStock, Article.idArticle, Article.Nom AS Article, Type.idType, Type.nom AS Type, Comptage.idComptage, Comptage.Nom AS Comptage, Article.Code, Article.Designation, Stock.Quantite, Stock.Disponible, Article.Poids, Stock.Tare, Unite.idUnite, Unite.Nom, Stock.numeroCasier FROM Stock INNER JOIN Article ON Article.idArticle=Stock.idArticle INNER JOIN Type ON Type.idType=Article.idType INNER JOIN Comptage ON Comptage.idComptage=Stock.idComptage INNER JOIN Unite ON Unite.idUnite=Stock.idUnite WHERE Article.Nom = '" + nomArticle + "'";
    }
    else
    {
        requeteBDD = "SELECT Stock.idStock, Article.idArticle, Article.Nom AS Article, Type.idType, Type.nom AS Type, Comptage.idComptage, Comptage.Nom AS Comptage, Article.Code, Article.Designation, Stock.Quantite, Stock.Disponible, Article.Poids, Stock.Tare, Unite.idUnite, Unite.Nom, Stock.numeroCasier FROM Stock INNER JOIN Article ON Article.idArticle=Stock.idArticle INNER JOIN Type ON Type.idType=Article.idType INNER JOIN Comptage ON Comptage.idComptage=Stock.idComptage INNER JOIN Unite ON Unite.idUnite=Stock.idUnite WHERE Article.Nom = '" + nomArticle + "' AND Stock.numeroCasier = '" + numCasier + "'";
    }

    QStringList donnees; // un seul casier pour cet article
    bdd->recuperer(requeteBDD, donnees);

    #ifdef DEBUG_ARTICLE
        qDebug() << Q_FUNC_INFO << donnees;
    #endif

    if(donnees.size() > 0)
    {
        this->idStock = donnees.at(TABLE_ARTICLE_ID_STOCK);
        this->idArticle = donnees.at(TABLE_ARTICLE_ID_ARTICLE);
        this->nomArticle = donnees.at(TABLE_ARTICLE_NOM_ARTICLE);
        this->idType = donnees.at(TABLE_ARTICLE_ID_TYPE);
        this->nomType = donnees.at(TABLE_ARTICLE_NOM_TYPE);
        this->idComptage = donnees.at(TABLE_ARTICLE_ID_COMPTAGE);
        this->nomComptage = donnees.at(TABLE_ARTICLE_NOM_COMPTAGE);
        this->codeBarre = donnees.at(TABLE_ARTICLE_CODE_BARRE);
        this->designation = donnees.at(TABLE_ARTICLE_DESIGNATION);
        this->quantite = donnees.at(TABLE_ARTICLE_QUANTITE);
        this->disponible = donnees.at(TABLE_ARTICLE_DISPONIBLE);
        this->poidsArticle = donnees.at(TABLE_ARTICLE_POIDS);
        this->tare = donnees.at(TABLE_ARTICLE_TARE);
        this->idUnite = donnees.at(TABLE_ARTICLE_ID_UNITE);
        this->nomUnite = donnees.at(TABLE_ARTICLE_NOM_UNITE);
        this->numeroCasier = donnees.at(TABLE_ARTICLE_NUMERO_CASIER);
        return true;
    }

    return false;
}

/**
 * @brief Définition de la méthode recupererDonneesArticleParNumeroCasier
 * @details permet de recuperer les données d'un article grâçe au numéro du casier
 * @param numeroCasier le numéro du casier
 * @return un booleen pour verifier si la mise à jour des données a bien était faite
 */
bool Article::recupererDonneesArticleParNumeroCasier(QString numeroCasier)
{
    if(numeroCasier.isEmpty())
        return false;

    QString requeteBDD;

    requeteBDD = "SELECT Stock.idStock, Article.idArticle, Article.Nom AS Article, Type.idType, Type.nom AS Type, Comptage.idComptage, Comptage.Nom AS Comptage, Article.Code, Article.Designation, Stock.Quantite, Stock.Disponible, Article.Poids, Stock.Tare, Unite.idUnite, Unite.Nom, Stock.numeroCasier FROM Stock INNER JOIN Article ON Article.idArticle=Stock.idArticle INNER JOIN Type ON Type.idType=Article.idType INNER JOIN Comptage ON Comptage.idComptage=Stock.idComptage INNER JOIN Unite ON Unite.idUnite=Stock.idUnite WHERE Stock.numeroCasier = '" + numeroCasier + "'";

    QStringList donnees;
    bdd->recuperer(requeteBDD, donnees);

    #ifdef DEBUG_ARTICLE
        qDebug() << Q_FUNC_INFO << donnees;
    #endif

    if(donnees.size() > 0)
    {
        this->idStock = donnees.at(TABLE_ARTICLE_ID_STOCK);
        this->idArticle = donnees.at(TABLE_ARTICLE_ID_ARTICLE);
        this->nomArticle = donnees.at(TABLE_ARTICLE_NOM_ARTICLE);
        this->idType = donnees.at(TABLE_ARTICLE_ID_TYPE);
        this->nomType = donnees.at(TABLE_ARTICLE_NOM_TYPE);
        this->idComptage = donnees.at(TABLE_ARTICLE_ID_COMPTAGE);
        this->nomComptage = donnees.at(TABLE_ARTICLE_NOM_COMPTAGE);
        this->codeBarre = donnees.at(TABLE_ARTICLE_CODE_BARRE);
        this->designation = donnees.at(TABLE_ARTICLE_DESIGNATION);
        this->quantite = donnees.at(TABLE_ARTICLE_QUANTITE);
        this->disponible = donnees.at(TABLE_ARTICLE_DISPONIBLE);
        this->poidsArticle = donnees.at(TABLE_ARTICLE_POIDS);
        this->tare = donnees.at(TABLE_ARTICLE_TARE);
        this->idUnite = donnees.at(TABLE_ARTICLE_ID_UNITE);
        this->nomUnite = donnees.at(TABLE_ARTICLE_NOM_UNITE);
        this->numeroCasier = donnees.at(TABLE_ARTICLE_NUMERO_CASIER);
        return true;
    }

    return false;
}

/**
 * @brief Définition de la méthode recupererNombreCasiersPourIdArticle
 * @details permet de recuperer le nombre de casiers à partir de l'id d'un article
 * @param idArticle l'id d'un article
 * @return un unsigned int qui correspond au nombre de casier
 */
unsigned int Article::recupererNombreCasiersPourIdArticle(QString idArticle)
{
    QString requete = "SELECT COUNT(Stock.idArticle) FROM Stock INNER JOIN Article ON Stock.idArticle = Article.idArticle WHERE Article.idArticle = '" + idArticle + "'";

    QString donnees;
    bdd->recuperer(requete, donnees);

    return donnees.toUInt();
}

/**
 * @brief Définition de la méthode recupererNombreCasiersPourNomArticle
 * @details permet de recuperer le nombre de casiers à partir du nom d'un article
 * @param nomArticle le nom d'un article
 * @return un unsigned int qui correspond au nombre de casier
 */
unsigned int Article::recupererNombreCasiersPourNomArticle(QString nomArticle)
{
    QString requete = "SELECT COUNT(Stock.idArticle) FROM Stock INNER JOIN Article ON Stock.idArticle = Article.idArticle WHERE Article.Nom = '" + nomArticle + "'";

    QString donnees;
    bdd->recuperer(requete, donnees);

    return donnees.toUInt();
}

/**
 * @brief Définition de la méthode recupererNumeroCasierPourIdArticle
 * @details permet de recuperer le numero des casiers à partir de l'id d'un article
 * @param idArticle l'id d'un article
 * @return le numero des casiers
 */
QVector<QString> Article::recupererNumeroCasierPourIdArticle(QString idArticle)
{
    QString requete = "SELECT Stock.numeroCasier FROM Stock INNER JOIN Article ON Article.idArticle=Stock.idArticle INNER JOIN Type ON Type.idType=Article.idType INNER JOIN Comptage ON Comptage.idComptage=Stock.idComptage INNER JOIN Unite ON Unite.idUnite=Stock.idUnite WHERE Article.idArticle = '" + idArticle + "'";

    QVector<QString> donnees;
    bdd->recuperer(requete, donnees);

    return donnees;
}

/**
 * @brief Définition de la méthode recupererNumeroCasierPourNomArticle
 * @details permet de recuperer le numero des casiers à partir du nom d'un article
 * @param nomArticle le nom d'un article
 * @return le numero des casiers
 */
QVector<QString> Article::recupererNumeroCasierPourNomArticle(QString nomArticle)
{
    QString requete = "SELECT Stock.numeroCasier FROM Stock INNER JOIN Article ON Article.idArticle=Stock.idArticle INNER JOIN Type ON Type.idType=Article.idType INNER JOIN Comptage ON Comptage.idComptage=Stock.idComptage INNER JOIN Unite ON Unite.idUnite=Stock.idUnite WHERE Article.Nom = '" + nomArticle + "'";

    QVector<QString> donnees;
    bdd->recuperer(requete, donnees);

    return donnees;
}

/**
 * @brief Définition de la méthode get
 * @details Accesseur get pour les différents champs d'un Article
 * @param champ un champ de la table = un attribut
 * @return
 */
QString Article::get(ChampArticle champ)
{
    switch(champ)
    {
        case TABLE_ARTICLE_ID_STOCK:
            return this->idStock;
            break;
        case TABLE_ARTICLE_ID_ARTICLE:
            return this->idArticle;
            break;
        case TABLE_ARTICLE_NOM_ARTICLE:
            return this->nomArticle;
            break;
        case TABLE_ARTICLE_ID_TYPE:
            return this->idType;
            break;
        case TABLE_ARTICLE_NOM_TYPE:
            return this->nomType;
            break;
        case TABLE_ARTICLE_ID_COMPTAGE:
            return this->idComptage;
            break;
        case TABLE_ARTICLE_NOM_COMPTAGE:
            return this->nomComptage;
            break;
        case TABLE_ARTICLE_CODE_BARRE:
            return this->codeBarre;
            break;
        case TABLE_ARTICLE_DESIGNATION:
            return this->designation;
            break;
        case TABLE_ARTICLE_QUANTITE:
            return this->quantite;
            break;
        case TABLE_ARTICLE_DISPONIBLE:
            return this->disponible;
            break;
        case TABLE_ARTICLE_POIDS:
            return this->poidsArticle;
            break;
        case TABLE_ARTICLE_TARE:
            return this->tare;
            break;
        case TABLE_ARTICLE_ID_UNITE:
            return this->idUnite;
            break;
        case TABLE_ARTICLE_NOM_UNITE:
            return this->nomUnite;
            break;
        case TABLE_ARTICLE_NUMERO_CASIER:
            return this->numeroCasier;
            break;
        default:
            qDebug() << Q_FUNC_INFO << champ << "champ inconnu";
    }
    return QString("");
}

/**
 * @brief Définition de la méthode mettreAJourQuantite
 * @details permet de mettre à jour la quantite disponible d'un article
 * @param quantite
 */
void Article::mettreAJourQuantite(QString quantite)
{
    if(idArticle.isEmpty())
        return;
    if(this->quantite != quantite)
    {
        #ifdef DEBUG_ARTICLE
            qDebug() << Q_FUNC_INFO << "quantite" << quantite;
        #endif
        this->quantite = quantite;
        QString requete = "UPDATE Stock SET Disponible =" + quantite + " WHERE idArticle =" + idArticle + ";";
        bdd->executer(requete);
    }
}
