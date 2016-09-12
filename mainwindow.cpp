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
//    setMouseTracking(true);

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

    item = new TextElement("1234 1234 1234 1234 4", true, 10, 150, 12, true);
    items.append(item);

    item = new TextElement("@@@@@", true, 0, 0, 12, true);
    items.append(item);

    on_actionFill_triggered();

    resize(700, 700);


}

MainWindow::~MainWindow()
{
    delete ui;
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

    needRotate = true;

    on_actionFill_triggered();
}

void MainWindow::on_actionFill_triggered()
{
    for (QGraphicsItem* item: items) {
        if (item->scene() != &scene) {
            continue;
        }

        scene.removeItem(item);
    }

    scene.addEllipse(-5, -5, 10, 10, Qt::NoPen, Qt::darkGreen);

    for (TextElement* item: items) {
        scene.addItem(item);

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
            // Поворот всей карты
            QTransform transform;

            // временный костыль
            if (needRotate)
            if (!card->isLandscape) {
                transform.rotate(-90);
            } else {
                transform.rotate(90);
            }
            item->setTransform(transform, true);

            if (!card->isLandscape) {
                QPointF posSide = card->mapToScene(side->rect().topLeft());

//                QPointF topLeft = card->mapToScene(side->rect().topLeft());
//                QPointF bottomLeft = card->mapToScene(side->rect().bottomLeft());
//                scene.addLine(topLeft.x(), topLeft.y(), bottomLeft.x(), bottomLeft.y(), QPen(Qt::red, 2));

//                scene.addRect(QRectF(posSide, QSizeF(10, 10)), Qt::NoPen, Qt::red);

//                qDebug()<< topLeft << bottomLeft;
//                qDebug()<< posSide << pos << posSide + QPointF(pos.y(), pos.x());

                pos = QPointF(posSide.x() + pos.y(), posSide.y() - pos.x());

//                pos = posSide + QPointF(pos.y(), pos.x());

//                pos = QPointF();

//                scene.addRect(QRectF(pos, item->boundingRect().size()), Qt::NoPen, QColor(255, 0, 0, 100));
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
