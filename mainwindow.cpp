#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene);

    // Убираем ползунки прокрутки
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    card = new Card();
    scene.addItem(card);

    QGraphicsItem* item = scene.addText("1234 1234 1234 1234 4", QFont("Arial", 12));
    item->setData(Qt::UserRole, false);
    item->setData(Qt::UserRole + 1, QPointF(10, 10));
    item->setData(Qt::UserRole + 2, true);
    items.append(item);

    item = scene.addText("999", QFont("Arial", 16));
    item->setData(Qt::UserRole, false);
    item->setData(Qt::UserRole + 1, QPointF(10, 35));
    items.append(item);

    item = scene.addText("John Smith", QFont("Arial", 16));
    item->setData(Qt::UserRole, true);
    item->setData(Qt::UserRole + 1, QPointF(5, 5));
    items.append(item);

    on_actionFill_triggered();

    resize(700, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRotate_triggered(bool checked)
{
    // Поворот всей карты
    QPointF center = card->boundingRect().center();
    QTransform transform;

    if (checked) {
        transform.translate(center.x(), center.y())
                .rotate(-90)
                .translate(-center.x(), -center.y());

    } else {
        transform.translate(center.x(), center.y())
                .rotate(90)
                .translate(-center.x(), -center.y());
    }
    card->setTransform(transform, true);

    card->isLandscape = !checked;

    on_actionFill_triggered();
}

void MainWindow::on_actionFill_triggered()
{
    for (QGraphicsItem* item: items) {
        scene.removeItem(item);
    }

    for (QGraphicsItem* item: items) {
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);

//        card->addToGroup(item);
//        if (item->data(Qt::UserRole + 2).toBool()) {
//            card->addToGroup(item);
//        } else {
//            scene.addItem(item);
//        }

//        card->addItem(item);

        scene.addItem(item);

        QPointF pos = item->data(Qt::UserRole + 1).toPointF();
        item->setPos(pos);

        CardSide* side = item->data(Qt::UserRole).toBool() ? card->backSide : card->frontSide;

        // Старый способ раположения элементов, работал только с альбомной ориентацией
        // pos = side->mapToScene(pos);

        // Теперь напишем свой аналог для расстановки элементов

        // Альбомная ориентация
        if (card->isLandscape) {
            pos = item->pos() + side->pos();

        // Портретная ориентация
        } else {
            pos = card->mapToScene(side->pos() + side->boundingRect().topRight());
            pos = item->pos() + pos;
        }

        item->setPos(pos);
    }
}
