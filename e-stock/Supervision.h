#ifndef SUPERVISION_H
#define SUPERVISION_H

/**
 * @file Supervision.h
 *
 * @brief Déclaration de la classe Supervision
 *
 * @author Legger Pierre-Antoine
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 12 Février 2020
 *
 */

#include <QObject>
#include <QString>
#include <QVector>

#define DEBUG_SUPERVISION
//#define CHANGE_PASSWORD_BEFORE
#define SUPERVISION_TEST_POIDS

#define PRECISION 0

class Armoire;
class Article;
class Ihm;
class Bdd;
class CodeBarre;
class Communication;
class Rfid;
class Utilisateur;
class Communication;
class Casier;

/**
 * @class Supervision
 *
 * @brief La classe Supervision permet de superviser l'ensemble de l'application
 *
 * @author Legger Pierre-Antoine
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 12 Février 2020
 *
 */
class Supervision : QObject
{
    Q_OBJECT

public:
    Supervision(Ihm *parent = nullptr);
    ~Supervision();

    void deconnecterUtilisateur();
    void creerCasiers();
    QStringList getInformationsArmoire();
    QVector<Casier*> getCasiers();

public slots:
    void verifierAuthentificationBadge(QString badge);
    void verifierAuthentificationIdentifiant(QString identifiant, QString motDePasse);
    void rechercherArticle(QString recherche);
    void selectionnerArticle(QString nomArticle);
    void traiterTramePoids(QString trame);
    void prendreObjetAvecCodeBarre(QString codeBarre);
    void ajouterObjetAvecCodeBarre(QString codeBarre);    

signals:
    void reponseDemandeDeConnexion(bool, QString);
    void erreurDepassementQuantite();
    void articlesTrouves(QVector<QStringList>);
    void donneesArticleSelectionne(QVector<QStringList>);
    void donneesArticleSelectionne(QStringList);
    void erreurArticleInsuffisants();
    void erreurQuantiteTropElevee();
    void erreurAucunCasierOuvert();
    void erreurAucunArticleAvecCodeBarre();

private:
    Ihm *ihm; //!< association d'un objet Ihm (fenêtre princiaple de l'application)
    Bdd *bdd; //!< association d'un objet Bdd (accès à la base de données)
    Rfid *rfid; //!< association d'un objet Rfid (le lecteur de badge)
    Utilisateur *utilisateur; //!< association d'un objet Utilisateur (l'utilisateur authentifié)
    CodeBarre *codeBarre; //!< association d'un objet CodeBarre
    Armoire *armoire; //!< association d'un objet Armoire
    Communication *communication; //!< association d'un objet Communication
    QVector<Casier*> casiers; //!< les casiers de l'armoire

    void connecterSignauxSlots();

    // Athentification
    QStringList recupererDonneesUtilisateur(QString requeteBDD);
    void crypterMotDepasse(QString &motDePasse);
    bool verifierDateValidite(QString stringDateValidite);
    bool verifierDonneesUtilisateur(QStringList &donnees);
    void connecterUtilisateur(QStringList &donnees);

    QString extrairePoids(QString trame);
    QString extraireNumeroCasier(QString trame);
    int compter(QString poidArticle, QString poidTotal, QString tare);
    unsigned int comptageAutomatiqueAjouterOuPrendre(QString nbArticleAvant, int nbArticleApres);
    unsigned int comptageAutomatiqueQuantite(QString nbArticleAvant, int nbArticleApres);
    void mettreAJourMouvement(QString idUtilisateur, QString idStock, QString idAction, QString quantite);

    void ajouterDonneesArticle(Article *article, QVector<QStringList> &donneesArticle, QStringList &donnees);

    bool verifierArticlePresentDansCasier(QString numCasier, QString idArticle);

    QString recupererHorodatage();
    QString recupererIdStockAvecNumeroCasier(int numeroCasier);
    int rechercherCasierOuvert();

};

#endif // SUPERVISION_H
