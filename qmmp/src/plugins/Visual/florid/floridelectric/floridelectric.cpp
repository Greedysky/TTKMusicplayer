#include <QPainter>
#include "fft.h"
#include "inlines.h"
#include "floridelectric.h"

#include <QPropertyAnimation>

ElectricPointLabel::ElectricPointLabel(QWidget *parent)
    : QWidget(parent)
{
    m_posAnimation = new QPropertyAnimation(this, QByteArray(), this);
    m_posAnimation->setDuration(ANIMATION_DURATION);
    m_posAnimation->setEasingCurve(QEasingCurve::InOutSine);

    connect(m_posAnimation, SIGNAL(finished()), SLOT(finished()));
    connect(m_posAnimation, SIGNAL(valueChanged(QVariant)), SLOT(posValueChanged(QVariant)));
}

ElectricPointLabel::~ElectricPointLabel()
{
    delete m_posAnimation;
}

void ElectricPointLabel::start()
{
    QTimer::singleShot(qrand() % ANIMATION_DURATION, this, SLOT(timeout()));
}

void ElectricPointLabel::setColor(const QColor &color)
{
    m_color = color;
}

void ElectricPointLabel::timeout()
{
    m_pos = rect().center();
    m_size = qrand() % DISTANCE / 5 + 1;
    int pos_x = qrand() % (DISTANCE * 3), pos_y = qrand() % (DISTANCE * 3);
    if(pos_x % 2 == 0)
    {
        pos_x = -pos_x;
    }
    if(pos_y % 2 == 0)
    {
        pos_y = -pos_y;
    }

    m_posAnimation->setStartValue(m_pos);
    m_posAnimation->setEndValue(m_pos + QPoint(DISTANCE / 2 + pos_x, DISTANCE / 2 + pos_y));
    m_posAnimation->start();
}

void ElectricPointLabel::finished()
{
    start();
}

void ElectricPointLabel::posValueChanged(const QVariant &value)
{
    m_pos = value.toPoint();

    const QPoint &startPoint = m_posAnimation->startValue().toPoint();
    const QPoint &endPoint = m_posAnimation->endValue().toPoint();
    const int totalLength = sqrt(pow(startPoint.x() - endPoint.x(), 2) + pow(startPoint.y() - endPoint.y(), 2));
    const int currentLength = sqrt(pow(startPoint.x() - m_pos.x(), 2) + pow(startPoint.y() - m_pos.y(), 2));
    const float delta = (totalLength - currentLength) * 1.0 / totalLength;
    m_opacity = delta;

    update();
}

void ElectricPointLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    if(m_pos == QPoint(0, 0))
    {
        return;
    }

    QPainter painter(this);
    painter.setOpacity(m_opacity);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(m_color, 2));
    painter.drawEllipse(m_pos, m_size, m_size);
}



ElectricCircleLabel::ElectricCircleLabel(QWidget *parent)
    : QWidget(parent)
{
    m_posAnimation = new QPropertyAnimation(this, QByteArray(), this);
    m_posAnimation->setDuration(ANIMATION_DURATION / 2);
    m_posAnimation->setEasingCurve(QEasingCurve::InOutSine);

    connect(m_posAnimation, SIGNAL(finished()), SLOT(finished()));
    connect(m_posAnimation, SIGNAL(valueChanged(QVariant)), SLOT(sizeValueChanged(QVariant)));
}

ElectricCircleLabel::~ElectricCircleLabel()
{
    delete m_posAnimation;
}

void ElectricCircleLabel::start()
{
    QTimer::singleShot(qrand() % (ANIMATION_DURATION / 2), this, SLOT(timeout()));
}

void ElectricCircleLabel::setColor(const QColor &color)
{
    m_color = color;
}

void ElectricCircleLabel::finished()
{
    start();
}

void ElectricCircleLabel::timeout()
{
    m_posAnimation->setStartValue(DISTANCE);
    m_posAnimation->setKeyValueAt(0.5, DISTANCE + qrand() % (DISTANCE / 2));
    m_posAnimation->setEndValue(DISTANCE);
    m_posAnimation->start();
}

void ElectricCircleLabel::sizeValueChanged(const QVariant &value)
{
    m_size = value.toInt();
    m_opacity = 1.3 - (m_size - DISTANCE) * 1.0 / (m_posAnimation->keyValueAt(0.5).toInt() - DISTANCE);

    update();
}

void ElectricCircleLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    if(m_size == POINT_SIZE)
    {
        return;
    }

    QPainter painter(this);
    painter.setOpacity(m_opacity);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(m_color);
    painter.setPen(QPen(m_color, 1));
    painter.drawEllipse(rect().center(), m_size, m_size);
}



ElectricLabel::ElectricLabel(QWidget *parent)
{
    for(int i=0; i<ANIMATION_SIZE * 5; ++i)
    {
        ElectricPointLabel *label = new ElectricPointLabel(parent);
        label->setGeometry(parent->geometry());
        m_pointLabels << label;
    }

    for(int i=0; i<ANIMATION_SIZE; ++i)
    {
        ElectricCircleLabel *label = new ElectricCircleLabel(parent);
        label->setGeometry(parent->geometry());
        m_circleLabels << label;
    }
}

ElectricLabel::~ElectricLabel()
{
    qDeleteAll(m_pointLabels);
    qDeleteAll(m_circleLabels);
}

void ElectricLabel::start()
{
    for(ElectricPointLabel *label : qAsConst(m_pointLabels))
    {
        label->start();
    }

    for(ElectricCircleLabel *label : qAsConst(m_circleLabels))
    {
        label->start();
    }
}

void ElectricLabel::setColor(const QColor &color)
{
    for(ElectricPointLabel *label : qAsConst(m_pointLabels))
    {
        label->setColor(color);
    }

    for(ElectricCircleLabel *label : qAsConst(m_circleLabels))
    {
        label->setColor(color);
    }
}

void ElectricLabel::resizeWindow(const QRect &rect)
{
    for(ElectricPointLabel *label : qAsConst(m_pointLabels))
    {
        label->setGeometry(rect);
    }

    for(ElectricCircleLabel *label : qAsConst(m_circleLabels))
    {
        label->setGeometry(rect);
    }
}



FloridElectric::FloridElectric(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Electric Widget"));

    m_label = new ElectricLabel(this);
}

FloridElectric::~FloridElectric()
{

}

void FloridElectric::start()
{
    Florid::start();
    m_label->start();
}

void FloridElectric::stop()
{
    Florid::stop();
}

void FloridElectric::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);
}

void FloridElectric::resizeEvent(QResizeEvent *e)
{
    Florid::resizeEvent(e);
    m_label->resizeWindow(QRect(0, 0, width(), height()));
    m_roundLabel->raise();
}

void FloridElectric::process(float *, float *)
{
    m_label->setColor(m_averageColor);
}
