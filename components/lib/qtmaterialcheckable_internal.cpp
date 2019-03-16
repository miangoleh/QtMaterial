#include "components/lib/qtmaterialcheckable_internal.h"
#include <QPainter>
#include <QIcon>
#include <QTransform>
#include "components/lib/qtmaterialcheckable.h"

/*!
 *  \class QtMaterialCheckableIcon
 *  \internal
 */

QtMaterialCheckableIcon::QtMaterialCheckableIcon(const QIcon &icon, QtMaterialCheckable *parent)
    : QWidget(parent),
      m_checkable(parent),
      m_color(Qt::black),
      m_icon(icon),
      m_iconSize(24),
      m_opacity(1.0)
{
    Q_ASSERT(parent);

    setAttribute(Qt::WA_TransparentForMouseEvents);
}

QtMaterialCheckableIcon::~QtMaterialCheckableIcon()
{
}

QSize QtMaterialCheckableIcon::sizeHint() const
{
    return QSize(m_iconSize, m_iconSize);
}

void QtMaterialCheckableIcon::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(m_opacity);

    int pixmapSize;
    if(m_iconSize == icon().pixmap(m_iconSize,m_iconSize).width())
        pixmapSize = m_iconSize;
    else
        pixmapSize = 24;
    QPixmap pixmap = icon().pixmap(pixmapSize, pixmapSize);

    if (!pixmap.isNull())
    {
        const qreal p = static_cast<qreal>((height())-m_iconSize)/2;
        const qreal z = m_iconSize/pixmapSize;

        QTransform t;
        if (QtMaterialCheckable::LabelPositionLeft == m_checkable->labelPosition()) {
            t.translate(p+ width() - (m_checkable->iconSize()), p);
        } else {
            t.translate(p, p);
        }
        t.scale(z, z);
        painter.setTransform(t);

        QPainter icon(&pixmap);
        icon.setCompositionMode(QPainter::CompositionMode_SourceIn);
        icon.fillRect(pixmap.rect(), color());
        painter.drawPixmap(0, 0, pixmap);
    }
}
