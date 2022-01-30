#ifndef CODEBARRE_H
#define CODEBARRE_H

/**
 * @file CodeBarre.h
 *
 * @brief Déclaration de la classe CodeBare
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 12 Février 2020
 *
 */

#include <QObject>

#define DEBUG_CODE_BARRE

class Bdd;

/**
 * @class CodeBarre
 *
 * @brief Déclaration de la classe CodeBarre
 *
 * @author Tranchat Joffrey
 *
 * @version 1.0
 *
 * @date Mercredi 12 Février 2020
 *
 */
class CodeBarre : public QObject
{
    Q_OBJECT

public:
    CodeBarre(QObject *parent = nullptr);
    ~CodeBarre();

    unsigned int recupererQuantiteMaxParNumeroCasier(QString numeroCasier);
    unsigned int recupererQuantiteDisponibleParNumeroCasier(QString numeroCasier);
    unsigned int recupererIdArticleAvecCodeBarres(QString codeBarre);
    unsigned int getQuantiteObjet();

private:
    Bdd *bdd;            //!< association d'un objet Bdd (accès à la base de données)
    bool etatPrendreOuAjouter;  //!< boolean pour savoir si l'on prend ou ajoute un objet(false = prendre, true = ajouter)
    unsigned int quantiteObjet; //!< quantité d'objet à prendre ou ajouter

    void setEtatPrendreOuAjouter(bool etat);
    void setQuantiteObjet(unsigned int quantite);
    QString corrigerCodeBarre(QString codeBarre);

public slots:
    void prendreOuAjouter(bool etat);
    void traiterCodeBarre(QString codeBarre);
    void changerQuantiteObjet(int quantite);

signals:
    void ajouterObjet(QString);
    void prendreObjet(QString);
};

#endif // CODEBARRE_H
