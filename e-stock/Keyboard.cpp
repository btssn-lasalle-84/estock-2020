/**
* @brief Fichier qui contient la définition de la classe Keyboard
* @file Keyboard.cpp
* @author Thierry Vaira
*
* based on The virtual keyboard demo software is Copyright (C) 2015 Klaralvdalens Datakonsult AB.
*/

#include "Keyboard.h"

#include <QGridLayout>
#include <QSignalMapper>
#include <QPushButton>
#include <QDebug>

#define NEXT_ROW_MARKER 0

struct KeyboardLayoutEntry
{
    int key;
    const char *label;
    int keyCapsLock;
    const char *labelCapsLock;
};

KeyboardLayoutEntry keyboardLayout[] =
{
    { Qt::Key_1, "1", Qt::Key_Ampersand, "&" },
    { Qt::Key_2, "2", Qt::Key_Eacute, "é" },
    { Qt::Key_3, "3", Qt::Key_QuoteDbl, "\"" },
    { Qt::Key_4, "4", Qt::Key_Apostrophe, "'" },
    { Qt::Key_5, "5", Qt::Key_ParenRight, "(" },
    { Qt::Key_6, "6", Qt::Key_hyphen, "-" },
    { Qt::Key_7, "7", Qt::Key_Egrave, "è" },
    { Qt::Key_8, "8", Qt::Key_Underscore, "_" },
    { Qt::Key_9, "9", Qt::Key_cedilla, "ç" },
    { Qt::Key_0, "0", Qt::Key_Agrave, "à" },
    { Qt::Key_Percent, "%", Qt::Key_ParenLeft, ")" },
    { Qt::Key_Plus, "+", Qt::Key_Equal, "=" },
    { Qt::Key_Backspace, "<-",  Qt::Key_Backspace, "<-"},
    { NEXT_ROW_MARKER, 0, 0, 0 },
    { Qt::Key_A, "a", Qt::Key_A, "A" },
    { Qt::Key_Z, "z", Qt::Key_Z, "Z" },
    { Qt::Key_E, "e", Qt::Key_E, "E" },
    { Qt::Key_R, "r", Qt::Key_R, "R" },
    { Qt::Key_T, "t", Qt::Key_T, "T" },
    { Qt::Key_Y, "y", Qt::Key_Y, "Y" },
    { Qt::Key_U, "u", Qt::Key_U, "U" },
    { Qt::Key_I, "i", Qt::Key_I, "I" },
    { Qt::Key_O, "o", Qt::Key_O, "O" },
    { Qt::Key_P, "p", Qt::Key_P, "P" },
    { Qt::Key_At, "@", Qt::Key_Dollar, "$" },
    { Qt::Key_Escape, "Esc", Qt::Key_Escape, "Esc" },
    { Qt::Key_Delete, "X",  Qt::Key_Delete, "X"},
    { NEXT_ROW_MARKER, 0, 0, 0 },
    { Qt::Key_Q, "q", Qt::Key_Q, "Q" },
    { Qt::Key_S, "s", Qt::Key_S, "S" },
    { Qt::Key_D, "d", Qt::Key_D, "D" },
    { Qt::Key_F, "f", Qt::Key_F, "F" },
    { Qt::Key_G, "g", Qt::Key_G, "G" },
    { Qt::Key_H, "h", Qt::Key_H, "H" },
    { Qt::Key_J, "j", Qt::Key_J, "J" },
    { Qt::Key_K, "k", Qt::Key_K, "K" },
    { Qt::Key_L, "l", Qt::Key_L, "L" },
    { Qt::Key_M, "m", Qt::Key_M, "M" },
    { Qt::Key_Ooblique, "/", Qt::Key_Backslash, "\\" },
    { Qt::Key_multiply, "*", Qt::Key_ssharp, "#" },
    { Qt::Key_Space, " ", Qt::Key_Space, " " },
    { NEXT_ROW_MARKER, 0, 0, 0 },
    { Qt::Key_CapsLock, "Maj", Qt::Key_CapsLock, "Maj" },
    { Qt::Key_Less, "<", Qt::Key_Greater, ">" },
    { Qt::Key_W, "w", Qt::Key_W, "W"},
    { Qt::Key_X, "x", Qt::Key_X, "X"},
    { Qt::Key_C, "c", Qt::Key_C, "C"},
    { Qt::Key_V, "v", Qt::Key_V, "V"},
    { Qt::Key_B, "b", Qt::Key_B, "B"},
    { Qt::Key_N, "n", Qt::Key_N, "N"},
    { Qt::Key_Comma, ",", Qt::Key_Question, "?"},
    { Qt::Key_Semicolon, ";", Qt::Key_Period, "."},
    { Qt::Key_Colon, ":", Qt::Key_AsciiTilde, "~"},
    { Qt::Key_exclamdown, "!", Qt::Key_section, "§"},
    { Qt::Key_Enter, "Entrée", Qt::Key_Enter, "Entrée"}
};

const static int layoutSize = (sizeof(keyboardLayout) / sizeof(KeyboardLayoutEntry));

#ifdef KEYBOARD_SINGLETON
Keyboard* Keyboard::instance = Q_NULLPTR;

Keyboard* Keyboard::getInstance(QLineEdit *lineEdit, QWidget *parent)
{
    if(instance == Q_NULLPTR)
    {
        instance = new Keyboard(lineEdit, parent);
    }
    return instance;
}
#endif

Keyboard::Keyboard(QLineEdit *lineEdit, QWidget *parent) : QWidget(parent), lineEdit(lineEdit), capsLock(false), mask(false)
{
#ifdef DEBUG_KEYBOARD
    qDebug() << Q_FUNC_INFO << this << lineEdit;
#endif
    init();
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(focusChange(QWidget*,QWidget*)));
    connect(this, SIGNAL(keyClicked(QString)), this, SLOT(keyboardKeyClicked(QString)));
    connect(this, SIGNAL(specialKeyClicked(int)), this, SLOT(keyboardSpecialKeyClicked(int)));
    setStyleSheet("/*QWidget{background-color:white;}*/QPushButton{font-family:\"Ubuntu Mono\";font:bold;font-size:16px;background-color:palegoldenrod;border-width:1px;border-color:darkkhaki;border-style:solid;border-radius:5;padding:1px;}QPushButton:hover{background-color:khaki;}QPushButton:pressed{background-color:#d0d67c;}");
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void Keyboard::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen;
    pen.setBrush(QBrush(QColor(128, 128, 128, 64)));
    painter.setPen(pen);
    painter.drawRoundedRect(0,0,width()-1, height()-1,5,5);
    QWidget::paintEvent(e);
}

void Keyboard::setLineEdit(QLineEdit *lineEdit)
{
    if(lineEdit && this->lineEdit != lineEdit)
    {
#ifdef DEBUG_KEYBOARD
    qDebug() << Q_FUNC_INFO << this << lineEdit;
#endif
        this->lineEdit = lineEdit;
        capsLock = false;
        mask = false;
    }
}

void Keyboard::showKeyboard(int globalX, int globalY)
{
    QWidget::move(globalX, globalY);
    if(mask)
    {
        setFixedSize(hBoxLayout->sizeHint());
        stackedWidget->setCurrentIndex(1);
    }
    else
    {
        setFixedSize(gridLayout->sizeHint());
        stackedWidget->setCurrentIndex(0);
    }
    QWidget::show();
}

void Keyboard::hideKeyboard()
{
    QWidget::hide();
}

bool Keyboard::keyboardVisible() const
{
    return QWidget::isVisible();
}

void Keyboard::buttonClicked(int key)
{
#ifdef DEBUG_KEYBOARD
    qDebug() << Q_FUNC_INFO << key;
#endif
    if ((key == Qt::Key_Escape))
    {
        mask = !mask;
        if(mask)
        {
            setFixedSize(hBoxLayout->sizeHint());
            stackedWidget->setCurrentIndex(1);
        }
        else
        {
            setFixedSize(gridLayout->sizeHint());
            stackedWidget->setCurrentIndex(0);
        }
        return;
    }
    if ((key == Qt::Key_CapsLock))
    {
        capsLock = !capsLock;
        makeCapsLock();
        return;
    }
    if ((key == Qt::Key_Delete))
    {
        if(lineEdit)
            lineEdit->clear();
        return;
    }
    if ((key == Qt::Key_Enter) || (key == Qt::Key_Backspace))
    {
        emit specialKeyClicked(key);
    }
    else
    {
        emit keyClicked(keyToCharacter(key));
    }
}

void Keyboard::focusChange(QWidget* oldWidget, QWidget* newWidget)
{
    if(!lineEdit || !newWidget)
        return;
#ifdef DEBUG_KEYBOARD
    qDebug() << Q_FUNC_INFO << oldWidget << newWidget;
#endif
    if(newWidget == lineEdit)
    {
        QPoint globalPos(0, 0);
        globalPos = newWidget->mapToGlobal(QPoint(0, newWidget->height()));
        showKeyboard(globalPos.x(), globalPos.y());
    }
    else
    {
        hideKeyboard();
    }
}

void Keyboard::keyboardSpecialKeyClicked(int key)
{
#ifdef DEBUG_KEYBOARD
    qDebug() << Q_FUNC_INFO << this << key;
#endif
    if (!lineEdit)
        return;

    if (key == Qt::Key_Enter)
    {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(lineEdit, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(lineEdit, releaseEvent);
    }
    else if (key == Qt::Key_Backspace)
    {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QGuiApplication::postEvent(lineEdit, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QGuiApplication::postEvent(lineEdit, releaseEvent);
    }
}

void Keyboard::keyboardKeyClicked(const QString &characters)
{
#ifdef DEBUG_KEYBOARD
    qDebug() << Q_FUNC_INFO << lineEdit << characters;
#endif
    if (!lineEdit)
        return;

    QInputMethodEvent event;
    event.setCommitString(characters);

    QGuiApplication::sendEvent(lineEdit, &event);
}

void Keyboard::init()
{
    setWindowFlags(Qt::WindowDoesNotAcceptFocus | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint);

    stackedWidget = new QStackedWidget(this);
    QWidget *widgetKeyboard = new QWidget(stackedWidget);
    QWidget *widgetMask = new QWidget(stackedWidget);

    gridLayout = new QGridLayout(widgetKeyboard);
    hBoxLayout = new QHBoxLayout(widgetMask);

    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

    int row = 0;
    int column = 0;

    for (int i = 0; i < layoutSize; ++i)
    {
        if (keyboardLayout[i].key == NEXT_ROW_MARKER)
        {
            row++;
            column = 0;
            continue;
        }

        QPushButton *button = new QPushButton(this);
        if (keyboardLayout[i].key == Qt::Key_Enter)
            button->setFixedWidth(55);
        else
            button->setFixedWidth(35);
        button->setFixedHeight(25);
        button->setText(QString::fromUtf8(keyboardLayout[i].label));

        mapper->setMapping(button, keyboardLayout[i].key);
        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));

        if (keyboardLayout[i].key == Qt::Key_Escape)
        {
            buttonEsc = new QPushButton(this);
            buttonEsc->setFixedWidth(35);
            buttonEsc->setText(QString::fromUtf8(keyboardLayout[i].label));
            mapper->setMapping(buttonEsc, keyboardLayout[i].key);
            connect(buttonEsc, SIGNAL(clicked()), mapper, SLOT(map()));
            hBoxLayout->addWidget(buttonEsc);
        }
        gridLayout->addWidget(button, row, column);
        column++;
    }
    widgetMask->setFixedSize(hBoxLayout->sizeHint());
    stackedWidget->addWidget(widgetKeyboard);
    stackedWidget->addWidget(widgetMask);
}

void Keyboard::makeCapsLock()
{
    int row = 0;
    int column = 0;
    for (int i = 0; i < layoutSize; ++i)
    {
        if (keyboardLayout[i].key == NEXT_ROW_MARKER)
        {
            row++;
            column = 0;
            continue;
        }

        QPushButton *button = dynamic_cast<QPushButton*>(gridLayout->itemAtPosition(row, column)->widget());
        if(capsLock)
        {
            mapper->setMapping(button, keyboardLayout[i].keyCapsLock);
            button->setText(QString::fromUtf8(keyboardLayout[i].labelCapsLock));
        }
        else
        {
            mapper->setMapping(button, keyboardLayout[i].key);
            button->setText(QString::fromUtf8(keyboardLayout[i].label));
        }
        column++;
    }
}

QString Keyboard::keyToCharacter(int key)
{
    for (int i = 0; i < layoutSize; ++i)
    {
        if (keyboardLayout[i].key == key)
        {
            if(capsLock)
                return QString::fromUtf8(keyboardLayout[i].labelCapsLock);
            else
                return QString::fromUtf8(keyboardLayout[i].label);
        }
        else if (keyboardLayout[i].keyCapsLock == key)
            return QString::fromUtf8(keyboardLayout[i].labelCapsLock);
    }

    return QString();
}
