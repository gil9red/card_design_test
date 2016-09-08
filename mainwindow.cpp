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

    TextElement* item = new TextElement("1234 1234 1234 1234 4", true, 10, 10, 12, false);
    scene.addItem(item);
    items.append(item);

    item = new TextElement("999", true, 10, 35, 16, false);
    scene.addItem(item);
    items.append(item);

    item = new TextElement("John Smith", false, 5, 5, 16, false);
    scene.addItem(item);
    items.append(item);

    on_actionFill_triggered();

//    ui->graphicsView->scale(.6, .6);

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

    for (TextElement* item: items) {
        scene.addItem(item);

        QPointF pos(item->_x, item->_y);
        CardSide* side = item->isFrontSide ? card->frontSide : card->backSide;

        // Альбомная ориентация
        if (card->isLandscape) {
            pos = side->pos() + pos;

        // Портретная ориентация
        } else {
            QPointF posSide = card->mapToScene(side->pos() + side->boundingRect().topRight());
            pos = posSide + pos;
        }

        item->setPos(pos);
    }
}

void MainWindow::on_actionTransformItems_triggered(bool checked)
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

    for (QGraphicsItem* item: items) {
        item->setTransform(transform, true);
    }
}

void MainWindow::on_actionTransfromCard_triggered(bool checked)
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
}
