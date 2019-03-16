#ifndef QTMATERIALICONBUTTON_H
#define QTMATERIALICONBUTTON_H

#include <QEnterEvent>

#include <QtWidgets/QAbstractButton>
#include "components/lib/qtmaterialtheme.h"

class QtMaterialIconButtonPrivate;

class QtMaterialIconButton : public QAbstractButton
{
    Q_OBJECT

public:
    explicit QtMaterialIconButton(const QIcon &icon=QIcon(), QWidget *parent = 0);
    ~QtMaterialIconButton();

    QSize sizeHint() const Q_DECL_OVERRIDE;

    void setUseThemeColors(bool value);
    bool useThemeColors() const;

    void setColor(const QColor &color);
    QColor color() const;

    void setDisabledColor(const QColor &color);
    QColor disabledColor() const;

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void setBackgroundMode(const Qt::BGMode mode);
    Qt::BGMode backgroundMode() const;

    void setRole(const Material::Role role);
    Material::Role role() const;

    void setOverlayStyle(const Material::OverlayStyle style);
    Material::OverlayStyle overlayStyle() const;

    void setOverlayShape(const Material::OverlayShape shape);
    Material::OverlayShape overlayShape() const;

    void setHasRipple(const bool hasRipple);
    bool hasRipple() const;


protected:
    QtMaterialIconButton(QtMaterialIconButtonPrivate &d, QWidget *parent = 0);

    bool event(QEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event)Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialIconButtonPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtMaterialIconButton)
    Q_DECLARE_PRIVATE(QtMaterialIconButton)

signals:
    void mouseEntered();
};

#endif // QTMATERIALICONBUTTON_H
