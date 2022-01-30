#ifndef IHM_H
#define IHM_H

#define DEBUG_IHM

/**
 * @file Ihm.h
 *
 * @brief Déclaration de la classe Ihm
 *
 * @author Legger Pierre-Antoine
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 12 Février 2020
 *
 */

/**
 * @enum FenetresIHM
 * @brief Définit les différents types de fenêtres de l'application
 */
enum FenetresIHM
{
    FENETRE_BADGE,          //!< Fenêtre d'authentifiaction par badge
    FENETRE_IDENTIFIANT,    //!< Fenêtre d'authentifiaction par identifiant
    FENETRE_MENU,           //!< Fenêtre ???
    FENETRE_SCANNER_OBJET   //!< Fenêtre pour scan d'un objet
};

#define APPLICATION "e-stock"
#define MESSAGE_ERREUR_UTILISATEUR_NON_VALIDE "Erreur : utilisateur non valide !"
#define MESSAGE_ERREUR_UTILISATEUR_DATE_NON_VALIDE "Erreur : le compte n'est plaus valide !"
#define MESSAGE_ERREUR_DEPASSEMENT_QUANTITE "Opération impossible, le nombre d'articles maximum serait dépassé"
#define MESSAGE_ERREUR_AUCUN_ARTICLE_CODE_BARRE "Erreur, aucun article ne correspond à ce code barre dans le casier"
#define MESSAGE_ERREUR_QUANTITE "Erreur, quantite non valide"
#define MESSAGE_ERREUR_QUANTITE_INSUFFISANTE "Erreur, pas assez d'articles disponible"
#define MESSAGE_ERREUR_AUCUN_CASIER_OUVERT "Erreur, vous devez d'avord ouvrir le casier dans lequelle vous souhaiteé effectuer vos actions!"

#define ARTICLE_QUANTITE 0
#define ARTICLE_DISPONIBLE 1
#define NUMERO_CASIERS 2

#include <QtWidgets>

class Supervision;
class Casier;
class Keyboard;

namespace Ui {
class Ihm;
}

/**
 * @class Ihm
 *
 * @brief Déclaration de la classe Ihm
 *
 * @author Legger Pierre-Antoine
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Vendredi 12 Février 2020
 *
 */
class Ihm : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ihm(QWidget *parent = nullptr);
    ~Ihm();

    void changerDeFenetre(int fenetre);
    void placerCasier(Casier *casier);
    void placerCasiers(const QVector<Casier*> &casiers, int fenetre);

private slots:
    void afficherInformationsArmoire(QStringList informationsArmoire);
    void authentifierParBadge();
    void authentifierParIdentifiant();
    void deconnecterUtilisateur();
    void allerFenetreBadge();
    void allerFenetreIdentifiant();    
    void allerFenetreMenu();
    void allerFenetreScannerObjet();
    void afficherErreurBadge(QString message);
    void afficherErreurDepassementQuantite();
    void traiterDemandeDeConnexion(bool reponse, QString message);
    void activerRecherche();
    void rechercherArticle();
    void effacerRechercheArticle();
    void mettreAJourListeArticles(QVector<QStringList> articlesTrouves);
    void selectionnerArticle(int index);
    void afficherDonneesArticleSelectionne(QStringList donneesArticle);
    void afficherDonneesArticleSelectionne(QVector<QStringList> donneesArticle);
    void appuyerBoutonPrendre();
    void appuyerBoutonAjouter();
    void changerTextePrendre();
    void changerTexteAjouter();
    void scannerObjet();
    void afficherDemandeQuantite(int);
    void afficherErreurArticleInsuffisants();
    void afficherErreurAucunCasierOuvert();
    void afficherErreurPasArticleAvecCodeBarre();
    void gererPageScanObjet(int numeroCasier, bool etat);
    void connecterClavier1();
    void connecterClavier2();

signals:
    void badgeDetecte(QString);
    void identifiantDetecte(QString identifiant, QString motDePasse);
    void rechercheArticle(QString);
    void articleSelectionne(QString);
    void boutonPrendre(bool);
    void boutonAjouter(bool);
    void codeBarreObjetScanner(QString);
    void envoyerQuantite(int);

private:
    Ui::Ihm *ui; //!< contenu de l'interface utilisateur
    Supervision *supervision; //!< association vers supervision
    Keyboard *keyboard; //!< association vers le clavier virtuel

    void creerListeArticles(const QVector<QStringList> &articlesTrouves);
    void connecterSignauxEtSlots();
};

#endif // IHM_H
