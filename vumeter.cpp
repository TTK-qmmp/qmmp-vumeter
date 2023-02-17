#include "vumeter.h"

#include <QMenu>
#include <QTimer>
#include <QPainter>
#include <math.h>

static void actionChecked(QAction *action, int value, int data)
{
    action->setData(value);
    action->setCheckable(true);
    action->setChecked(data == value);
}


VUMeter::VUMeter(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("VU Meter Widget"));

    m_pixmap.load(":/data/vu");
    setMinimumSize(m_pixmap.size());

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
    {
        m_timer->start();
    }
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
    painter.fillRect(rect(), Qt::black);

    const int w = (width() - m_pixmap.width()) / 2;
    const int h = (height() - m_pixmap.height()) / 2;
    painter.drawPixmap(w, h, m_pixmap);

    float value = 0;
    for(int c = 0; c < m_channels; ++c)
    {
        value = std::max(value, m_values[c]);
    }

    constexpr int radius = 130;
    const int x = w + m_pixmap.width() / 2;
    const int y = h + m_pixmap.height() - 174 / 2;
    const float offset = M_PI * (value / (m_rangeValue * 2.5) - 0.75);

    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(x, y, x + radius * cos(offset), y + radius * sin(offset));
}

void VUMeter::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);

    QMenu rangeMenu(tr("Range"), &menu);
    actionChecked(rangeMenu.addAction(tr("30 DB")), 30, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("50 DB")), 50, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("70 DB")), 70, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("90 DB")), 90, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("110 DB")), 110, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("130 DB")), 130, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("150 DB")), 150, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("170 DB")), 170, m_rangeValue);
    connect(&rangeMenu, SIGNAL(triggered(QAction*)), this, SLOT(rangeChanged(QAction*)));
    menu.addMenu(&rangeMenu);
    menu.exec(QCursor::pos());
}

void VUMeter::process(float *left, float *right)
{
    const int channels = qBound(m_channels, 1, MAX_CHANNELS);

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
