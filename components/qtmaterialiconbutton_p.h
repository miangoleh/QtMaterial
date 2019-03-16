#ifndef QTMATERIALICONBUTTON_P_H
#define QTMATERIALICONBUTTON_P_H

#include <QtGlobal>
#include <QColor>
#include "components/lib/qtmaterialtheme.h"

class QtMaterialIconButton;
class QtMaterialRippleOverlay;
class QtMaterialIconButtonStateMachine;

class QtMaterialIconButtonPrivate
{
    Q_DISABLE_COPY(QtMaterialIconButtonPrivate)
    Q_DECLARE_PUBLIC(QtMaterialIconButton)

public:
    QtMaterialIconButtonPrivate(QtMaterialIconButton *q);
    virtual ~QtMaterialIconButtonPrivate();

    void init();
    void updateRipple();

    QtMaterialIconButton                *const q_ptr;
    QtMaterialRippleOverlay             *rippleOverlay;
    bool                                hasRipple;
    QtMaterialIconButtonStateMachine    *stateMachine;
    Material::OverlayStyle              overlayStyle;
    Material::OverlayShape              overlayShape;
    QColor                              color;
    Material::Role                      role;
    QColor                              disabledColor;
    QColor                              backgroundColor;
    bool                                useThemeColors;
    Qt::BGMode                          backgroundMode;


};

#endif // QTMATERIALICONBUTTON_P_H
