#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * @brief Fichier qui contient la déclaration de la classe Keyboard
 * @file Keyboard.h
 * @author Thierry Vaira
 *
 * based on The virtual keyboard demo software is Copyright (C) 2015 Klaralvdalens Datakonsult AB.
 */

#include <QtWidgets>

#define KEYBOARD_SINGLETON
//#define DEBUG_KEYBOARD

/**
 * @class Keyboard
 * @brief Déclaration de la classe Keyboard
 * @details Cette classe permet de gérer un clavier virtuel
*/
class Keyboard : public QWidget
{
    Q_OBJECT
private:
#ifdef KEYBOARD_SINGLETON
    explicit Keyboard(QLineEdit *lineEdit = Q_NULLPTR, QWidget *parent = Q_NULLPTR);
    static Keyboard* instance;
#endif
    QStackedWidget *stackedWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *hBoxLayout;
    QSignalMapper *mapper;
    QLineEdit *lineEdit;
    QPushButton *buttonEsc;
    bool capsLock;
    bool mask;
    void init();
    void makeCapsLock();
    QString keyToCharacter(int key);

protected:
    void paintEvent(QPaintEvent *e);

public:
#ifndef KEYBOARD_SINGLETON
    explicit Keyboard(QLineEdit *lineEdit = Q_NULLPTR, QWidget *parent = Q_NULLPTR);
#else
    static Keyboard* getInstance(QWidget *parent = Q_NULLPTR);
    static Keyboard* getInstance(QLineEdit *lineEdit, QWidget *parent = Q_NULLPTR);
#endif
    void setLineEdit(QLineEdit *lineEdit);

public slots:
    void showKeyboard(int globalX, int globalY);
    void hideKeyboard();
    bool keyboardVisible() const;

signals:
    void specialKeyClicked(int key);
    void keyClicked(const QString &text);

private slots:
    void buttonClicked(int key);
    void focusChange(QWidget*, QWidget*);
    void keyboardSpecialKeyClicked(int key);
    void keyboardKeyClicked(const QString &characters);
};

#endif
