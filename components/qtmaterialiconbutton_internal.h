#ifndef QTMATERIALICONBUTTON_INTERNAL_H
#define QTMATERIALICONBUTTON_INTERNAL_H

#include <QStateMachine>
#include <QColor>
#include "components/lib/qtmaterialtheme.h"

class QtMaterialIconButton;
class QSequentialAnimationGroup;

class QtMaterialIconButtonStateMachine : public QStateMachine
{
    Q_OBJECT

    Q_PROPERTY(qreal overlayOpacity WRITE setOverlayOpacity READ overlayOpacity)

public:
    explicit QtMaterialIconButtonStateMachine(QtMaterialIconButton *parent);
    ~QtMaterialIconButtonStateMachine();

    void setOverlayOpacity(qreal opacity);
    inline qreal overlayOpacity() const;

    void setupProperties();

private:
    Q_DISABLE_COPY(QtMaterialIconButtonStateMachine)

    void addTransition(QObject *object, QEvent::Type eventType, QState *fromState, QState *toState);
    void addTransition(QAbstractTransition *transition, QState *fromState, QState *toState);

    QtMaterialIconButton *const m_button;
    QState               *const m_hoveredState;
    QState               *const m_neutralState;
    qreal                       m_overlayOpacity;
};

inline qreal QtMaterialIconButtonStateMachine::overlayOpacity() const
{
    return m_overlayOpacity;
}

#endif // QTMATERIALICONBUTTON_INTERNAL_H
