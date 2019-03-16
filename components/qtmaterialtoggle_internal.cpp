#include "qtmaterialtoggle_internal.h"
#include <QPainter>
#include <QEvent>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include "qtmaterialtoggle.h"
#include "components/lib/qtmaterialripple.h"

/*!
 *  \class QtMaterialToggleRippleOverlay
 *  \internal
 */

QtMaterialToggleRippleOverlay::QtMaterialToggleRippleOverlay(
        QtMaterialToggleThumb *thumb,
        QtMaterialToggleTrack *track,
        QtMaterialToggle      *parent)
    : QtMaterialRippleOverlay(parent->parentWidget()),
      m_toggle(parent),
      m_thumb(thumb),
      m_track(track)
{
    connect(parent, SIGNAL(toggled(bool)), this, SLOT(addToggleRipple()));

    thumb->installEventFilter(this);
}

QtMaterialToggleRippleOverlay::~QtMaterialToggleRippleOverlay()
{
}

void QtMaterialToggleRippleOverlay::addToggleRipple()
{
    if (!m_toggle->isEnabled()) {
        return;
    }

    int t, w;

    if (Qt::Horizontal == m_toggle->orientation()) {
        t = m_toggle->height()/2;
        w = m_thumb->height()/2 + m_thumb->height()*0.15;
    } else {
        t = m_toggle->width()/2;
        w = m_thumb->width()/2;
    }

    QtMaterialRipple *ripple = new QtMaterialRipple(QPoint(t,t));
    ripple->setColor(m_track->trackColor());
    ripple->setRadiusEndValue(w);
    ripple->setOpacityStartValue(0.8);

    addRipple(ripple);
}

bool QtMaterialToggleRippleOverlay::eventFilter(QObject *obj, QEvent *event)
{
    if (QEvent::Paint == event->type()) {
        setGeometry(overlayGeometry());
        QList<QtMaterialRipple *>::const_iterator i;
        QList<QtMaterialRipple *> items = ripples();
        QColor color = m_track->trackColor();
        for (i = items.begin(); i != items.end(); ++i) {
            (*i)->setColor(color);
        }
    }
    return QtMaterialRippleOverlay::eventFilter(obj, event);
}

QRect QtMaterialToggleRippleOverlay::overlayGeometry() const
{
    const qreal offset = m_thumb->offset();
    if (Qt::Horizontal == m_toggle->orientation()) {
        return m_toggle->geometry().adjusted(offset, 0, offset,0);
    } else {
        return m_toggle->geometry().adjusted(0, offset, 0,offset);
    }
}

/*!
 *  \class QtMaterialToggleThumb
 *  \internal
 */

QtMaterialToggleThumb::QtMaterialToggleThumb(QtMaterialToggle *parent)
    : QWidget(parent),
      m_toggle(parent),
      m_shift(0),
      m_offset(0)
{
    Q_ASSERT(parent);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(6);
    effect->setColor(QColor(0, 0, 0, 80));
    effect->setOffset(QPointF(0, 1));
    setGraphicsEffect(effect);

    parent->installEventFilter(this);
}

QtMaterialToggleThumb::~QtMaterialToggleThumb()
{
}

void QtMaterialToggleThumb::setShift(qreal shift)
{
    if (m_shift == shift) {
        return;
    }

    m_shift = shift;
    updateOffset();
}

bool QtMaterialToggleThumb::eventFilter(QObject *obj, QEvent *event)
{
    const QEvent::Type type = event->type();

    if (QEvent::Resize == type || QEvent::Move == type)
    {
        setGeometry(m_toggle->rect());
        updateOffset();
    }
    return QWidget::eventFilter(obj, event);
}

void QtMaterialToggleThumb::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(m_toggle->isEnabled() ? m_thumbColor : Qt::white);

    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);

    int s;
    QRectF r;

    if (Qt::Horizontal == m_toggle->orientation()) {
        s = height()*0.8;
        r = QRectF(m_offset,height()*0.1, s, s);
    } else {
        s = width()*0.8;
        r = QRectF(height()*0.1,m_offset, s, s);
    }

    painter.drawEllipse(r);

    if (!m_toggle->isEnabled()) {
        brush.setColor(m_toggle->disabledColor());
        painter.setBrush(brush);
        painter.drawEllipse(r);
    }
}

void QtMaterialToggleThumb::updateOffset()
{
    const QSize s(Qt::Horizontal == m_toggle->orientation()
        ? size() : size().transposed());
    m_offset = m_shift*static_cast<qreal>(s.width()-1.1*s.height());
    qDebug() << "QtMaterialToggleThumb::updateOffset" << m_offset;
    update();
}

/*!
 *  \class QtMaterialToggleTrack
 *  \internal
 */

QtMaterialToggleTrack::QtMaterialToggleTrack(QtMaterialToggle *parent)
    : QWidget(parent),
      m_toggle(parent)
{
    Q_ASSERT(parent);

    parent->installEventFilter(this);
}

QtMaterialToggleTrack::~QtMaterialToggleTrack()
{
}

void QtMaterialToggleTrack::setTrackColor(const QColor &color)
{
    m_trackColor = color;
    update();
}

bool QtMaterialToggleTrack::eventFilter(QObject *obj, QEvent *event)
{
    const QEvent::Type type = event->type();

    if (QEvent::Resize == type || QEvent::Move == type) {
        setGeometry(m_toggle->rect());
    }
    return QWidget::eventFilter(obj, event);
}

void QtMaterialToggleTrack::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush;
    if (m_toggle->isEnabled()) {
        brush.setColor(m_trackColor);
        painter.setOpacity(0.8);
    } else {
        brush.setColor(m_toggle->disabledColor());
        painter.setOpacity(0.6);
    }
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);

    if (Qt::Horizontal == m_toggle->orientation()) {
        const int h = height()*0.2;
        const QRect r(0, 0, width()*0.8, height()*0.6);
        painter.drawRoundedRect(r.adjusted(width()*0.1,h,-width()*0.1,h),height()*0.3, height()*0.3);
    } else {
        const int w = width()*0.2;
        const QRect r(0, 0, w, height());
        painter.drawRoundedRect(r.adjusted(0, w, 0, w), w/2, w/2);
    }
}
