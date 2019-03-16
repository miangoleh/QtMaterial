#include "qtmaterialiconbutton_internal.h"
#include <QEventTransition>
#include <QPropertyAnimation>
#include <QFocusEvent>
#include "qtmaterialiconbutton.h"
#include "components/lib/qtmaterialstatetransition.h"
#include <QDebug>

/*!
 *  \class QtMateriaIconButtonStateMachine
 *  \internal
 */

QtMaterialIconButtonStateMachine::QtMaterialIconButtonStateMachine(QtMaterialIconButton *parent)
    : QStateMachine(parent),
      m_button(parent),
      m_hoveredState(new QState(ExclusiveStates)),
      m_neutralState(new QState(ExclusiveStates)),
      m_overlayOpacity(0)
{
    Q_ASSERT(parent);

    parent->installEventFilter(this);

    addState(m_neutralState);
    addState(m_hoveredState);
    setInitialState(m_neutralState);

    addTransition(m_button, QEvent::Enter, m_neutralState, m_hoveredState);
    addTransition(m_button, QEvent::Leave, m_hoveredState, m_neutralState);
}

QtMaterialIconButtonStateMachine::~QtMaterialIconButtonStateMachine()
{

}

void QtMaterialIconButtonStateMachine::setOverlayOpacity(qreal opacity)
{
    m_overlayOpacity = opacity;
    m_button->update();
}

void QtMaterialIconButtonStateMachine::addTransition(QObject *object,
                                                     QEvent::Type eventType,
                                                     QState *fromState,
                                                     QState *toState)
{
    addTransition(new QEventTransition(object, eventType), fromState, toState);
}

void QtMaterialIconButtonStateMachine::addTransition(QAbstractTransition *transition,
                                                     QState *fromState,
                                                     QState *toState)
{
    transition->setTargetState(toState);

    QPropertyAnimation *animation;

    animation = new QPropertyAnimation(this, "overlayOpacity", this);
    animation->setDuration(150);
    transition->addAnimation(animation);

    fromState->addTransition(transition);
}

void QtMaterialIconButtonStateMachine::setupProperties()
{
    const qreal baseOpacity = 0.2;

    m_neutralState->assignProperty(this, "overlayOpacity", 0);
    m_hoveredState->assignProperty(this, "overlayOpacity", baseOpacity);

    m_button->update();
}
