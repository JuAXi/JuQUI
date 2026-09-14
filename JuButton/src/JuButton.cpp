/*
 * Copyright 2026 JuAXi
 * https://github.com/JuAXi/JuQUI
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://apache.org
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "JuButton.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPropertyAnimation>
#include <QMouseEvent>

JuButton::JuButton(QString text,
	QWidget* parent,
	ju_button::ColorSet base_color,
	ju_button::ColorSet hover_color,
	ju_button::ColorSet press_color,
	int animation_ms) : QWidget(parent)
{
	label = new QLabel(text, this);
	label->setAlignment(Qt::AlignCenter);
	layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setObjectName("JuButton");

	for (auto& spacer_ptr : spacers)
	{
		spacer_ptr = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
	}

	layout->addItem(spacers[0], 0, 1);
	layout->addItem(spacers[1], 1, 2);
	layout->addItem(spacers[2], 2, 1);
	layout->addItem(spacers[3], 1, 0);
	layout->addWidget(label, 1, 1);

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

		this->setStyleSheet(QString("#JuButton {background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
			.arg(background_color.red())
			.arg(background_color.green())
			.arg(background_color.blue())
			.arg(border_color.red())
			.arg(border_color.green())
			.arg(border_color.blue())
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

		this->setStyleSheet(QString("#JuButton {background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
			.arg(background_color.red())
			.arg(background_color.green())
			.arg(background_color.blue())
			.arg(border_color.red())
			.arg(border_color.green())
			.arg(border_color.blue())
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

		this->setStyleSheet(QString("#JuButton {background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
			.arg(background_color.red())
			.arg(background_color.green())
			.arg(background_color.blue())
			.arg(border_color.red())
			.arg(border_color.green())
			.arg(border_color.blue())
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
	SetPadding(padding, padding, padding, padding);
}

void JuButton::SetPadding(int left, int top, int right, int bottom)
{
	spacers[0]->changeSize(0, top);
	spacers[1]->changeSize(right, 0);
	spacers[2]->changeSize(0, bottom);
	spacers[3]->changeSize(left, 0);

	this->adjustSize();
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
	if (this->rect().contains(event->position().toPoint()))
	{
		emit SignalButtonClicked();
	}
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
	this->setStyleSheet(QString("#JuButton {background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
		.arg(background_color.red())
		.arg(background_color.green())
		.arg(background_color.blue())
		.arg(border_color.red())
		.arg(border_color.green())
		.arg(border_color.blue())
		.arg(radius));
}

QColor JuButton::GetBorderColor()
{
	return border_color;
}

void JuButton::SetBorderColor(QColor color)
{
	this->border_color = color;
	this->setStyleSheet(QString("#JuButton {background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: %7px }")
		.arg(background_color.red())
		.arg(background_color.green())
		.arg(background_color.blue())
		.arg(border_color.red())
		.arg(border_color.green())
		.arg(border_color.blue())
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