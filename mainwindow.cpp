#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene);
    ui->graphicsView->viewport()->installEventFilter(this);

    // Убираем ползунки прокрутки
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    card = new Card();
    scene.addItem(card);

    TextElement* item = new TextElement("1234 1234 1234 1234 4", true, 10, 10, 12, false);
    items.append(item);

    item = new TextElement("999", true, 10, 35, 16, false);
    items.append(item);

    item = new TextElement("John Smith", false, 5, 5, 16, false);
    items.append(item);

    item = new TextElement("John Smith", false, 5, 150, 16, true);
    items.append(item);

    item = new TextElement("1234 1234 1234 1234 4", true, 10, 150, 12, true);
    items.append(item);

    item = new TextElement("@@@@@", true, 0, 0, 12, true);
    items.append(item);

    for (QGraphicsItem* item: items) {
        scene.addItem(item);
    }


    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    updateSceneRect();


    on_actionFill_triggered();

    resize(700, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSceneRect() {
    QPolygonF polygonF = card->mapToScene(card->boundingRect());
    qreal x = -10 + polygonF.first().x();
    qreal y = -10 - polygonF.first().x();
    qreal width = card->boundingRect().width();
    qreal height = card->boundingRect().height();
    ui->graphicsView->setSceneRect(QRectF(x, y, width, height));
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e) {
    if (obj == ui->graphicsView->viewport()) {
        if (e->type() == QEvent::MouseMove) {
            QMouseEvent* me = (QMouseEvent*) e;

            QPoint pos = me->pos();
            QPointF posf = ui->graphicsView->mapToScene(pos);

            statusBar()->showMessage(
                QString("%1x%2").arg(posf.x()).arg(posf.y())
            );
        }
    }

    return QMainWindow::eventFilter(obj, e);
}

void MainWindow::on_actionRotate_triggered(bool checked)
{
    // Поворот всей карты
    QTransform transform;

    checked ?
        transform.rotate(-90).translate(-card->boundingRect().width(), 0) :
        transform.rotate(90).translate(0, -card->boundingRect().width());

    card->setTransform(transform, true);

    card->isLandscape = !checked;

    needRotate = true;

    on_actionFill_triggered();


    updateSceneRect();
}

void MainWindow::on_actionFill_triggered()
{
    for (TextElement* item: items) {
        QPointF pos(item->_x, item->_y);
        CardSide* side = item->isFrontSide ? card->frontSide : card->backSide;

        if (!item->isFixedPos) {
            // Альбомная ориентация
            if (card->isLandscape) {
                pos = side->pos() + pos;

            // Портретная ориентация
            } else {
                QPointF posSide = card->mapToScene(side->pos() + side->boundingRect().topRight());
                pos = posSide + pos;
            }

        } else {
            // Поворот зафиксированного элемента
            QTransform transform;

            // временный костыль
            if (needRotate)
            if (!card->isLandscape) {
                transform.rotate(-90);
            } else {
                transform.rotate(90);
            }
            item->setTransform(transform, true);

            if (card->isLandscape) {
                pos = side->pos() + pos;

            } else {
                QPointF posSide = card->mapToScene(side->pos());
                pos = QPointF(posSide.x() + pos.y(), posSide.y() - pos.x());
            }
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

void MainWindow::on_actionAddX_triggered()
{
    for (TextElement* item: items) {
        if (item->isFixedPos) {
            item->setPos(item->pos().x() + 1, item->pos().y());
        }
    }
}
void MainWindow::on_actionAddY_triggered()
{
    for (TextElement* item: items) {
        if (item->isFixedPos) {
            item->setPos(item->pos().x(), item->pos().y() + 1);
        }
    }
}
void MainWindow::on_actionMinusX_triggered()
{
    for (TextElement* item: items) {
        if (item->isFixedPos) {
            item->setPos(item->pos().x() - 1, item->pos().y());
        }
    }
}
void MainWindow::on_actionMinusY_triggered()
{
    for (TextElement* item: items) {
        if (item->isFixedPos) {
            item->setPos(item->pos().x(), item->pos().y() - 1);
        }
    }
}


void MainWindow::on_actionRotateItem_triggered(bool checked)
{
    for (TextElement* item: items) {
        if (item->isFixedPos) {
            // Поворот всей карты
            QPointF center = item->boundingRect().center();
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

            item->setTransform(transform, true);
        }
    }
}

void MainWindow::on_actionAddViewX_triggered()
{
    QRectF sceneRect = ui->graphicsView->sceneRect();
    sceneRect.moveRight(sceneRect.right() + 5);

    ui->graphicsView->setSceneRect(sceneRect);
}

void MainWindow::on_actionMinusViewX_triggered()
{
    QRectF sceneRect = ui->graphicsView->sceneRect();
    sceneRect.moveLeft(sceneRect.left() + 5);

    ui->graphicsView->setSceneRect(sceneRect);
}

void MainWindow::on_actionAddViewY_triggered()
{
    QRectF sceneRect = ui->graphicsView->sceneRect();
    sceneRect.moveBottom(sceneRect.bottom() + 5);

    ui->graphicsView->setSceneRect(sceneRect);
}

void MainWindow::on_actionMinusViewY_triggered()
{
    QRectF sceneRect = ui->graphicsView->sceneRect();
    sceneRect.moveTop(sceneRect.top() + 5);

    ui->graphicsView->setSceneRect(sceneRect);
}
