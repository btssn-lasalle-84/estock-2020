#ifndef ARTICLE_H
#define ARTICLE_H


/**
 * @file Article.h
 *
 * @brief Déclaration de la classe Article
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 11 Mars 2020
 *
 */

#include <QObject>
#include <QString>
#include <QDebug>

#define DEBUG_ARTICLE

/**
 * @enum ChampArticle
 * @brief Définit les différents champs pour une requête d'un article dans le stock
 */
enum ChampArticle
{
    TABLE_ARTICLE_ID_STOCK,
    TABLE_ARTICLE_ID_ARTICLE,
    TABLE_ARTICLE_NOM_ARTICLE,
    TABLE_ARTICLE_ID_TYPE,
    TABLE_ARTICLE_NOM_TYPE,
    TABLE_ARTICLE_ID_COMPTAGE,
    TABLE_ARTICLE_NOM_COMPTAGE,
    TABLE_ARTICLE_CODE_BARRE,
    TABLE_ARTICLE_DESIGNATION,
    TABLE_ARTICLE_QUANTITE,
    TABLE_ARTICLE_DISPONIBLE,
    TABLE_ARTICLE_POIDS,
    TABLE_ARTICLE_TARE,
    TABLE_ARTICLE_ID_UNITE,
    TABLE_ARTICLE_NOM_UNITE,
    TABLE_ARTICLE_NUMERO_CASIER
};

class Bdd;

/**
 * @class Article
 *
 * @brief La classe Article traite les articles
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 11 Mars 2020
 *
 */
class Article : public QObject
{
    Q_OBJECT
public:
    Article(QObject *parent = nullptr);
    ~Article();

    bool recupererDonneesArticle(QString idArticle, int numCasier=0);
    bool recupererDonneesArticleParNom(QString nomArticle, int numCasier=0);
    bool recupererDonneesArticleParNumeroCasier(QString numeroCasier);
    static unsigned int recupererNombreCasiersPourIdArticle(QString idArticle);
    static unsigned int recupererNombreCasiersPourNomArticle(QString nomArticle);
    static QVector<QString> recupererNumeroCasierPourIdArticle(QString idArticle);
    static QVector<QString> recupererNumeroCasierPourNomArticle(QString nomArticle);

    QString get(ChampArticle champ);
    void mettreAJourQuantite(QString quantite);

private:
    static Bdd *bdd;        //!< association d'un objet Bdd (accès à la base de données)
    QString idStock;        //!< idStock de l'article récupéré
    QString idArticle;      //!< idArticle de l'article récupéré
    QString nomArticle;     //!< nomArticle de l'article récupéré
    QString idType;         //!< idType de l'article récupéré
    QString nomType;        //!< nomType de l'article récupéré
    QString idComptage;     //!< idComptage de l'article récupéré
    QString nomComptage;    //!< nomComptage de l'article récupéré
    QString codeBarre;      //!< codeBarre de l'article récupéré
    QString designation;    //!< designation de l'article récupéré
    QString quantite;       //!< quantite de l'article récupéré
    QString disponible;     //!< disponibilité de l'article récupéré
    QString poidsArticle;   //!< poidsArticle de l'article récupéré
    QString tare;           //!< tare du numéro de casier de l'article récupéré
    QString idUnite;        //!< idUnite de l'article récupéré
    QString nomUnite;       //!< nomUnite de l'article récupéré
    QString numeroCasier;   //!< numeroCasier de l'article récupéré

private slots:    

signals:    

public slots:
};

#endif // ARTICLE_H
