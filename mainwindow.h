#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QDebug>

#include <QMouseEvent>
#include <QStatusBar>


namespace Ui {
class MainWindow;
}


class CardSide: public QGraphicsRectItem {
public:
    CardSide(bool backSide) {
        setRect(0, 0, 323, 206);
        setBrush(QColor("#FFFFF0"));

        chip = new QGraphicsRectItem(0, 0, 49, 45);
        chip->setPos(34.0157, 68.0315);
        chip->setBrush(Qt::yellow);

        magstripe = new QGraphicsRectItem(0, 0, 323, 49);
        magstripe->setPos(0, 18.8976);
        magstripe->setBrush(Qt::black);

        if (backSide) {
            chip->setPos(rect().width() - (chip->pos().y() + chip->rect().width()), chip->pos().y());
            chip->setOpacity(0.5);

            QBrush brush = chip->brush();
            brush.setStyle(Qt::DiagCrossPattern);
            chip->setBrush(brush);

        } else {
            magstripe->hide();
        }

        QGraphicsItemGroup* itemGroup = new QGraphicsItemGroup();
        itemGroup->addToGroup(chip);
        itemGroup->addToGroup(magstripe);
        itemGroup->setParentItem(this);
    }

    QGraphicsRectItem* chip;
    QGraphicsRectItem* magstripe;
};


class Card: public QGraphicsItemGroup {
public:
    Card() {
        frontSide = new CardSide(false);
        backSide = new CardSide(true);

        // По умолчанию, альбомное расположение
        double y = frontSide->boundingRect().height() + distance; // Вертикальное расположение
        backSide->setX(0);
        backSide->setY(y);

        addToGroup(frontSide);
        addToGroup(backSide);
    }

    CardSide* frontSide;
    CardSide* backSide;

    const double distance = 5.0;
    bool isLandscape = true;
};


class TextElement: public QGraphicsTextItem {
public:
    TextElement(const QString& text, bool isFrontSide, double _x, double _y, int fontSize, int isFixedPos=false): QGraphicsTextItem() {
        setPlainText(text);
        setFont(QFont("Arial", fontSize));

        this->_x = _x;
        this->_y = _y;

        this->isFrontSide = isFrontSide;
        this->isFixedPos = isFixedPos;

        if (isFixedPos) {
            setDefaultTextColor(Qt::darkGreen);
        }

        setPos(_x, _y);

        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    bool isFrontSide = true;
    bool isFixedPos = false;

    // По условиям моделируемой задачи, значения не содержатся в полях элемента,
    // а приходят из вне.
    // _x и _y -- значения, не зависящие от положения и ориентации сторон
    // поэтому они указываются только раз и используются только чтобы
    // рассчитать правильное положение элемента с учетом того на какой
    // стороне карты он размещается и где находится эта сторона
    double _x;
    double _y;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionRotate_triggered(bool checked);
    void on_actionFill_triggered();
    void on_actionTransformItems_triggered(bool checked);
    void on_actionTransfromCard_triggered(bool checked);

    void on_actionAddX_triggered();
    void on_actionAddY_triggered();
    void on_actionMinusX_triggered();
    void on_actionMinusY_triggered();

    void on_actionRotateItem_triggered(bool checked);

    void on_actionAddViewX_triggered();
    void on_actionMinusViewX_triggered();
    void on_actionAddViewY_triggered();
    void on_actionMinusViewY_triggered();

protected:
    bool eventFilter(QObject* obj, QEvent* e);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;

    Card* card;
    QList<TextElement*> items;

    bool needRotate = false;
};


#endif // MAINWINDOW_H
