#include "qtmaterialiconbutton.h"
#include "qtmaterialiconbutton_p.h"
#include <QPainter>
#include <QEvent>
#include "components/lib/qtmaterialstyle.h"
#include "components/lib/qtmaterialrippleoverlay.h"
#include "qtmaterialiconbutton_internal.h"

/*!
 *  \class QtMaterialIconButtonPrivate
 *  \internal
 */


QtMaterialIconButtonPrivate::QtMaterialIconButtonPrivate(QtMaterialIconButton *q)
    : q_ptr(q)
{
}

QtMaterialIconButtonPrivate::~QtMaterialIconButtonPrivate()
{
}

void QtMaterialIconButtonPrivate::init()
{
    Q_Q(QtMaterialIconButton);

    rippleOverlay  = new QtMaterialRippleOverlay(q->parentWidget());
    stateMachine         = new QtMaterialIconButtonStateMachine(q);
    useThemeColors = true;

    role = Material::Default;
    overlayStyle = Material::NoOverlay;
    overlayShape = Material::Box;
    hasRipple = true;
    backgroundMode = Qt::TransparentMode;

    rippleOverlay->installEventFilter(q);

    q->setStyle(&QtMaterialStyle::instance());

    QSizePolicy policy;
    policy.setWidthForHeight(true);
    q->setSizePolicy(policy);

    q->setAttribute(Qt::WA_Hover);
    stateMachine->setupProperties();
    stateMachine->start();
}

void QtMaterialIconButtonPrivate::updateRipple()
{
    Q_Q(QtMaterialIconButton);

    QRect r(q->rect());
    r.setSize(QSize(q->width()*2, q->height()*2));
    r.moveCenter(q->geometry().center());
    rippleOverlay->setGeometry(r);
}

/*!
 *  \class QtMaterialIconButton
 */

QtMaterialIconButton::QtMaterialIconButton(const QIcon &icon, QWidget *parent)
    : QAbstractButton(parent),
      d_ptr(new QtMaterialIconButtonPrivate(this))
{
    d_func()->init();

    setIcon(icon);
}

QtMaterialIconButton::~QtMaterialIconButton()
{
}

/*!
 *  \reimp
 */
QSize QtMaterialIconButton::sizeHint() const
{
    return iconSize();
}

void QtMaterialIconButton::setUseThemeColors(bool value)
{
    Q_D(QtMaterialIconButton);

    if (d->useThemeColors == value) {
        return;
    }

    d->useThemeColors = value;
    update();
}

bool QtMaterialIconButton::useThemeColors() const
{
    Q_D(const QtMaterialIconButton);

    return d->useThemeColors;
}

void QtMaterialIconButton::setColor(const QColor &color)
{
    Q_D(QtMaterialIconButton);

    d->color = color;

    MATERIAL_DISABLE_THEME_COLORS
            update();
}

QColor QtMaterialIconButton::color() const
{
    Q_D(const QtMaterialIconButton);

    if (d->useThemeColors || !d->color.isValid()) {
        switch (d->role)
        {
        case Material::Primary:
            return QtMaterialStyle::instance().themeColor("primary");
        case Material::Secondary:
            return QtMaterialStyle::instance().themeColor("secondary");
        case Material::Gray:
            return QtMaterialStyle::instance().themeColor("gray");
        case Material::Warning:
        case Material::Default:
        default:
            return QtMaterialStyle::instance().themeColor("text");
        }
    }
    return d->color;
}

void QtMaterialIconButton::setDisabledColor(const QColor &color)
{
    Q_D(QtMaterialIconButton);

    d->disabledColor = color;

    MATERIAL_DISABLE_THEME_COLORS
            update();
}

QColor QtMaterialIconButton::disabledColor() const
{
    Q_D(const QtMaterialIconButton);

    if (d->useThemeColors || !d->disabledColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("disabled");
    }
    return d->disabledColor;
}

void QtMaterialIconButton::setBackgroundColor(const QColor &color)
{
    Q_D(QtMaterialIconButton);

    d->backgroundColor = color;

    MATERIAL_DISABLE_THEME_COLORS

    setBackgroundMode(Qt::OpaqueMode);
}

QColor QtMaterialIconButton::backgroundColor() const
{
    Q_D(const QtMaterialIconButton);

    if (d->useThemeColors || !d->backgroundColor.isValid()) {
        return color();
    }
    return d->backgroundColor;
}

void QtMaterialIconButton::setBackgroundMode(const Qt::BGMode mode)
{
    Q_D(QtMaterialIconButton);

    d->backgroundMode = mode;
}

Qt::BGMode QtMaterialIconButton::backgroundMode() const
{
    Q_D(const QtMaterialIconButton);

    return d->backgroundMode;
}

void QtMaterialIconButton::setRole(const Material::Role role)
{
    Q_D(QtMaterialIconButton);

    d->role = role;
    update();
}

Material::Role QtMaterialIconButton::role() const
{
    Q_D(const QtMaterialIconButton);

    return d->role;
}

void QtMaterialIconButton::setOverlayStyle(const Material::OverlayStyle style)
{
    Q_D(QtMaterialIconButton);

    d->overlayStyle = style;
    update();
}

Material::OverlayStyle QtMaterialIconButton::overlayStyle() const
{
    Q_D(const QtMaterialIconButton);

    return d->overlayStyle;
}

void QtMaterialIconButton::setOverlayShape(const Material::OverlayShape shape)
{
    Q_D(QtMaterialIconButton);

    d->overlayShape = shape;
    update();
}

Material::OverlayShape QtMaterialIconButton::overlayShape() const
{
    Q_D(const QtMaterialIconButton);

    return d->overlayShape;
}

void QtMaterialIconButton::setHasRipple(const bool hasRipple)
{
    Q_D(QtMaterialIconButton);

    d->hasRipple = hasRipple;
}

bool QtMaterialIconButton::hasRipple() const
{
    Q_D(const QtMaterialIconButton);

    return d->hasRipple;
}

QtMaterialIconButton::QtMaterialIconButton(QtMaterialIconButtonPrivate &d, QWidget *parent)
    : QAbstractButton(parent),
      d_ptr(&d)
{
    d_func()->init();
}

/*!
 *  \reimp
 */
bool QtMaterialIconButton::event(QEvent *event)
{
    Q_D(QtMaterialIconButton);

    switch (event->type())
    {
    case QEvent::Move:
    case QEvent::Resize:
        d->updateRipple();
        break;
    case QEvent::ParentChange: {
        QWidget *widget;
        if ((widget = parentWidget())) {
            d->rippleOverlay->setParent(widget);
        }
        break;
    }
    default:
        break;
    }
    return QAbstractButton::event(event);
}

/*!
 *  \reimp
 */
bool QtMaterialIconButton::eventFilter(QObject *obj, QEvent *event)
{
    if (QEvent::Resize == event->type())
    {
        Q_D(QtMaterialIconButton);

        d->updateRipple();
    }
    return QAbstractButton::eventFilter(obj, event);
}

/*!
 *  \reimp
 */
void QtMaterialIconButton::mousePressEvent(QMouseEvent *event)
{
    Q_D(QtMaterialIconButton);
    if(hasRipple())
        d->rippleOverlay->addRipple(QPoint(d->rippleOverlay->width(),
                                           d->rippleOverlay->height())/2,
                                    iconSize().width());

    QAbstractButton::mousePressEvent(event);
}

/*!
 *  \reimp
 */
void QtMaterialIconButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if(overlayStyle() != Material::NoOverlay)
    {
    Q_D(const QtMaterialIconButton);

    qreal overlayopacity = d->stateMachine->overlayOpacity();

    painter.setRenderHint(QPainter::HighQualityAntialiasing,true);
    painter.setOpacity(overlayStyle() == Material::TintedOverlay ? overlayopacity*3 : overlayopacity);
    QColor overlayColor = d->role == Material::Warning ?  QtMaterialStyle::instance().themeColor("Warning") : color();
    QColor grayColor =  QtMaterialStyle::instance().themeColor("hover");
    grayColor.setAlpha(255);
    painter.setBrush(overlayStyle() == Material::TintedOverlay ? overlayColor :grayColor);
    painter.setPen(Qt::NoPen);

    if(d->overlayShape == Material::Box)
        painter.drawRect(0, 0, width(), height());
    else
        painter.drawEllipse(0, 0, width(), height());
    }

    QPixmap pixmap = icon().pixmap(iconSize());
    pixmap.setDevicePixelRatio(devicePixelRatio());


    QRect r = QRect(rect().x(), rect().y(),
                    rect().width(), rect().height());
    const qreal w = pixmap.width()/devicePixelRatio();
    const qreal h = pixmap.height()/devicePixelRatio();

    QPainter icon(&pixmap);
    QRect pixRect = QRect(pixmap.rect().x(), pixmap.rect().y(),
                          pixmap.rect().width(), pixmap.rect().height());
    if(backgroundMode() == Qt::OpaqueMode)
    {
        painter.setRenderHint(QPainter::HighQualityAntialiasing,true);
        painter.setOpacity(0.3);
        painter.setBrush(backgroundColor());
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect());

        icon.setCompositionMode(QPainter::CompositionMode_SourceIn);
        QColor icColor;
        if(backgroundColor() != color())
            icColor = color();
        else
            icColor = QtMaterialStyle::instance().themeColor("opqicon");
        icon.fillRect(pixRect, isEnabled() ? icColor : disabledColor());
    }
    else
    {
        icon.setCompositionMode(QPainter::CompositionMode_SourceIn);
        icon.fillRect(pixRect, isEnabled() ? color() : disabledColor());
    }

    painter.setOpacity(1);
    painter.drawPixmap(QRect((r.width()-w)/2, (r.height()-h)/2, w, h), pixmap);
}

void QtMaterialIconButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    emit mouseEntered();
}
