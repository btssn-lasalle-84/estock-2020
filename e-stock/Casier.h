#ifndef CASIER_H
#define CASIER_H

/**
 * @file Casier.h
 *
 * @brief Déclaration de la classe Casier
 *
 * @author
 *
 * @version 1.0
 *
 * @date
 *
 */

#include <QtWidgets>

#define SIMULATION_CASIER

/**
 * @class Casier
 *
 * @brief La classe Casier gère le casier contenant des articles
 *
 * @author Tranchat Joffrey
 * @author Legger Pierre-Antoine
 *
 * @version 1.0
 *
 * @date samedi 28 Mars 2020
 *
 */
class Casier : public QPushButton
{
    Q_OBJECT
public:
    Casier(int numero, QWidget *parent=0);
    ~Casier();

    int getNumero() const;
    bool estOuvert() const;
    void setOuvert(bool ouvert);
    void ouvrir();    

private:
    int numero;     //!< numero du casier
    bool ouvert;    //!< état ouvert ou fermé du casier

public slots:
    void gererEtat();

signals:
    void estOuvert(int numero, bool etat);

};

#endif // CASIER_H
