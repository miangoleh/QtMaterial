#include "qtmaterialhoverstatemachine.h"

QtMaterialHoverStateMachine::QtMaterialHoverStateMachine(QWidget *widget, Material::hoverActiveState state)
    : QStateMachine (widget),
      b_widget(widget),
      m_hoveredState(new QState(ExclusiveStates)),
      m_neutralState(new QState(ExclusiveStates)),
      m_overlayOpacity(0),
      m_baseOpacity(0.2)
{
    Q_ASSERT(widget);

    widget->installEventFilter(this);

    addState(m_neutralState);
    addState(m_hoveredState);
    setInitialState(m_neutralState);

    if(state == Material::EnterLeave)
    {
        addTransition(b_widget, QEvent::Enter, m_neutralState, m_hoveredState);
        addTransition(b_widget, QEvent::Leave, m_hoveredState, m_neutralState);
    }
    else if(state == Material::HideShow)
    {
        addTransition(b_widget, QEvent::Show, m_neutralState, m_hoveredState);
        addTransition(b_widget, QEvent::Hide, m_hoveredState, m_neutralState);
    }
}

QtMaterialHoverStateMachine::~QtMaterialHoverStateMachine()
{

}

void QtMaterialHoverStateMachine::setOverlayOpacity(qreal opacity)
{
    m_overlayOpacity = opacity;
    setupProperties();
}

void QtMaterialHoverStateMachine::setBaseOpacity(const qreal baseopacity)
{
    m_baseOpacity = baseopacity;
    b_widget->update();
}

qreal QtMaterialHoverStateMachine::baseOpacity() const
{
    return m_baseOpacity;
}

void QtMaterialHoverStateMachine::setNeutral2HoverTransition(QObject *object, QEvent::Type eventType)
{
    addTransition(object, eventType, m_neutralState, m_hoveredState);
}

void QtMaterialHoverStateMachine::setHover2NeutralTransition(QObject *object, QEvent::Type eventType)
{
    addTransition(object, eventType, m_hoveredState, m_neutralState);
}

void QtMaterialHoverStateMachine::addTransition(QObject *object,
                                                     QEvent::Type eventType,
                                                     QState *fromState,
                                                     QState *toState)
{
    addTransition(new QEventTransition(object, eventType), fromState, toState);
}

void QtMaterialHoverStateMachine::addTransition(QAbstractTransition *transition,
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

void QtMaterialHoverStateMachine::setupProperties()
{
    m_neutralState->assignProperty(this, "overlayOpacity", 0);
    m_hoveredState->assignProperty(this, "overlayOpacity", m_baseOpacity);

    b_widget->update();
}
