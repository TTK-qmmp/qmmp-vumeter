#include "visualvumeterfactory.h"
#include "vumeter.h"

#include <QMessageBox>

VisualProperties VisualVUMeterFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("VU Meter Plugin");
    properties.shortName = "vumeter";
    properties.hasAbout = true;
    return properties;
}

Visual *VisualVUMeterFactory::create(QWidget *parent)
{
    return new VUMeter(parent);
}

#if (QMMP_VERSION_INT < 0x10700) || (0x20000 <= QMMP_VERSION_INT && QMMP_VERSION_INT < 0x20200)
QDialog *VisualVUMeterFactory::createConfigDialog(QWidget *parent)
#else
QDialog *VisualVUMeterFactory::createSettings(QWidget *parent)
#endif
{
    Q_UNUSED(parent);
    return nullptr;
}

void VisualVUMeterFactory::showAbout(QWidget *parent)
{
    QMessageBox::about(parent, tr("About VU Meter Visual Plugin"),
                       tr("Qmmp VU Meter Visual Plugin") + "\n" +
                       tr("This plugin adds VU Meter visualization") + "\n" +
                       tr("Written by: Greedysky <greedysky@163.com>"));
}

QString VisualVUMeterFactory::translation() const
{
    return QString();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(vumeter, VisualVUMeterFactory)
#endif
