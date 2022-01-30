#include "Bdd.h"
#include <QDebug>
#include <QMessageBox>

/**
 * @file Bdd.cpp
 *
 * @brief Définition de la classe Bdd
 *
 * @author Tranchat Joffrey
 * @author Legger Pierre-Antoine
 * @author Vaira Thierry
 *
 * @version 1.0
 *
 * @date 14 Février 2020
 */

Bdd* Bdd::bdd = NULL;
int Bdd::nbAcces = 0;

/**
 * @brief Définition du constructeur de la classe Bdd
 * @fn Bdd::Bdd
 * @details initialise le type MySQL pour la connexion à la base de données
 */
Bdd::Bdd()
{
    #ifdef DEBUG_BDD
    qDebug() << Q_FUNC_INFO;
    #endif
    db = QSqlDatabase::addDatabase("QMYSQL");
}

/**
 * @brief Définition du destructeur de la classe Bdd
 * @fn Bdd::~Bdd
 * @details destructeur de la classe Bdd
 */
Bdd::~Bdd()
{
    #ifdef DEBUG_BDD
    qDebug() << Q_FUNC_INFO;
    #endif
}

/**
 * @brief Définition méthode getInstance()
 * @fn Bdd::getInstance
 * @details permet l'instanciation d'un objet Bdd en vérifiant qu'il n'en existe pas déja un
 * @return bdd l'instance unique
 */
Bdd* Bdd::getInstance()
{
    if(bdd == NULL)
        bdd = new Bdd();

    nbAcces++;

    #ifdef DEBUG_BDD
    qDebug() << Q_FUNC_INFO << "nbAcces" << nbAcces;
    #endif

    return bdd;

}

/**
 * @brief Définition méthode detruireInstance()
 * @fn Bdd::detruireInstance
 * @details détruit l'instance de la Bdd si elle existe et si personne l'utilise
 */
void Bdd::detruireInstance()
{
    if(bdd != NULL)
    {
        nbAcces--;
        #ifdef DEBUG_BASEDEDONNEES
        qDebug() << Q_FUNC_INFO << "nbAcces" << nbAcces;
        #endif

        if(nbAcces == 0)
            delete bdd;
    }
}

/**
 * @brief Définition méthose connecter()
 * @fn Bdd::connecter
 * @details permet de se connecter à la base de données
 * @return boolean
 */
bool Bdd::connecter()
{
    if(!db.isOpen())
    {
        db.setHostName(HOSTNAME);
        db.setUserName(USERNAME);
        db.setPassword(PASSWORD);
        db.setDatabaseName(DATABASENAME);

        #ifdef DEBUG_BDD
        qDebug() << Q_FUNC_INFO << "HostName" << db.hostName();
        qDebug() << Q_FUNC_INFO << "UserName" << db.userName();
        qDebug() << Q_FUNC_INFO << "DatabaseName" << db.databaseName();
        #endif
        if(db.open())
        {
            #ifdef DEBUG_BDD
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("connexion réussie à %1").arg(db.hostName());
            #endif

            return true;
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("erreur : impossible de se connecter à la base de données !");

            QMessageBox::critical(0, QString::fromUtf8("e-stock"), QString::fromUtf8("Impossible de se connecter à la base de données !"));
            return false;
        }

    }
    else
        return true;
}

/**
 * @brief retourne l'état de connexion à la base de données
 * @fn Bdd::estConnecte
 * @details retourne l'état de connexion à la base de données
 * @return boolean true si connecté à la base de données sinon false
 */
bool Bdd::estConnecte()
{
    return db.isOpen();
}

/**
 * @brief exécute une requête SQL de type UPDATE, INSERT et DELETE
 * @fn bool Bdd::executer(QString requete)
 * @details exécute une requête SQL de type UPDATE, INSERT et DELETE
 * @param[in] requete une requête SQL de type UPDATE, INSERT et DELETE
 * @return boolean true si la requête a été exécutée avec succès sinon false
 */
bool Bdd::executer(QString requete)
{
    QSqlQuery r;
    bool retour;

    if(db.isOpen())
    {
        if(requete.contains("UPDATE") || requete.contains("INSERT") || requete.contains("DELETE"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;

}

/**
 * @brief exécute une requête SQL de type SELECT et récupère un champ d'un seul enregistrement
 * @fn bool Bdd::recuperer(QString requete, QString &donnees)
 * @param[in] requete une requête SQL de type SELECT
 * @param[out] donnees le champ QString récupéré
 * @return boolean true si la requête a été exécutée avec succès sinon false
 */
bool Bdd::recuperer(QString requete, QString &donnees)
{
    QSqlQuery r;
    bool retour;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // on se positionne sur l'enregistrement
                r.first();

                // on vérifie l'état de l'enregistrement retourné
                if(!r.isValid())
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Résultat non valide !");
                    #endif
                    return false;
                }

                // on récupère sous forme de QString la valeur du champ
                if(r.isNull(0))
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Aucun résultat !");
                    #endif
                    return false;
                }
                donnees = r.value(0).toString();
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}

/**
 * @brief exécute une requête SQL de type SELECT et récupère plusieurs champs d'un seul enregistrement
 * @fn bool Bdd::recuperer(QString requete, QStringList &donnees)
 * @param[in] requete une requête SQL de type SELECT
 * @param[out] donnees plusieurs champs d'un seul enregistrement dans un QStringList
 * @return boolean true si la requête a été exécutée avec succès sinon false
 */
bool Bdd::recuperer(QString requete, QStringList &donnees)
{
    QSqlQuery r;
    bool retour;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // on se positionne sur l'enregistrement
                r.first();

                // on vérifie l'état de l'enregistrement retourné
                if(!r.isValid())
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Résultat non valide !");
                    #endif
                    return false;
                }

                // on récupère sous forme de QString la valeur de tous les champs sélectionnés
                // et on les stocke dans une liste de QString
                for(int i=0;i<r.record().count();i++)
                    if(!r.isNull(i))
                        donnees << r.value(i).toString();
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}

/**
 * @brief exécute une requête SQL de type SELECT et récupère un seul champ de plusieurs enregistrements
 * @fn bool Bdd::recuperer(QString requete, QVector<QString> &donnees)
 * @param[in] requete une requête SQL de type SELECT
 * @param[out] donnees un seul champ de plusieurs enregistrements dans un QVector de QString
 * @return boolean true si la requête a été exécutée avec succès sinon false
 */
bool Bdd::recuperer(QString requete, QVector<QString> &donnees)
{
    QSqlQuery r;
    bool retour;
    QString data;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // pour chaque enregistrement
                while ( r.next() )
                {
                    // on récupère sous forme de QString la valeur du champs sélectionné
                    data = r.value(0).toString();

                    #ifdef DEBUG_BASEDEDONNEES
                    //qDebug() << Q_FUNC_INFO << "Enregistrement -> " << data;
                    #endif

                    // on stocke l'enregistrement dans le QVector
                    donnees.push_back(data);
                }
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}

/**
 * @brief exécute une requête SQL de type SELECT et récupère plusieurs champs de plusieurs enregistrements
 * @param[in] requete une requête SQL de type SELECT
 * @param[out] donnees : plusieurs champs de plusieurs enregistrements dans un QVector de QStringList
 * @return boolean true si la requête a été exécutée avec succès sinon false
 */
bool Bdd::recuperer(QString requete, QVector<QStringList> &donnees)
{
    QSqlQuery r;
    bool retour;
    QStringList data;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // pour chaque enregistrement
                while ( r.next() )
                {
                    // on récupère sous forme de QString la valeur de tous les champs sélectionnés
                    // et on les stocke dans une liste de QString
                    for(int i=0;i<r.record().count();i++)
                        data << r.value(i).toString();

                    #ifdef DEBUG_BASEDEDONNEES
                    //qDebug() << Q_FUNC_INFO << "Enregistrement -> " << data;
                    /*for(int i=0;i<r.record().count();i++)
                        qDebug() << r.value(i).toString();*/
                    #endif

                    // on stocke l'enregistrement dans le QVector
                    donnees.push_back(data);

                    // on efface la liste de QString pour le prochain enregistrement
                    data.clear();
                }
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}
