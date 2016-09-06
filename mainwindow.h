#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QDebug>


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

//    void addItem(QGraphicsItem* item) {
//        QPointF pos = item->data(Qt::UserRole + 1).toPointF();

//        CardSide* side = item->data(Qt::UserRole).toBool() ? backSide : frontSide;
//        pos = side->mapToScene(pos);

////        // Теперь напишем свой аналог для расстановки элементов
////        QPointF pos;

////        // Альбомная ориентация
////        if (isLandscape) {
////            pos = item->pos() + side->pos();

////        // Портретная ориентация
////        } else {
////            pos = mapToScene(side->pos() + side->boundingRect().topRight());
////            pos = item->pos() + pos;
////        }

//        item->setPos(pos);
//    }

    CardSide* frontSide;
    CardSide* backSide;

    const double distance = 5.0;
    bool isLandscape = true;
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

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;

    Card* card;
    QList<QGraphicsItem*> items;
};


#endif // MAINWINDOW_H
