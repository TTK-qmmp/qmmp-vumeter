#include "vumeter.h"

#include <QMenu>
#include <QTimer>
#include <QPainter>
#include <math.h>

VUMeter::VUMeter(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("VU Meter Widget"));

    m_pixmap.load(":/data/vu");
    setFixedSize(m_pixmap.size());

    m_timer = new QTimer(this);
    m_timer->setInterval(40);
    connect(m_timer, SIGNAL(timeout()), SLOT(updateVisual()));
}

VUMeter::~VUMeter()
{

}

void VUMeter::start()
{
    if(isVisible())
        m_timer->start();
}

void VUMeter::stop()
{
    m_timer->stop();
}

void VUMeter::updateVisual()
{
    if(takeData(m_left, m_right))
    {
        process(m_left, m_right);
        update();
    }
}

void VUMeter::rangeChanged(QAction *action)
{
    m_rangeValue = action->data().toInt();
}

void VUMeter::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void VUMeter::showEvent(QShowEvent *)
{
    m_timer->start();
}

void VUMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.drawPixmap(rect(), m_pixmap);

    float value = 0;
    for(int c = 0; c < m_channels; ++c)
    {
        value = std::max(value, m_values[c]);
    }

    const int radius = 130;
    const int x = m_pixmap.width() / 2;
    const int y = m_pixmap.height() - 174 / 2;
    const float offset = M_PI * (value / (m_rangeValue * 2.5) - 0.75);

    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(x, y, x + radius * cos(offset), y + radius * sin(offset));
}

void VUMeter::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);

    QMenu rangeMenu(tr("Range"), &menu);
    rangeMenu.addAction(tr("30 DB"))->setData(30);
    rangeMenu.addAction(tr("50 DB"))->setData(50);
    rangeMenu.addAction(tr("70 DB"))->setData(70);
    rangeMenu.addAction(tr("90 DB"))->setData(90);
    rangeMenu.addAction(tr("110 DB"))->setData(110);
    rangeMenu.addAction(tr("130 DB"))->setData(130);
    rangeMenu.addAction(tr("150 DB"))->setData(150);
    rangeMenu.addAction(tr("170 DB"))->setData(170);
    connect(&rangeMenu, SIGNAL(triggered(QAction*)), this, SLOT(rangeChanged(QAction*)));
    menu.addMenu(&rangeMenu);
    menu.exec(QCursor::pos());
}

void VUMeter::process(float *left, float *right)
{
    const int channels = qBound(m_channels, 1, CHANNEL_MAX);

    float peaks[channels];
    for(int i = 0; i < channels; ++i)
    {
        peaks[i] = fabsf(i == 0 ? left[0] : right[0]);
    }

    for(int i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
    {
        for(int j = 0; j < channels; ++j)
        {
            peaks[j] = fmaxf(peaks[j], fabsf(j == 0 ? left[i] : right[i]));
        }
    }

    for(int i = 0; i < channels; ++i)
    {
        m_values[i] = 0;
        const float db = m_rangeValue + (20.0f * log10f(peaks[i]));

        if(db > m_values[i])
        {
            m_values[i] = db;
        }
    }
}
