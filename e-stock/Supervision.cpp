#include "Supervision.h"
#include "Ihm.h"
#include "Bdd.h"
#include "CodeBarre.h"
#include "Communication.h"
#include "Rfid.h"
#include "Utilisateur.h"
#include "Armoire.h"
#include "Article.h"
#include "Communication.h"
#include "Casier.h"
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QCryptographicHash>

/**
 * @file Supervision.cpp
 *
 * @brief Définition de la classe Supervision
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
 * @brief Définition du constructeur de la classe Supervision
 * @details Initialise la supervision
 * @param parent l'objet QObject parent
 */
Supervision::Supervision(Ihm *parent) : QObject(parent), ihm(parent)
{
    // Instancie les objets dont la classe Supervision coordonne les actions
    bdd = Bdd::getInstance();
    bdd->connecter();
    codeBarre = new CodeBarre(this);
    //portSerie = new Communication(this);
    rfid = new Rfid(this);
    utilisateur = nullptr;
    armoire = new Armoire(this);
    communication = new Communication(this);

    connecterSignauxSlots();

    communication->demarrerCommunicationPort();

}

/**
 * @brief Définition du destructeur de Supervision
 * @fn Supervision::~Supervision()
 * @details Détruit un objet Supervision
 */
Supervision::~Supervision()
{

}

/**
 * @brief Méthode qui permet la déconnexion de l'utilisateur
 * @fn Supervision::deconnecterUtilisateur()
 * @details Supprime les données de l'utilisateur
 */
void Supervision::deconnecterUtilisateur()
{
    if(utilisateur != nullptr)
    {
        delete utilisateur;
        utilisateur = nullptr;
    }
}

/**
 * @brief Définition de la méthode creerCasiers
 * @fn Supervision::creerCasiers()
 * @details Méthode qui crée les casiers à gérer
 */
void Supervision::creerCasiers()
{
    QString nbCasiers = armoire->getNbCasiers();
    qDebug() << Q_FUNC_INFO << "nbCasiers" << nbCasiers;
    if(!nbCasiers.isEmpty())
    {
        for(int i=0; i < nbCasiers.toInt(); i++)
        {
            Casier* casier = new Casier(i+1, ihm);            
            connect(casier, SIGNAL(estOuvert(int,bool)), ihm, SLOT(gererPageScanObjet(int,bool)));
            casiers.push_back(casier);
        }
    }
    ihm->placerCasiers(casiers, FENETRE_MENU);
}

/**
 * @brief Définition de la méthode getInformationsArmoire
 * @fn Supervision::getInformationsArmoire()
 * @details Récupère les informations (nom, ...) sur l'armoire
 * @return les informations (nom, ...) sur l'armoire sous la forme d'un QStringList
 */
QStringList Supervision::getInformationsArmoire()
{
    QStringList informationsArmoire = armoire->getInformations();

    return informationsArmoire;
}

/**
 * @brief Définition de la méthode getCasiers
 * @details Renvoie les casiers
 * @return QVector<Casier*> casiers
 */
QVector<Casier*> Supervision::getCasiers()
{
    return casiers;
}

/**
 * @brief Définition de la méthode verifierAuthentificationBadge
 * @fn Supervision::verifierAuthentificationBadge(QString badge)
 * @details Permet la vérification des données utilisateur par badge
 * @param badge Chaîne de caractères de l'uid du badge
 */
void Supervision::verifierAuthentificationBadge(QString badge)
{
    QString requeteBDD = "SELECT * from Utilisateur where Badge = '" + badge + "';";
    QStringList donnees = recupererDonneesUtilisateur(requeteBDD);
    if(verifierDonneesUtilisateur(donnees))
    {
        connecterUtilisateur(donnees);
    }
}

/**
 * @brief Définition de la méthode verifierAuthentificationIdentifiant
 * @fn Supervision::verifierAuthentificationIdentifiant(QString identifiant, QString motDePasse)
 * @details Permet la vérification des données utilisateur par champs
 * @param identifiant Chaîne de caractères de l'identifiant
 * @param motDePasse Chaîne de caractères du mot de passe
 */
void Supervision::verifierAuthentificationIdentifiant(QString identifiant, QString motDePasse)
{
    this->crypterMotDepasse(motDePasse);

    #ifdef CHANGE_PASSWORD_BEFORE
    QString requete = QString("UPDATE Utilisateur SET MotDePasse='%1' WHERE Identifiant='%2'").arg(motDePasse).arg(identifiant);
    bdd->executer(requete);
    #endif

    QString requeteBDD = "SELECT * from Utilisateur where Identifiant =  '" + identifiant + "' && MotDePasse = '" + motDePasse + "';";
    QStringList donnees = recupererDonneesUtilisateur(requeteBDD);
    if(verifierDonneesUtilisateur(donnees))
    {
        connecterUtilisateur(donnees);
    }
}

/**
 * @brief Définition de la méthode recupererDonneesUtilisateur
 * @fn Supervision::recupererDonneesUtilisateur(QString requeteBDD)
 * @details Récupére des données utilisateur dans la base de donnéess
 * @param requeteBDD
 * @return La liste des données utilisateur
 */
QStringList Supervision::recupererDonneesUtilisateur(QString requeteBDD)
{
    QStringList donnees;
    bdd->recuperer(requeteBDD, donnees);

    return donnees;
}

/**
 * @brief Définition de la méthode crypterMotDepasse
 * @fn Supervision::crypterMotDepasse(QString &motDePasse)
 * @details Crypte le mots de passe avec la méthode Md5 puis vers l'hexadécimal
 * @param motDePasse Chaîne de caractères du mot de passe
 */
void Supervision::crypterMotDepasse(QString &motDePasse)
{
    if(!motDePasse.isEmpty())
    {
    motDePasse = QString(QCryptographicHash::hash((motDePasse).toLatin1(), QCryptographicHash::Md5).toHex());
    }

    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "Mot de passe crypte" << motDePasse;
    #endif
}

/**
 * @brief Définition de la méthode verifierDateValidite
 * @fn Supervision::verifierDateValidite(QString stringDateValidite)
 * @details Permet de vérifier la date de validité
 * @param stringDateValidite Chaîne de caractères de la date de validité
 * @return Si la date de validité est valide
 */
bool Supervision::verifierDateValidite(QString stringDateValidite)
{
    // Verification de la date de validité
    QDate dateValidite = dateValidite.fromString(stringDateValidite,"yyyy-MM-dd");
    QDate dateActuelle = QDate::currentDate();

    #ifdef DEBUG_SUPERVISION
        qDebug() << "Date actuelle" << dateActuelle;
        qDebug() << "Date validité" << dateValidite;
    #endif

    if(dateActuelle <= dateValidite)
    {
        return true;
    }

    return false;
}

/**
 * @brief Définition de la méthode verifierDonneesUtilisateur
 * @fn Supervision::verifierDonneesUtilisateur(QStringList &donnees)
 * @details Vérfie la date de validité et que les données ne sont pas vide sinon renvoie des erreurs
 * @param donnees Chaîne de caractères de la date de validité
 * @return Si la demande de connexion est autoriser
 */
bool Supervision::verifierDonneesUtilisateur(QStringList &donnees)
{
    #ifdef DEBUG_SUPERVISON
        qDebug() << Q_FUNC_INFO << donnees;
    #endif

    if(!donnees.isEmpty())
    {
        if(verifierDateValidite(donnees.at(TABLE_UTILISATEUR_DATE_VALIDITE)))
        {
            emit reponseDemandeDeConnexion(true, "");
            return true;
        }
        else
        {
            emit reponseDemandeDeConnexion(false, MESSAGE_ERREUR_UTILISATEUR_DATE_NON_VALIDE);
            return false;
        }
    }
    else
    {
        emit reponseDemandeDeConnexion(false, MESSAGE_ERREUR_UTILISATEUR_NON_VALIDE);
        return false;
    }
}

/**
 * @brief Définition de la méthode connecterUtilisateur
 * @fn Supervision::connecterUtilisateur(QStringList &donnees)
 * @details Connecte l'utilisateur et le supprime si il en existe un
 * @param donnees Plusieurs chaînes de caractères des données utilisateur
 */
void Supervision::connecterUtilisateur(QStringList &donnees)
{
    if(utilisateur != nullptr)
    {
        deconnecterUtilisateur();
    }
    utilisateur = new Utilisateur(donnees, this);
    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << utilisateur->getIdentifiantUtilisateur() << "authentifié";
    #endif

    #ifdef SUPERVISION_TEST_POIDS
        QString trameTest = "CASIERS;3;2;1745";
        traiterTramePoids(trameTest);
    #endif
}

/**
 * @brief Définition de la méthode connecterSignauxSlots
 * @fn Supervision::connecterSignauxSlots()
 * @details Etablie la connexion entre les diffrents signaux et slots
 */
void Supervision::connecterSignauxSlots()
{
    // Armoire
    connect(armoire, SIGNAL(informationsArmoire(QStringList)), ihm, SLOT(afficherInformationsArmoire(QStringList)));

    // Authentification Badge
    connect(ihm, SIGNAL(badgeDetecte(QString)), rfid, SLOT(traiterBadge(QString)));
    connect(rfid, SIGNAL(erreurBadgeInvalide(QString)), ihm, SLOT(afficherErreurBadge(QString)));
    connect(rfid, SIGNAL(nouveauUidBadge(QString)), this, SLOT(verifierAuthentificationBadge(QString)));

    // Authentification Identifiant
    connect(ihm, SIGNAL(identifiantDetecte(QString, QString)), this, SLOT(verifierAuthentificationIdentifiant(QString, QString)));

    // Authentification Utilisateur
    connect(this, SIGNAL(reponseDemandeDeConnexion(bool,QString)), ihm, SLOT(traiterDemandeDeConnexion(bool,QString)));

    // Article
    connect(communication, SIGNAL(envoieTramePoids(QString)), this, SLOT(traiterTramePoids(QString)));
    connect(ihm, SIGNAL(rechercheArticle(QString)), this, SLOT(rechercherArticle(QString)));
    connect(this, SIGNAL(articlesTrouves(QVector<QStringList>)), ihm, SLOT(mettreAJourListeArticles(QVector<QStringList>)));
    connect(ihm, SIGNAL(articleSelectionne(QString)), this, SLOT(selectionnerArticle(QString)));
    connect(this, SIGNAL(donneesArticleSelectionne(QStringList)), ihm, SLOT(afficherDonneesArticleSelectionne(QStringList)));
    connect(this, SIGNAL(donneesArticleSelectionne(QVector<QStringList>)), ihm, SLOT(afficherDonneesArticleSelectionne(QVector<QStringList>)));
    connect(this, SIGNAL(erreurDepassementQuantite()), ihm, SLOT(afficherErreurDepassementQuantite()));

    // CodeBarre    
    connect(ihm, SIGNAL(boutonPrendre(bool)), codeBarre, SLOT(prendreOuAjouter(bool)));
    connect(ihm, SIGNAL(boutonAjouter(bool)), codeBarre, SLOT(prendreOuAjouter(bool)));
    connect(ihm, SIGNAL(codeBarreObjetScanner(QString)), codeBarre, SLOT(traiterCodeBarre(QString)));
    connect(this, SIGNAL(erreurAucunArticleAvecCodeBarre()), ihm, SLOT(afficherErreurPasArticleAvecCodeBarre()));
    connect(codeBarre, SIGNAL(prendreObjet(QString)), this, SLOT(prendreObjetAvecCodeBarre(QString)));
    connect(codeBarre, SIGNAL(ajouterObjet(QString)), this, SLOT(ajouterObjetAvecCodeBarre(QString)));
    connect(ihm, SIGNAL(envoyerQuantite(int)), codeBarre, SLOT(changerQuantiteObjet(int)));
    connect(this, SIGNAL(erreurArticleInsuffisants()), ihm, SLOT(afficherErreurArticleInsuffisants()));
    connect(this, SIGNAL(erreurQuantiteTropElevee()), ihm, SLOT(afficherErreurDepassementQuantite()));
    connect(this, SIGNAL(erreurAucunCasierOuvert()), ihm, SLOT(afficherErreurAucunCasierOuvert()));
}

/**
 * @brief Définition de la méthode rechercherArticle
 * @fn Supervision::rechercherArticle()
 * @details Recherche un Article
 * @param recherche
 */
void Supervision::rechercherArticle(QString recherche)
{
    QString requete = "SELECT Stock.NumeroCasier, Article.idType, Article.Nom, Stock.Quantite, Stock.Disponible, Article.Designation FROM Stock INNER JOIN Article ON Stock.idArticle = Article.idArticle WHERE Article.Nom LIKE '%" + recherche + "%' OR Article.Code LIKE '%" + recherche + "%' OR Article.Designation LIKE '%" + recherche + "%' ORDER BY Stock.NumeroCasier ASC";

    QVector<QStringList> listeArticlesTrouves;
    bdd->recuperer(requete, listeArticlesTrouves);

    emit articlesTrouves(listeArticlesTrouves);
}

/**
 * @brief Définition de la méthode selectionnerArticle
 * @details sélectionne un article
 * @param nomArticle
 */
void Supervision::selectionnerArticle(QString nomArticle)
{
    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "Nom article" << nomArticle;
    #endif

    Article *article = new Article(this);
    QVector<QStringList> donneesArticle;
    QStringList donnees;

    unsigned int nombreCasiers = Article::recupererNombreCasiersPourNomArticle(nomArticle);
    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "nombreCasiers" << nombreCasiers;
    #endif

    if(nombreCasiers > 1)
    {
        QVector<QString> numeroDesCasiers;

        numeroDesCasiers = Article::recupererNumeroCasierPourNomArticle(nomArticle);

        for(int i = 0; i < numeroDesCasiers.size(); i++)
        {
            article->recupererDonneesArticleParNumeroCasier(numeroDesCasiers[i]);
            ajouterDonneesArticle(article, donneesArticle, donnees);
        }

        if(!donneesArticle.isEmpty())
        {
            emit donneesArticleSelectionne(donneesArticle);
        }
    }
    else
    {
        article->recupererDonneesArticleParNom(nomArticle);
        ajouterDonneesArticle(article, donneesArticle, donnees);

        if(!donneesArticle.isEmpty())
        {
            emit donneesArticleSelectionne(donneesArticle.at(0));
        }
    }
}

/**
 * @brief Définition de la méthode traiterTramePoids
 * @fn Supervision::traiterTramePoids()
 * @details traite la trame poids reçue
 * @param trame
 */
void Supervision::traiterTramePoids(QString trame)
{
    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << trame;
    #endif

    QString numCasier = extraireNumeroCasier(trame);

    Article *article = new Article(this);
    if(article->recupererDonneesArticleParNumeroCasier(numCasier))
    {
        #ifdef DEBUG_SUPERVISION
            qDebug() << Q_FUNC_INFO << "Article" << article->get(TABLE_ARTICLE_NOM_ARTICLE) << article->get(TABLE_ARTICLE_QUANTITE) << article->get(TABLE_ARTICLE_DISPONIBLE);
        #endif

        int nombreArticle = compter(article->get(TABLE_ARTICLE_POIDS), extrairePoids(trame), article->get(TABLE_ARTICLE_TARE));

        QString strArticleQuantite = article->get(TABLE_ARTICLE_QUANTITE);

        int articleQuantite = strArticleQuantite.toInt();

        if(nombreArticle > articleQuantite)
        {
            emit erreurDepassementQuantite();
        }
        else
        {
            article->mettreAJourQuantite(QString::number(nombreArticle));

            QString idUtilisateur = utilisateur->getIdUtilisateur();
            QString idStock = article->get(TABLE_ARTICLE_ID_STOCK);
            QString idAction = QString::number(comptageAutomatiqueAjouterOuPrendre(article->get(TABLE_ARTICLE_DISPONIBLE), nombreArticle));
            QString quantite = QString::number(comptageAutomatiqueQuantite(article->get(TABLE_ARTICLE_DISPONIBLE), nombreArticle));

            mettreAJourMouvement(idUtilisateur, idStock,  idAction, quantite);
        }
    }
    else
    {
        #ifdef DEBUG_SUPERVISION
            qDebug() << Q_FUNC_INFO << "Article introuvable !";
        #endif
    }
}

/**
 * @brief Définition de la méthode comptageAutomatiqueAjouterOuPrendre
 * @details permet de savoir si l'action effectuer est un retrait ou un ajout
 * @param QString nbArticleAvant
 * @param int nbArticleApres
 * @return unsigned int action
 */
unsigned int Supervision::comptageAutomatiqueAjouterOuPrendre(QString nbArticleAvant, int nbArticleApres)
{
    int intNbArticleAvant = nbArticleAvant.toInt();

    if(intNbArticleAvant > nbArticleApres)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief Définition de la méthode comptageAutomatiqueQuantite
 * @details permet de savoir combien d'article il y à d'article de différence
 * @param QString nbArticleAvant
 * @param int nbArticleApres
 * @return unsigned int quantite
 */
unsigned int Supervision::comptageAutomatiqueQuantite(QString nbArticleAvant, int nbArticleApres)
{
    int intNbArticleAvant = nbArticleAvant.toInt();

    if(intNbArticleAvant > nbArticleApres)
    {
        return intNbArticleAvant - nbArticleApres;
    }
    else
    {
        return nbArticleApres - intNbArticleAvant;
    }
}

/**
 * @brief Définition de la méthode extrairePoids
 * @fn Supervision::extrairePoids()
 * @details extrait le poids de l'article de la trame
 * @param trame
 * @return le poids de l'article sous forme d'un QString
 */
QString Supervision::extrairePoids(QString trame)
{
    QString poids = trame.section(';',3,3);

    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "poids:" << poids;
    #endif

    return poids;
}

/**
 * @brief Définition de la méthode extraireNumeroCasier
 * @fn Supervision::extraireNumeroCasier()
 * @details extrait le numéro de casier de la trame
 * @param trame
 * @return le numéro du casier
 */
QString Supervision::extraireNumeroCasier(QString trame)
{
    QString numCasier = trame.section(';',2,2);

    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "numCasier:" << numCasier;
    #endif

    return numCasier;
}

/**
 * @brief Définition de la méthode compter
 * @details assure le comptage automatique du nombre d'article présent dans le casier
 * @param poidsArticle le poids total dans le casier
 * @param poidsTotal le poids d'un article
 * @param tare
 * @return la quantite d'article présent dans le casier sous forme d'un entier
 */
int Supervision::compter(QString poidsArticle, QString poidsTotal, QString tare)
{
    double doublePoidsArticle = poidsArticle.toDouble();
    double doublePoidsTotal = poidsTotal.toDouble();
    double doubleTare = tare.toDouble();

    //comptage du nombre d'articles

    double doubleNombreArticle = qRound((doublePoidsTotal - doubleTare) / doublePoidsArticle);
    QString strNombreArticle = QString::number(doubleNombreArticle, 'f', PRECISION);
    int nombreArticle = strNombreArticle.toInt();

    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "nombreArticle:" << nombreArticle;
    #endif

    return nombreArticle;
}

/**
 * @brief Définition de la méthode recupererHorodatage
 * @details permet de récupérer la date est l'heure actuel
 * @return QString horodatage
 */
QString Supervision::recupererHorodatage()
{
    QDate qDate(QDate::currentDate());
    QString date = qDate.toString("yyyy-MM-dd");

    QTime time(QTime::currentTime());
    QString heure = time.toString("hh:mm:ss");

    return date + " " + heure;
}

/**
 * @brief Définition de la méthode mettreAJourMouvement
 * @details permet de mettre à jour les mouvements
 * @param QString idUtilisateur
 * @param QString idStock
 * @param QString idAction
 * @param QString quantite
 */
void Supervision::mettreAJourMouvement(QString idUtilisateur, QString idStock, QString idAction, QString quantite)
{
    QString horodatage = recupererHorodatage();
    QString requete ="INSERT INTO Mouvement(idUtilisateur, idStock, idAction, Quantite, Horodatage) VALUES('" + idUtilisateur + "', '" + idStock + "', '" + idAction + "', '" + quantite + "', '" + horodatage + "');";
    bdd->executer(requete);
}

/**
 * @brief Définition de la méthode ajouterDonneesArticle
 * @details permet d'ajouter des données d'un article d'un casier
 * @param article
 * @param donneesArticle
 * @param donnees
 */
void Supervision::ajouterDonneesArticle(Article *article, QVector<QStringList> &donneesArticle, QStringList &donnees)
{
    donnees << article->get(TABLE_ARTICLE_QUANTITE);
    donnees << article->get(TABLE_ARTICLE_DISPONIBLE);
    donnees << article->get(TABLE_ARTICLE_NUMERO_CASIER);

    donneesArticle.push_back(donnees);
    donnees.clear();
}

/**
 * @brief Définition de la méthode verifierArticlePresentDansCasier
 * @details permet de vérifier si l'article est bien présent dans le casier
 * @param QString numCasier
 * @param QString idArticle
 * @return bool articlePresent
 */
bool Supervision::verifierArticlePresentDansCasier(QString numCasier, QString idArticle)
{
    QString requete = "SELECT idArticle FROM Stock WHERE numeroCasier = "+ numCasier + ";";
    QString donnees;

    bdd->recuperer(requete, donnees);

    if(donnees == idArticle)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Définition de la méthode prendreObjetAvecCodeBarre
 * @details méthode qui permet de prendre un objet avec son code barres
 * @param QString codeBarre
 */
void Supervision::prendreObjetAvecCodeBarre(QString codeBarre)
{    
    int numeroCasier = rechercherCasierOuvert();

    if(numeroCasier == -1)
        return;

    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "codeBarre" << codeBarre << "casier" << numeroCasier;
    #endif

    unsigned int quantiteDisponible = this->codeBarre->recupererQuantiteDisponibleParNumeroCasier(QString::number(casiers[numeroCasier]->getNumero()));
    int quantite = quantiteDisponible - this->codeBarre->getQuantiteObjet();
    QString idArticle = QString::number(this->codeBarre->recupererIdArticleAvecCodeBarres(codeBarre));

    if(casiers[numeroCasier]->estOuvert())
    {
        if(verifierArticlePresentDansCasier(QString::number(casiers[numeroCasier]->getNumero()), idArticle))
        {
            if(quantite >= 0)
            {
                QString strQuantite = QString::number(quantite);
                QString requete = "UPDATE Stock SET Disponible = '" +  strQuantite + "' WHERE Stock.idArticle = '" + idArticle + "'";
                bdd->executer(requete);
                QString idUtilisateur = utilisateur->getIdUtilisateur();
                QString idStock = recupererIdStockAvecNumeroCasier(numeroCasier+1 );
                QString idAction = "1";
                QString quantiteMouvement = QString::number(this->codeBarre->getQuantiteObjet());
                mettreAJourMouvement(idUtilisateur, idStock, idAction, quantiteMouvement);
            }
            else
            {
                erreurArticleInsuffisants();
            }
        }
        else
        {
            emit erreurAucunArticleAvecCodeBarre();
        }
    }
    else
    {
        erreurAucunCasierOuvert();
    }
}

/**
 * @brief Définition de la méthode ajouterObjetAvecCodeBarre
 * @details méthode qui permet d'ajouter un objet avec son code barres
 * @param QString codeBarre
 */
void Supervision::ajouterObjetAvecCodeBarre(QString codeBarre)
{
    int numeroCasier = rechercherCasierOuvert();

    if(numeroCasier == -1)
        return;

    #ifdef DEBUG_SUPERVISION
        qDebug() << Q_FUNC_INFO << "codeBarre" << codeBarre << "casier" << numeroCasier;
    #endif

    unsigned int quantiteMax = this->codeBarre->recupererQuantiteMaxParNumeroCasier(QString::number(casiers[numeroCasier]->getNumero()));
    unsigned int quantiteDisponible = this->codeBarre->recupererQuantiteDisponibleParNumeroCasier(QString::number(casiers[numeroCasier]->getNumero()));
    unsigned int quantite = quantiteDisponible + this->codeBarre->getQuantiteObjet();
    QString idArticle = QString::number(this->codeBarre->recupererIdArticleAvecCodeBarres(codeBarre));

    if(casiers[numeroCasier]->estOuvert())
    {
        if(verifierArticlePresentDansCasier(QString::number(casiers[numeroCasier]->getNumero()), idArticle))
        {
            if(quantite <= quantiteMax)
            {
                QString strQuantite = QString::number(quantite);
                QString requete = "UPDATE Stock SET Disponible = '" +  strQuantite + "' WHERE Stock.idArticle = '" + idArticle + "'";
                bdd->executer(requete);
                QString idUtilisateur = utilisateur->getIdUtilisateur();
                QString idStock = recupererIdStockAvecNumeroCasier(numeroCasier+1);
                QString idAction = "2";
                QString quantiteMouvement = QString::number(this->codeBarre->getQuantiteObjet());
                mettreAJourMouvement(idUtilisateur, idStock, idAction, quantiteMouvement);
            }
            else
            {
                emit erreurQuantiteTropElevee();
            }
        }
        else
        {
            emit erreurAucunArticleAvecCodeBarre();
        }
    }
    else
    {
        erreurAucunCasierOuvert();
    }
}

/**
 * @brief Définition de la méthode rechercherCasierOuvert
 * @details méthode pour rechercher le casier ouvert
 * @return int casierOuvert
 */
int Supervision::rechercherCasierOuvert()
{
    for(int i=0; i < casiers.size(); i++)
    {
        if(casiers[i]->estOuvert())
            return i;
    }
    return -1;
}

/**
 * @brief Définition de la méthode recupererIdStockAvecNumeroCasier
 * @details méthode pour récupérer l'idStock avec un numéro de casier
 * @param int numeroCasier
 * @return QString IdStock
 */
QString Supervision::recupererIdStockAvecNumeroCasier(int numeroCasier)
{
    QString strNumeroCasier = QString::number(numeroCasier);
    QString requete = "SELECT idStock FROM Stock WHERE numeroCasier = " + strNumeroCasier + ";";
    QString donnees;
    bdd->recuperer(requete, donnees);

    return donnees;
}
