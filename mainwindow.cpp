/*
 * Copyright (C) 2013 Simon A. Berger
 *
 * This file is part of photo_box.
 *
 *  photo_box is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  photo_box is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with photo_box.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QThread>
#include <QDateTime>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    is_fullscreen_(false)
{
    ui->setupUi(this);


    ui->graphicsView->setScene(&scene_);
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   // ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);


    scene_.setBackgroundBrush(Qt::black);
    //scene_.setSceneRect( 0, 0, 100, 100);
//    QDir default_dir( "c:\\test");
//    if( !default_dir.isReadable() ) {
//        default_dir = QDir();
//    }
    path_ = QFileDialog::getExistingDirectory(0, "Picture Directory" );

    QDir dir( path_, "*.jpg" );

    QStringList sl = dir.entryList();
    file_set_ = sl.toSet();


//     "c:\\test";
    watcher.addPath( path_ );
    QObject::connect( &watcher, SIGNAL(directoryChanged(QString)), this, SLOT(on_watcher_directoryChanged(QString)));
    QObject::connect( &timer, SIGNAL(timeout()), this, SLOT(sl_timer_timeout()));

    timer.setSingleShot(true);
    timer.start( 20 );
    setWindowState(Qt::WindowMaximized);
    std::cout << "bla" << std::endl;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbFullscreen_clicked() {

    if( is_fullscreen_ ) {
        setWindowState(Qt::WindowMaximized);
    } else {
        setWindowState(Qt::WindowFullScreen);
    }

    is_fullscreen_ = !is_fullscreen_;
}

void MainWindow::on_watcher_directoryChanged( const QString &path ) {

    QDir dir( path, "*.jpg" );

    QStringList sl = dir.entryList();
    QSet<QString> new_set = sl.toSet();


    QSet<QString> sub( new_set );
    sub.subtract(file_set_);


    if( sub.size() >= 1 ) {
        //for( QSet<QString>::Iterator it = sub.begin(); it != sub.end(); ++it ) {
        QSet<QString>::Iterator it = sub.end() - 1;
        {

            QFileInfo fi( path, *it );
            std::cout << "new file: " << fi.absoluteFilePath().toStdString() << std::endl;
            timer.stop();

            QDateTime lmStart = fi.lastModified();




            std::cout << "printing!" << std::endl;
            if( false ) {
                QPrinter printer;
                printer.setOrientation( QPrinter::Landscape );
                QPainter painter(&printer);
                QRect rect = painter.viewport();
                std::cout << rect.width() << " " << rect.height() << std::endl;
                QImage image( fi.absoluteFilePath() );
                painter.drawImage( rect, image, image.rect() );


            }
            QPixmap pixmap;

            for( int i = 0; i < 20; ++i ) {
                pixmap.load( fi.absoluteFilePath() );

                if( pixmap.width() != 0 ) {
                    break;
                }
                std::cout << "waiting: " << i << std::endl;
                QThread::usleep(200000);
            }



            show_pixmap(pixmap);
            QRectF rect = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->geometry()).boundingRect();
            QFont font;
            font.setPixelSize(200);
            QGraphicsSimpleTextItem  *text = scene_.addSimpleText( "printing", font);
            text->setBrush(QBrush(Qt::blue));
            text->setPos( rect.x(), rect.y());

            timer.start( 10000 );
//            p.prin
        }
    }
    file_set_.swap(new_set);



}

void MainWindow::show_pixmap( const QPixmap &image ) {
    scene_.clear();
    QGraphicsPixmapItem *item = scene_.addPixmap( image );

    QRectF rect = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->geometry()).boundingRect();//ui->graphicsView->visibleRegion().boundingRect();



    std::cout << "rect: " << rect.x() << " " << rect.y() << " " << rect.width() << " " << rect.height() << std::endl;
    std::cout << "image: " << image.width() << " " << image.height() << std::endl;
    qreal scalex = qreal(rect.width()) / image.width();
    qreal scaley = qreal(rect.height()) / image.height();
    qreal scale = std::min(scalex, scaley);

    qreal ws = image.width() * scale;
    qreal hs = image.height() * scale;

    QPointF offset(rect.x() + (rect.width() - ws) / 2.0, rect.y() + (rect.height() - hs) / 2.0 );

    //item->setPos( QPoint(rect.x() - w2, rect.y() - h2));
    item->setPos( offset );

    std::cout << scale << std::endl;
    item->setScale( scale );

}

void MainWindow::sl_timer_timeout()
{

    QList<QString> list = file_set_.toList();

    if( list.size() > 0 ) {
        size_t idx = qrand() % list.size();
        QFileInfo fi( path_, list[idx] );
        std::cout << "show: " << idx << " " << fi.absoluteFilePath().toStdString() << std::endl;
        QPixmap image( fi.absoluteFilePath() );


        show_pixmap( image );

        //ui->centralWidget->
    }
    timer.start( 5000 );
}
