#ifndef QTMATERIALHOVERSTATEMACHINE_H
#define QTMATERIALHOVERSTATEMACHINE_H

#include <QStateMachine>
#include <QtWidgets/QtWidgets>
#include <QColor>
#include "components/lib/qtmaterialtheme.h"


class QtMaterialHoverStateMachine : public QStateMachine
{
    Q_OBJECT

    Q_PROPERTY(qreal overlayOpacity WRITE setOverlayOpacity READ overlayOpacity)

public:
    explicit QtMaterialHoverStateMachine(QWidget *widget,Material::hoverActiveState state = Material::EnterLeave,int animationDutation = 150);
    ~QtMaterialHoverStateMachine();

    void setOverlayOpacity(qreal opacity);
    inline qreal overlayOpacity() const;

    void setBaseOpacity(const qreal baseopacity);
    qreal baseOpacity() const;

    void setNeutral2HoverTransition(QObject *object, QEvent::Type eventType);
    void setHover2NeutralTransition(QObject *object, QEvent::Type eventType);

    void setupProperties();

    int getAnimationDuration() const;

private:
    Q_DISABLE_COPY(QtMaterialHoverStateMachine)

    void addTransition(QObject *object, QEvent::Type eventType, QState *fromState, QState *toState);
    void addTransition(QAbstractTransition *transition, QState *fromState, QState *toState);

    QWidget              *const b_widget;
    QState               *const m_hoveredState;
    QState               *const m_neutralState;
    qreal                       m_overlayOpacity;
    qreal                       m_baseOpacity;
    int                         m_animationDuration;
};

inline qreal QtMaterialHoverStateMachine::overlayOpacity() const
{
    return m_overlayOpacity;
}

#endif // QTMATERIALHOVERSTATEMACHINE_H
