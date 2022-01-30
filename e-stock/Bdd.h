#ifndef BDD_H
#define BDD_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>

#define DEBUG_BDD
#define DEBUG_BASEDEDONNEES

#define HOSTNAME       "localhost"
#define USERNAME       "estock"
#define PASSWORD       "password"
#define DATABASENAME   "e-stock"

/**
 * @file Bdd.h
 *
 * @brief Déclaration de la classe Bdd
 *
 * @author Tranchat Joffrey
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date Vendredi 14 Février
 *
 */

/**
 * @class Bdd
 *
 * @brief Déclaration de la classe utilisant la base de données
 *
 * @author Legger Pierre-Antoine
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Vendredi 14 Février
 *
 */
class Bdd
{
public:    
    static Bdd* getInstance();
    static void detruireInstance();

    bool connecter();
    bool estConnecte();

    /* uniquement pour : UPDATE, INSERT et DELETE */
    bool executer(QString requete);

    /* uniquement pour : SELECT */
    bool recuperer(QString requete, QString &donnees); // 1 -> 1
    bool recuperer(QString requete, QStringList &donnees); // 1 -> 1..*
    bool recuperer(QString requete, QVector<QString> &donnees); // 1..* -> 1
    bool recuperer(QString requete, QVector<QStringList> &donnees); // 1..* -> 1..*

private:
    Bdd();
    ~Bdd();
    QSqlDatabase db; //!< pour la connexion à la base de données MySQL
    static Bdd* bdd; //!< pointeur sur l'instance unique
    static int nbAcces; //!< compte le nombre d'accès à l'instance unique
};

#endif // BDD_H
