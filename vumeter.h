/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef VUMETER_H
#define VUMETER_H

#include <qmmp/visual.h>

#define MAX_CHANNELS    2

class QMenu;
class QActionGroup;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class VUMeter : public Visual
{
    Q_OBJECT
public:
    explicit VUMeter(QWidget *parent = nullptr);

public slots:
    virtual void start() override final;
    virtual void stop() override final;

private slots:
    void readSettings();
    void writeSettings();
    void updateVisual();

private:
    virtual void hideEvent(QHideEvent *e) override final;
    virtual void showEvent(QShowEvent *e) override final;
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *e) override final;

    void process(float *left, float *right);
    void createMenu();

    QPixmap m_pixmap;
    int m_rangeValue = 30;
    const int m_channels = 2;
    float m_values[MAX_CHANNELS] = {0};
    float m_left[QMMP_VISUAL_NODE_SIZE];
    float m_right[QMMP_VISUAL_NODE_SIZE];
    QTimer *m_timer = nullptr;

    QMenu *m_menu;
    QActionGroup *m_rangeActions;

};

#endif
