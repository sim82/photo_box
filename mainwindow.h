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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QSet>
#include <QTimer>
#include <QGraphicsScene>
#include <QShortcut>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void show_pixmap(const QPixmap &image);
public Q_SLOTS:
    void on_pbFullscreen_clicked();
    void on_watcher_directoryChanged(const QString &path);
    void sl_timer_timeout();
private:
    Ui::MainWindow *ui;
    bool is_fullscreen_;
    QFileSystemWatcher watcher;
    QSet<QString> file_set_;
    QTimer timer;
    QString path_;
    QGraphicsScene scene_;
    QShortcut fullscreen_;
};

#endif // MAINWINDOW_H
