/*
 * Copyright 2026 JuAXi
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

#include "JuSwitch.h"

#include <QPainter>
#include <QPainterPath>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStyleHints>
#include <QPropertyAnimation>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

/*		main widget		*/
JuSwitch::JuSwitch(QWidget* parent, ju_switch::ParamSet param, ju_switch::ColorSet color_set, ju_switch::StyleSet style_set) : QWidget(parent)
{
	/*		create sub widget		*/
	button_ptr = new JuSwitchButton(this);
	slider_ptr = new JuSwitchSlider(this, button_ptr);
	button_ptr->raise();

	/*		create animations		*/
	// button move
	_button_move_animation_ptr = new QPropertyAnimation(button_ptr, "pos", button_ptr);
	_button_move_animation_ptr->setEasingCurve(QEasingCurve::OutCubic);
	connect(_button_move_animation_ptr, &QPropertyAnimation::finished, [this]() {this->_animation_running = false; });
	// button color
	_button_color_animation_ptr = new QPropertyAnimation(button_ptr, "my_color", button_ptr);
	_button_color_animation_ptr->setEasingCurve(QEasingCurve::Linear);
	// button border color
	_button_border_animation_ptr = new QPropertyAnimation(button_ptr, "my_border_color", button_ptr);
	_button_border_animation_ptr->setEasingCurve(QEasingCurve::Linear);

	this->_current_state = param.default_state;
	this->StyleSet(style_set);
	this->ColorSet(color_set);
	this->ParamSet(param);
	this->button_ptr->icon_state = param.default_state;

	this->setCursor(Qt::PointingHandCursor);
	this->setFocusPolicy(Qt::StrongFocus);

	connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, this, &JuSwitch::ColorSchemeChangedSlot);
}

JuSwitch::~JuSwitch()
{
}

ju_switch::StyleSet JuSwitch::StyleSet()
{
	return this->_style_set;
}

void JuSwitch::StyleSet(ju_switch::StyleSet new_style_set)
{
	this->_style_set = new_style_set;

	QRegularExpression check_unit("^([+-]?\\d+(?:\\.\\d+)?)(px|%)$");

	// check padding_h
	auto match = check_unit.match(new_style_set.padding_h);
	if (match.hasMatch())
	{
		_is_padding_h_px = (match.captured(2) == "px");
		if (_is_padding_h_px)
		{
			_padding_h = match.captured(1).toDouble();
		}
		else
		{
			_padding_h = match.captured(1).toDouble() / 100;
		}
	}

	// check padding_v
	match = check_unit.match(new_style_set.padding_v);
	if (match.hasMatch())
	{
		_is_padding_v_px = (match.captured(2) == "px");
		if (_is_padding_v_px)
		{
			_padding_v = match.captured(1).toDouble();
		}
		else
		{
			_padding_v = match.captured(1).toDouble() / 100;
		}
	}

	// check margin
	match = check_unit.match(new_style_set.margin);
	if (match.hasMatch())
	{
		_is_margin_px = (match.captured(2) == "px");
		if (_is_margin_px)
		{
			_margin = match.captured(1).toDouble();
		}
		else
		{
			_margin = match.captured(1).toDouble() / 100;
		}
	}

	// check slider radius
	match = check_unit.match(new_style_set.slider_radius);
	if (match.hasMatch())
	{
		_is_slider_radius_px = (match.captured(2) == "px");
		if (_is_slider_radius_px)
		{
			_slider_radius = match.captured(1).toDouble();
		}
		else
		{
			_slider_radius = match.captured(1).toDouble() / 100;
		}
		slider_ptr->radius = _slider_radius;
		slider_ptr->update();
	}

	button_ptr->ResetStyle(new_style_set);
}

ju_switch::ParamSet JuSwitch::ParamSet()
{
	return this->_param;
}

void JuSwitch::ParamSet(ju_switch::ParamSet new_param_set)
{
	this->_param = new_param_set;
	_button_move_animation_ptr->setDuration(new_param_set.animation_duration_ms);
	_button_color_animation_ptr->setDuration(new_param_set.animation_duration_ms);
	_button_border_animation_ptr->setDuration(new_param_set.animation_duration_ms);
}

ju_switch::ColorSet JuSwitch::ColorSet()
{
	return this->_color_set;
}

void JuSwitch::ColorSet(ju_switch::ColorSet new_color_set)
{
	this->_color_set = new_color_set;
	ColorSchemeChangedSlot(QGuiApplication::styleHints()->colorScheme());
}

bool JuSwitch::CurrentState()
{
	return this->_current_state;
}

void JuSwitch::resizeEvent(QResizeEvent* event)
{
	auto rect = this->rect();
	const auto required_resized = rect.height() * _style_set.min_width_height_rate > rect.width();
	const auto canvas_height = required_resized ?
		(static_cast<int>(rect.width() / _style_set.min_width_height_rate))
		:
		(rect.height());
	const auto margin = _is_margin_px ? static_cast<int>(_margin) : static_cast<int>(canvas_height * _margin);
	const auto button_size = canvas_height - (margin << 1);

	// resize button
	const auto button_y = (rect.height() - button_size) >> 1;
	button_ptr->setGeometry(
		this->_current_state ? (rect.width() - button_size - margin) : (margin),
		button_y,
		button_size,
		button_size);
	_button_move_animation_ptr->setStartValue(QPoint(margin, button_y));
	_button_move_animation_ptr->setEndValue(QPoint(rect.width() - margin - button_size, button_y));

	// resize slider
	auto width = 0;
	auto height = canvas_height - (margin << 1);
	auto y = margin;
	auto x = 0;
	// calculate 'x' and 'width'
	switch (_style_set.padding_x_base)
	{
		case ju_switch::PaddingBase::WIDGET:
		{
			x = margin;
			if (_is_padding_h_px)
			{
				x += _padding_h;
			}
			else
			{
				x += (_padding_h * width);
			}
			width = rect.width() - x * 2;
			break;
		}
		case ju_switch::PaddingBase::BUTTON:
		{
			x = margin + (button_size >> 1);
			if (_is_padding_h_px)
			{
				x += _padding_h;
			}
			else
			{
				x += (_padding_h * button_size / 2);
			}
			width = rect.width() - x * 2;
			break;
		}
	}
	// calculate 'y'
	if (_is_padding_v_px)
	{
		y += _padding_v;
	}
	else
	{
		y += (_padding_v * height);
	}
	height = canvas_height - y * 2;

	slider_ptr->setGeometry(x, (rect.height() - height) >> 1, width, height);

	if (!_is_slider_radius_px)
	{
		slider_ptr->radius = height * _slider_radius;
	}

	QWidget::resizeEvent(event);
}

void JuSwitch::paintEvent(QPaintEvent* event)
{
	slider_ptr->update();
	button_ptr->update();
}

void JuSwitch::mousePressEvent(QMouseEvent* event)
{
	if (((_param.events & ju_switch::EventListen::MOUSE_LDOWN) && event->button() == Qt::LeftButton)
		|| (_param.events & ju_switch::EventListen::MOUSE_RDOWN) && event->button() == Qt::RightButton)
	{
		ChangeState(!_current_state);
		return;
	}
}

void JuSwitch::mouseReleaseEvent(QMouseEvent* event)
{
	if (((_param.events & ju_switch::EventListen::MOUSE_LUP) && event->button() == Qt::LeftButton)
		|| (_param.events & ju_switch::EventListen::MOUSE_RUP) && event->button() == Qt::RightButton)
	{
		ChangeState(!_current_state);
		return;
	}
}

void JuSwitch::keyPressEvent(QKeyEvent* event)
{
	if (((_param.events & ju_switch::EventListen::KEY_ENTER_DOWN) && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
		|| (_param.events & ju_switch::EventListen::KEY_SPACE_DOWN) && event->key() == Qt::Key_Space)
	{
		ChangeState(!_current_state);
		return;
	}
}

void JuSwitch::keyReleaseEvent(QKeyEvent* event)
{
	if (((_param.events & ju_switch::EventListen::KEY_ENTER_UP) && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
		|| (_param.events & ju_switch::EventListen::KEY_SPACE_UP) && event->key() == Qt::Key_Space)
	{
		ChangeState(!_current_state);
		return;
	}
}

void JuSwitch::ColorSchemeChangedSlot(Qt::ColorScheme colorScheme)
{
	if (colorScheme == Qt::ColorScheme::Light)
	{
		/*		slider color		*/
		slider_ptr->color_on = _color_set.slider_color.color_light_on;
		slider_ptr->color_off = _color_set.slider_color.color_light_off;
		slider_ptr->border_on = _color_set.slider_color.border_light_on;
		slider_ptr->border_off = _color_set.slider_color.border_light_off;

		/*		button color		*/
		if (_current_state)
		{
			button_ptr->SetColor(_color_set.button_color.color_light_on);
			button_ptr->SetBorderColor(_color_set.button_color.border_light_on);

		}
		else
		{
			button_ptr->SetColor(_color_set.button_color.color_light_off);
			button_ptr->SetBorderColor(_color_set.button_color.border_light_off);
		}

		_button_color_animation_ptr->setStartValue(_color_set.button_color.color_light_off);
		_button_color_animation_ptr->setEndValue(_color_set.button_color.color_light_on);

		_button_border_animation_ptr->setStartValue(_color_set.button_color.border_light_off);
		_button_border_animation_ptr->setEndValue(_color_set.button_color.border_light_on);
	}
	else
	{
		/*		slider color		*/
		slider_ptr->color_on = _color_set.slider_color.color_dark_on;
		slider_ptr->color_off = _color_set.slider_color.color_dark_off;
		slider_ptr->border_on = _color_set.slider_color.border_dark_on;
		slider_ptr->border_off = _color_set.slider_color.border_dark_off;

		/*		button color		*/
		if (_current_state)
		{
			button_ptr->SetColor(_color_set.button_color.color_dark_on);
			button_ptr->SetBorderColor(_color_set.button_color.border_dark_on);
		}
		else
		{
			button_ptr->SetColor(_color_set.button_color.color_dark_off);
			button_ptr->SetBorderColor(_color_set.button_color.border_dark_off);
		}

		_button_color_animation_ptr->setStartValue(_color_set.button_color.color_dark_off);
		_button_color_animation_ptr->setEndValue(_color_set.button_color.color_dark_on);

		_button_border_animation_ptr->setStartValue(_color_set.button_color.border_dark_off);
		_button_border_animation_ptr->setEndValue(_color_set.button_color.border_dark_on);
	}

	slider_ptr->update();
	button_ptr->update();
}

void JuSwitch::ChangeState(bool new_state, bool emit_signal)
{
	if (_animation_running)
	{
		return;
	}

	_animation_running = true;
	if (new_state == _current_state)
	{
		return;
	}

	this->_current_state = new_state;

	if (this->_current_state)
	{
		// false -> true
		_button_move_animation_ptr->setDirection(QAbstractAnimation::Forward);
		_button_color_animation_ptr->setDirection(QAbstractAnimation::Forward);
		_button_border_animation_ptr->setDirection(QAbstractAnimation::Forward);
	}
	else
	{
		// true -> false
		_button_move_animation_ptr->setDirection(QAbstractAnimation::Backward);
		_button_color_animation_ptr->setDirection(QAbstractAnimation::Backward);
		_button_border_animation_ptr->setDirection(QAbstractAnimation::Backward);
	}

	_button_move_animation_ptr->start();
	_button_color_animation_ptr->start();
	_button_border_animation_ptr->start();

	if (emit_signal)
	{
		emit SignalStateChange(this->_current_state);
	}

	button_ptr->icon_state = this->_current_state;
}


/*		slider widget		*/
JuSwitchSlider::JuSwitchSlider(QWidget* parent, JuSwitchButton* button_ptr) : QWidget(parent), _button_ptr(button_ptr)
{
}

JuSwitchSlider::~JuSwitchSlider()
{
}

void JuSwitchSlider::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QPen pen_false{ border_off };
	QBrush brush_false{ color_off };
	QPen pen_true{ border_on };
	QBrush brush_true{ color_on };
	auto rect = this->rect();

	QPainterPath mask;
	mask.addRoundedRect(rect, radius, radius);
	painter.setClipPath(mask);

	auto width = _button_ptr->x() + (_button_ptr->width() >> 1);

	// draw true
	painter.setPen(pen_true);
	painter.setBrush(brush_true);
	painter.drawRect(0, 0, width, rect.height());

	// draw false
	painter.setPen(pen_false);
	painter.setBrush(brush_false);
	painter.drawRect(width, 0, rect.width() - width, rect.height());
}

/*		button widget		*/
JuSwitchButton::JuSwitchButton(QWidget* parent) : QWidget(parent)
{
}

JuSwitchButton::~JuSwitchButton()
{
}

void JuSwitchButton::ResetStyle(ju_switch::StyleSet style_set)
{
	this->_icon_when_false = style_set.icon_when_false;
	this->_icon_when_true = style_set.icon_when_true;

	QRegularExpression check_unit("^(\\d+(?:\\.\\d+)?)(px|%)$");

	// check button radius
	auto match = check_unit.match(style_set.button_radius);
	if (match.hasMatch())
	{
		_is_button_radius_px = (match.captured(2) == "px");
		if (_is_button_radius_px)
		{
			_radius = match.captured(1).toDouble();
		}
		else
		{
			_radius = match.captured(1).toDouble() / 100;
		}
	}
}

QColor JuSwitchButton::GetColor()
{
	return this->_color;
}

void JuSwitchButton::SetColor(QColor color)
{
	this->_color = color;
}

QColor JuSwitchButton::GetBorderColor()
{
	return this->_border_color;
}

void JuSwitchButton::SetBorderColor(QColor color)
{
	this->_border_color = color;
}

void JuSwitchButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.setPen(QPen(_border_color));
	painter.setBrush(QBrush(_color));

	const auto rect = this->rect();

	if (_is_button_radius_px)
	{
		painter.drawRoundedRect(rect, _radius, _radius);
	}
	else
	{
		auto radius = rect.width() * _radius;
		painter.drawRoundedRect(rect, radius, radius);
	}

	if (icon_state)
	{
		if (!_icon_when_true.isNull())
		{
			painter.drawPixmap(rect, _icon_when_true);
		}
	}
	else
	{
		if (!_icon_when_false.isNull())
		{
			painter.drawPixmap(rect, _icon_when_false);
		}
	}
}