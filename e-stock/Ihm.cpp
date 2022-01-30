#include "Ihm.h"
#include "ui_Ihm.h"
#include "Supervision.h"
#include "Armoire.h"
#include "Casier.h"
#include "Keyboard.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QString>

/**
 * @file Ihm.cpp
 *
 * @brief Définition de la classe Ihm
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
 * @brief Constructeur de la classe Ihm
 * @details Initialise un objet Ihm
 * @param parent
 */
Ihm::Ihm(QWidget *parent) : QMainWindow(parent), ui(new Ui::Ihm), supervision(new Supervision(this))
{
    ui->setupUi(this);
    // Suppression des parties inutile du QMainWindow
    delete ui->menuBar;
    delete ui->mainToolBar;
    delete ui->statusBar;

    // Récupère le clavier virtuelle
    keyboard = Keyboard::getInstance(nullptr, this);

    // Affiche la fenêtre par défaut en plein écran
    allerFenetreBadge();

    // Met la fenêtre en plein écran fenêtrer
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::FramelessWindowHint);
    // Pour la Raspberry Pi
    //showMaximized();

    connecterSignauxEtSlots();

    supervision->getInformationsArmoire();
    supervision->creerCasiers();
}

/**
 * @brief Destructeur de la classe Ihm
 * @details Détruit un objet Ihm
 */
Ihm::~Ihm()
{
    delete ui;
}

/**
 * @brief Définition de la méthode connecterSignauxEtSlots
 * @details Etablie la connexion entre les diffrents signaux et slots
 */
void Ihm::connecterSignauxEtSlots()
{
    // Les deux types d'authentifiaction
    connect(ui->lineBadge, SIGNAL(editingFinished()), this, SLOT(authentifierParBadge()));
    connect(ui->pushSeConnecter, SIGNAL(clicked()), this, SLOT(authentifierParIdentifiant()));
    connect(ui->pushSeDeconnecter, SIGNAL(clicked()), this, SLOT(deconnecterUtilisateur()));
    connect(ui->lineIdentifiant, SIGNAL(editingFinished()), this, SLOT(connecterClavier1()));
    connect(ui->lineMotDePasse, SIGNAL(editingFinished()), this, SLOT(connecterClavier2()));

    //fenêtre scanner un objet
    connect(ui->pushScannerObjet, SIGNAL(clicked()), this, SLOT(allerFenetreScannerObjet()));
    connect(ui->pushScannerSeDeconnecter, SIGNAL(clicked()), this, SLOT(deconnecterUtilisateur()));
    connect(ui->pushScannerPageStock, SIGNAL(clicked()), this, SLOT(allerFenetreMenu()));

    // Les deux fenêtres d'authentifiaction
    connect(ui->pushParIdentifiant, SIGNAL(clicked()), this, SLOT(allerFenetreIdentifiant()));
    connect(ui->pushParBadge, SIGNAL(clicked()), this, SLOT(allerFenetreBadge()));

    // Article
    connect(ui->lineRecherche, SIGNAL(textChanged(QString)), this, SLOT(activerRecherche()));
    connect(ui->pushRecherche, SIGNAL(clicked(bool)), this, SLOT(rechercherArticle()));

    // CodeBarre
    connect(ui->pushScannerPrendre, SIGNAL(clicked()), this, SLOT(appuyerBoutonPrendre()));
    connect(ui->pushScannerPrendre, SIGNAL(clicked()), this, SLOT(changerTextePrendre()));
    connect(ui->pushScannerAjouter, SIGNAL(clicked()), this, SLOT(appuyerBoutonAjouter()));
    connect(ui->pushScannerAjouter, SIGNAL(clicked()), this, SLOT(changerTexteAjouter()));
    connect(ui->spinBoxQuantiteScan, SIGNAL(valueChanged(int)), this, SLOT(afficherDemandeQuantite(int)));
    connect(ui->lineEditScanner, SIGNAL(editingFinished()), this, SLOT(scannerObjet()));
}

/**
 * @brief Définition de la méthode changerDeFenetre
 * @details Permet de changer de fenêtre sur l'ihm
 * @param fenetre
 */
void Ihm::changerDeFenetre(int fenetre)
{
    ui->stackedWidget->setCurrentIndex(fenetre);
}

/**
 * @brief Définition de la méthode placerCasier
 * @details gère l'affichage des casiers en fonction du nombre de ces derniers
 * @param casier
 */
void Ihm::placerCasier(Casier *casier)
{
    // pair/impair -> droite/gauche ?
    int numero = casier->getNumero() - 1;    
    if((numero+1)%2)
    {
        ui->gridLayoutCasiers->addWidget(casier, numero/2, 0, 1, 1);
        ui->gridLayoutCasiersScan->addWidget(casier, numero/2, 0, 1, 1);
    }
    else
    {
        ui->gridLayoutCasiers->addWidget(casier, numero/2, 1, 1, 1);
        ui->gridLayoutCasiersScan->addWidget(casier, numero/2, 1, 1, 1);
    }
}

void Ihm::placerCasiers(const QVector<Casier*> &casiers, int fenetre)
{
    for(int i=0; i < casiers.size(); i++)
    {
        Casier* casier = casiers[i];

        // pair/impair -> droite/gauche ?
        int numero = casier->getNumero() - 1;
        if((numero+1)%2)
        {
            switch (fenetre)
            {
            case FENETRE_MENU:
                ui->gridLayoutCasiers->addWidget(casier, numero/2, 0, 1, 1);
                break;
            case FENETRE_SCANNER_OBJET:
                ui->gridLayoutCasiersScan->addWidget(casier, numero/2, 0, 1, 1);
                break;
            default:
                break;
            }
        }
        else
        {
            switch (fenetre)
            {
            case FENETRE_MENU:
                ui->gridLayoutCasiers->addWidget(casier, numero/2, 1, 1, 1);
                break;
            case FENETRE_SCANNER_OBJET:
                ui->gridLayoutCasiersScan->addWidget(casier, numero/2, 1, 1, 1);
                break;
            default:
                break;
            }
        }
    }
}

/**
 * @brief Définition de la méthode afficherInformationsArmoire
 * @details Affiche les informations de l'armoire
 * @param informationsArmoire
 */
void Ihm::afficherInformationsArmoire(QStringList informationsArmoire)
{
    #ifdef DEBUG_IHM
        qDebug() << Q_FUNC_INFO << "informationsArmoire" << informationsArmoire;
    #endif
    ui->labelNomArmoire->setText(informationsArmoire.at(TABLE_ARMOIRE_NOM) + " " + informationsArmoire.at(TABLE_ARMOIRE_DESCRIPTION) + " (" + informationsArmoire.at(TABLE_ARMOIRE_NB_CASIERS+1) + ")");
    ui->labelNbCasiers->setText(informationsArmoire.at(TABLE_ARMOIRE_NB_CASIERS));
}

/**
 * @brief Définition de la méthode authentifierParBadge
 * @details Récupère le badge et l'envoie à la méthode permettant de traiter le badge
 */
void Ihm::authentifierParBadge()
{
    ui->labelErreurBadge->clear();

    if(ui->lineBadge->text() != "")
    {
        #ifdef DEBUG_IHM
            qDebug() << Q_FUNC_INFO << "Contenu brut badge" << ui->lineBadge->text();
        #endif

        QString trameBadge = ui->lineBadge->text();
        ui->lineBadge->clear();
        emit badgeDetecte(trameBadge);
    }
}

/**
 * @brief Définition de la méthode authentifierParIdentifiant
 * @details Récupère les identifiants et l'envoie à la méthode permettant de s'authentifier par identifiant
 */
void Ihm::authentifierParIdentifiant()
{
    if(ui->lineIdentifiant->text() != "")
    {
        #ifdef DEBUG_IHM
            qDebug() << Q_FUNC_INFO << "Identifiant" << ui->lineIdentifiant->text() << "MotDePasse" << ui->lineMotDePasse->text();
        #endif

        QString identifiant = ui->lineIdentifiant->text();
        QString motDePasse = ui->lineMotDePasse->text();
        ui->lineIdentifiant->clear();
        ui->lineMotDePasse->clear();
        emit identifiantDetecte(identifiant, motDePasse);
    }
}

/**
 * @brief Définition de la méthode deconnecterUtilisateur
 * @details Permet de déconnecter l'utilisateur
 */
void Ihm::deconnecterUtilisateur()
{
    supervision->deconnecterUtilisateur();
    changerDeFenetre(FENETRE_BADGE);
}

/**
 * @brief Définition de la méthode allerFenetreBadge
 * @details Permet de se rendre à la fenêtre badge
 */
void Ihm::allerFenetreBadge()
{
    changerDeFenetre(FENETRE_BADGE);
    ui->lineBadge->setFocus();
}

/**
 * @brief Définition de la méthode allerFenetreIdentifiant
 * @fn Ihm::allerFenetreIdentifiant()
 * @details Permet de se rendre à la fenêtre identifiant
 */
void Ihm::allerFenetreIdentifiant()
{
    ui->lineMotDePasse->setEchoMode(QLineEdit::Password);
    keyboard->setLineEdit(ui->lineIdentifiant);
    changerDeFenetre(FENETRE_IDENTIFIANT);
    ui->lineIdentifiant->setFocus();
}

/**
 * @brief Définition de la méthode allerFenetreMenu
 * @details Permet de se rendre à la fenêtre menu
 */
void Ihm::allerFenetreMenu()
{
    changerDeFenetre(FENETRE_MENU);
    // Initialisation widgets    
    placerCasiers(supervision->getCasiers(), FENETRE_MENU);
    keyboard->setLineEdit(ui->lineRecherche);
    ui->pushRecherche->setFocus();
    ui->comboBoxArticle->clear();
    ui->comboBoxArticle->addItem("Sélectionner un article");
    ui->pushRecherche->setEnabled(false);
    ui->lineRecherche->setFocus();
    // Lance une recherche de tous les articles
    emit rechercheArticle("");
}

/**
 * @brief Définition de la méthode allerFenetreScannerObjet
 * @details Permet de se rendre à la fenêtre scannerObjet
 */
void Ihm::allerFenetreScannerObjet()
{
    placerCasiers(supervision->getCasiers(), FENETRE_SCANNER_OBJET);
    ui->pushScannerPrendre->setEnabled(false);
    ui->pushScannerAjouter->setEnabled(false);
    ui->spinBoxQuantiteScan->setValue(0);
    ui->spinBoxQuantiteScan->setEnabled(false);
    changerDeFenetre(FENETRE_SCANNER_OBJET);
}

/**
 * @brief Définition de la méthode afficherErreurBadge
 * @details Affiche ErreurBadge
 * @param message
 */
void Ihm::afficherErreurBadge(QString message)
{
    ui->labelErreurBadge->setText(message);
}

/**
 * @brief Définition de la méthode afficherErreurDepassementQuantite
 * @details Affiche que la quantite est dépasser
 */
void Ihm::afficherErreurDepassementQuantite()
{
    QMessageBox::critical(nullptr, APPLICATION, QString::fromUtf8(MESSAGE_ERREUR_DEPASSEMENT_QUANTITE));
}

/**
 * @brief Définition de la méthode traiterDemandeDeConnexion
 * @details traite la demande de connexion
 * @param reponse
 * @param message
 */
void Ihm::traiterDemandeDeConnexion(bool reponse, QString message)
{
    if(reponse)
    {
        allerFenetreMenu();
    }
    else
    {
        QMessageBox::critical(nullptr, APPLICATION, message);
    }
}

/**
 * @brief Définition de la méthode traiterDemandeDeConnexion
 * @details traite la demande de connexion
 */
void Ihm::activerRecherche()
{
    if(!ui->lineRecherche->text().isEmpty())
        ui->pushRecherche->setEnabled(true);
    else
        ui->pushRecherche->setEnabled(false);
}

/**
 * @brief Définition de la méthode rechercherArticle
 * @details récupère l'article à rechercher et l'envoie à la méthode qui effectue la recherche
 */
void Ihm::rechercherArticle()
{
    if(!ui->lineRecherche->text().isEmpty())
        supervision->rechercherArticle(ui->lineRecherche->text());
}

/**
 * @brief Définition de la méthode effacerRechercheArticle
 * @details efface la recherche de l'article
 */
void Ihm::effacerRechercheArticle()
{
    ui->lineRecherche->setText("");
}

/**
 * @brief Définition de la méthode mettreAJourListeArticles
 * @details Mets à jour la liste des articles
 * @param articlesTrouves
 */
void Ihm::mettreAJourListeArticles(QVector<QStringList> articlesTrouves)
{
    #ifdef DEBUG_IHM
        qDebug() << Q_FUNC_INFO << "articlesTrouves" << articlesTrouves.size() << articlesTrouves;
    #endif
    creerListeArticles(articlesTrouves);

    effacerRechercheArticle();
}

/**
 * @brief Définition de la méthode selectionnerArticle
 * @details selectionne un Article
 * @param index
 */
void Ihm::selectionnerArticle(int index)
{
    #ifdef DEBUG_IHM
        qDebug() << Q_FUNC_INFO << "index" << index << ui->comboBoxArticle->currentText();
    #endif

    supervision->selectionnerArticle(ui->comboBoxArticle->currentText());
}

/**
 * @brief Définition de la méthode afficherDonneesArticleSelectionne
 * @details Affiche les données de l'article sélectionnée
 * @param donneesArticle
 */
void Ihm::afficherDonneesArticleSelectionne(QStringList donneesArticle)
{
    ui->labelCasier->setText("Casier :");
    ui->labelQuantiteNombre->setText(donneesArticle.at(ARTICLE_QUANTITE));
    ui->labelDisponibleNombre->setText(donneesArticle.at(ARTICLE_DISPONIBLE));
    ui->labelCasierNombre->setText(donneesArticle.at(NUMERO_CASIERS));
}

/**
 * @brief Définition de la méthode afficherDonneesArticleSelectionne
 * @details Affiche les données des articles sélectionnés
 * @param donneesArticle
 */
void Ihm::afficherDonneesArticleSelectionne(QVector<QStringList> donneesArticle)
{
    if(donneesArticle.size() <= 0)
        return;
    unsigned int articleQuantite = 0;
    unsigned int articleDisponible = 0;
    QString casiersQuantite;
    QString casiersDisponible;
    QString casiers;
    int nombreCasiers = donneesArticle.size();

    for(int i = 0; i < nombreCasiers; i++)
    {
        #ifdef DEBUG_IHM
            qDebug() << Q_FUNC_INFO << "disponible" << (donneesArticle[i].at(ARTICLE_DISPONIBLE)).toUInt();
            qDebug() << Q_FUNC_INFO << "articleDisponible" << articleDisponible;
            qDebug() << Q_FUNC_INFO << "quantite" << (donneesArticle[i].at(ARTICLE_QUANTITE)).toUInt();
            qDebug() << Q_FUNC_INFO << "articleQuantite" << articleQuantite;
        #endif
        articleDisponible += (donneesArticle[i].at(ARTICLE_DISPONIBLE)).toUInt();
        articleQuantite += (donneesArticle[i].at(ARTICLE_QUANTITE)).toUInt();

        if(i == 0)
        {
            casiers = donneesArticle[i].at(NUMERO_CASIERS);
            casiersDisponible = QString(" (") + donneesArticle[i].at(ARTICLE_DISPONIBLE);
            casiersQuantite = QString(" (") + donneesArticle[i].at(ARTICLE_QUANTITE);
        }
        else
        {
            casiers += " et " + donneesArticle[i].at(NUMERO_CASIERS);
            casiersDisponible += QString(" et ") + donneesArticle[i].at(ARTICLE_DISPONIBLE);
            casiersQuantite += QString(" et ") + donneesArticle[i].at(ARTICLE_QUANTITE);
        }
    }
    casiersDisponible += QString(")");
    casiersQuantite += QString(")");

    ui->labelCasier->setText("Casiers :");
    ui->labelQuantiteNombre->setText(QString::number(articleQuantite) + casiersQuantite);
    ui->labelDisponibleNombre->setText(QString::number(articleDisponible) + casiersDisponible);
    ui->labelCasierNombre->setText(casiers);
}

/**
 * @brief Définition de la méthode creerListeArticles
 * @details Crée la liste déroulante contenant les articles issus d'une recherche
 * @param articlesTrouves
 */
void Ihm::creerListeArticles(const QVector<QStringList> &articlesTrouves)
{
    disconnect(ui->comboBoxArticle, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionnerArticle(int)));
    ui->comboBoxArticle->clear();

    ui->comboBoxArticle->addItem("Sélectionner un article");
    for(int i = 0 ; i < articlesTrouves.size() ; i++)
    {
        if(ui->comboBoxArticle->findText(articlesTrouves[i].at(2)) == -1)
        {
            ui->comboBoxArticle->addItem(articlesTrouves[i].at(2));
        }
    }
    connect(ui->comboBoxArticle, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionnerArticle(int)));
}

/**
 * @brief Définition de la méthode appuyerBoutonPrendre
 * @details slot pour quand l'on appuie sur le bouton prendre
 */
void Ihm::appuyerBoutonPrendre()
{
    ui->labelScannerObjet->setText("Vous pouvez indiquer une quantité");
    ui->lineEditScanner->setFocus();
    emit boutonPrendre(false);    
}

/**
 * @brief Définition de la méthode appuyerBoutonAjouter
 * @details slot pour quand l'on appuie sur le bouton ajouter
 */
void Ihm::appuyerBoutonAjouter()
{
    ui->labelScannerObjet->setText("Vous pouvez indiquer une quantité");
    ui->lineEditScanner->setFocus();
    emit boutonAjouter(true);
}

/**
 * @brief définition de la méthode changerTextePrendre
 * @details slot pour changer le texte afficher quand on appuie sur le bouton prendre
 */
void Ihm::changerTextePrendre()
{
    ui->labelScannerObjet->setText("Veuillez scanner l'objet à prendre");
}

/**
 * @brief définition de la méthode changerTexteAjouter
 * @details slot pour changer le texte afficher quand on appuie sur le bouton ajouter
 */
void Ihm::changerTexteAjouter()
{
    ui->labelScannerObjet->setText("Veuillez scanner l'objet à ajouter");
}

/**
 * @brief Définition de la méthode objetScanner
 * @details slot pour quand un code barre a était scanné
 */
void Ihm::scannerObjet()
{
    QString codeBarreObjet = ui->lineEditScanner->text();
    qDebug() << Q_FUNC_INFO << "codeBarreObjet" << codeBarreObjet;

    if(!ui->lineEditScanner->text().isEmpty())
    {
        emit codeBarreObjetScanner(codeBarreObjet);
    }

    ui->lineEditScanner->setText("");
}

/**
 * @brief définition de la méthode afficherDemandeQuantite
 * @details methode permettant d'effectuer la saisie de la quantité
 * @param quantite
 */
void Ihm::afficherDemandeQuantite(int quantite)
{
    emit envoyerQuantite(quantite);
    ui->lineEditScanner->setFocus();
}

/**
 * @brief définition de la méthode afficherErreurArticleInsuffisants
 * @details methode permettant d'afficher que le nombre d'article est insuffisant
 */
void Ihm::afficherErreurArticleInsuffisants()
{
    QMessageBox::critical(nullptr, APPLICATION, QString::fromUtf8(MESSAGE_ERREUR_QUANTITE_INSUFFISANTE));
}

/**
 * @brief définition de la méthode afficherErreurAucunCasierOuvert
 * @details methode permettant d'afficher que aucun casier n'est ouvert
 */
void Ihm::afficherErreurAucunCasierOuvert()
{
    QMessageBox::critical(nullptr, APPLICATION, QString::fromUtf8(MESSAGE_ERREUR_AUCUN_CASIER_OUVERT));
}

/**
 * @brief définition de la méthode afficherErreurPasArticleAvecCodeBarre
 * @details methode permettant d'afficher que aucun article de correspond à ce code-barres
 */
void Ihm::afficherErreurPasArticleAvecCodeBarre()
{
    QMessageBox::critical(nullptr, APPLICATION, QString::fromUtf8(MESSAGE_ERREUR_AUCUN_ARTICLE_CODE_BARRE));
}

/**
 * @brief Définition de la méthode gererPageScanObjet
 * @details méthode qui permet de gérer la possibilité de scanner un objet
 * @param numeroCasier
 * @param etat
 */
void Ihm::gererPageScanObjet(int numeroCasier, bool etat)
{
    if(etat)
    {
        ui->labelScannerObjet->setText("Veuillez prendre ou ajouter un objet");
        ui->pushScannerPrendre->setEnabled(true);
        ui->pushScannerAjouter->setEnabled(true);
        ui->spinBoxQuantiteScan->setEnabled(true);
        ui->spinBoxQuantiteScan->setValue(1); // par défaut
        ui->lineEditScanner->setEnabled(true);
        ui->lineEditScanner->setFocus();
    }
    else
    {
        ui->labelScannerObjet->setText("Veuillez ouvrir un casier");
        ui->pushScannerPrendre->setEnabled(false);
        ui->pushScannerAjouter->setEnabled(false);
        ui->spinBoxQuantiteScan->setValue(0);
        ui->spinBoxQuantiteScan->setEnabled(false);
        ui->lineEditScanner->setEnabled(false);
    }
}

/**
 * @brief définition de la méthode connecterClavier1
 * @details methode permettant de connecter le clavier
 */
void Ihm::connecterClavier1()
{
    keyboard->setLineEdit(ui->lineMotDePasse);
}

/**
 * @brief définition de la méthode connecterClavier2
 * @details methode permettant de connecter le clavier
 */
void Ihm::connecterClavier2()
{
    keyboard->setLineEdit(ui->lineIdentifiant);
}
