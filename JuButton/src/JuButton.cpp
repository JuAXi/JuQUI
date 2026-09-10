#include "JuButton.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPropertyAnimation>

JuButton::JuButton(QString text,
	QWidget* parent,
	ju_button::ColorSet base_color,
	ju_button::ColorSet hover_color,
	ju_button::ColorSet press_color,
	int animation_ms) : QWidget(parent)
{
	label = new QLabel(text, this);
	label->setAlignment(Qt::AlignCenter);
	layout = new QHBoxLayout(this);
	layout->addWidget(label);

	this->setCursor(Qt::PointingHandCursor);

	background_animation = new QPropertyAnimation(this, "background_color", this);
	border_animation = new QPropertyAnimation(this, "border_color", this);
	text_animation = new QPropertyAnimation(this, "text_color", this);

	background_animation->setDuration(animation_ms);
	border_animation->setDuration(animation_ms);
	text_animation->setDuration(animation_ms);

	SetBaseColor(base_color);
	SetHoverColor(hover_color);
	SetPressColor(press_color);
}

JuButton::~JuButton()
{
}

void JuButton::SetBaseColor(ju_button::ColorSet colors)
{
	this->base_color = colors;
	if (current_state == 0)
	{
		background_color = colors.button_background_color;
		border_color = colors.button_border_color;
		text_color = colors.text_color;

		this->setStyleSheet(QString("QWidget { background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
			.arg(background_color.red()).arg(background_color.green()).arg(background_color.blue())
			.arg(border_color.red()).arg(border_color.green()).arg(border_color.blue())
			.arg(radius));
		label->setStyleSheet(QString("QLabel { color: %1}").arg(text_color.name()));
	}
}

void JuButton::SetHoverColor(ju_button::ColorSet colors)
{
	this->hover_color = colors;
	if ((current_state & 1) && (!(current_state & 0b10)))
	{
		background_color = colors.button_background_color;
		border_color = colors.button_border_color;
		text_color = colors.text_color;

		this->setStyleSheet(QString("QWidget { background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
			.arg(background_color.red()).arg(background_color.green()).arg(background_color.blue())
			.arg(border_color.red()).arg(border_color.green()).arg(border_color.blue())
			.arg(radius));
		label->setStyleSheet(QString("QLabel { color: %1}").arg(text_color.name()));
	}
}

void JuButton::SetPressColor(ju_button::ColorSet colors)
{
	this->press_color = colors;
	if (current_state & 0b10)
	{
		background_color = colors.button_background_color;
		border_color = colors.button_border_color;
		text_color = colors.text_color;

		this->setStyleSheet(QString("QWidget { background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
			.arg(background_color.red()).arg(background_color.green()).arg(background_color.blue())
			.arg(border_color.red()).arg(border_color.green()).arg(border_color.blue())
			.arg(radius));

		label->setStyleSheet(QString("QLabel { color: %1}").arg(text_color.name()));
	}
}

void JuButton::SetAnimationDuration(int ms)
{
	background_animation->setDuration(ms);
	border_animation->setDuration(ms);
	text_animation->setDuration(ms);
}

void JuButton::SetText(QString text)
{
	label->setText(text);
}

void JuButton::SetFont(QFont font)
{
	label->setFont(font);
}

void JuButton::SetPadding(int padding)
{
	layout->setContentsMargins(padding, padding, padding, padding);
}

void JuButton::SetPadding(int left, int top, int right, int bottom)
{
	layout->setContentsMargins(left, top, right, bottom);
}

void JuButton::enterEvent(QEnterEvent* event)
{
	current_state |= 1;
	ChangeColorAnime();
	QWidget::enterEvent(event);
}

void JuButton::leaveEvent(QEvent* event)
{
	current_state &= 0b10;
	ChangeColorAnime();
	QWidget::leaveEvent(event);
}

void JuButton::mousePressEvent(QMouseEvent* event)
{
	emit SignalButtonPress();
	current_state |= 0b10;
	ChangeColorAnime();
	QWidget::mousePressEvent(event);
}

void JuButton::mouseReleaseEvent(QMouseEvent* event)
{
	emit SignalButtonRelease();
	current_state &= 0b01;
	ChangeColorAnime();
	QWidget::mouseReleaseEvent(event);
}

QColor JuButton::GetBackgroundColor()
{
	return background_color;
}

void JuButton::SetBackgroundColor(QColor color)
{
	this->background_color = color;
	this->setStyleSheet(QString("QWidget { background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
		.arg(background_color.red()).arg(background_color.green()).arg(background_color.blue())
		.arg(border_color.red()).arg(border_color.green()).arg(border_color.blue())
		.arg(radius));
}

QColor JuButton::GetBorderColor()
{
	return border_color;
}

void JuButton::SetBorderColor(QColor color)
{
	this->border_color = color;
	this->setStyleSheet(QString("QWidget { background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
		.arg(background_color.red()).arg(background_color.green()).arg(background_color.blue())
		.arg(border_color.red()).arg(border_color.green()).arg(border_color.blue())
		.arg(radius));
}

QColor JuButton::GetTextColor()
{
	return text_color;
}

void JuButton::SetTextColor(QColor color)
{
	this->text_color = color;
	label->setStyleSheet(QString("QLabel { color: %1}").arg(text_color.name()));
}

void JuButton::ChangeColorAnime()
{
	ju_button::ColorSet target_color;
	switch (current_state)
	{
		case 0:
		{
			target_color = base_color;
			break;
		}
		case 1:
		{
			target_color = hover_color;
			break;
		}
		case 2:
		case 3:
		{
			target_color = press_color;
			break;
		}
	}

	background_animation->setStartValue(background_color);
	background_animation->setEndValue(target_color.button_background_color);
	border_animation->setStartValue(border_color);
	border_animation->setEndValue(target_color.button_border_color);
	text_animation->setStartValue(text_color);
	text_animation->setEndValue(target_color.text_color);

	background_animation->start();
	border_animation->start();
	text_animation->start();
}