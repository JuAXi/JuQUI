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

#pragma once
#include <QWidget>

class QLabel;
class QGridLayout;
class QSpacerItem;
class QPropertyAnimation;

namespace ju_button
{
	struct ColorSet
	{
		QColor button_background_color;
		QColor button_border_color;
		QColor text_color;
	};

	static ColorSet DefaultBaseColor()
	{
		return ColorSet{ QColor(248, 250, 252), QColor(226, 230, 237), QColor(51, 51, 51) };
	}

	static ColorSet DefaultHoverColor()
	{
		return ColorSet{ QColor(255, 255, 255), QColor(59, 130, 246), QColor(59, 130, 246) };
	}

	static ColorSet DefaultPressColor()
	{
		return ColorSet{ QColor(239, 246, 255), QColor(29, 78, 216), QColor(29, 78, 216) };
	}
}

class JuButton : public QWidget
{
	Q_OBJECT;

	Q_PROPERTY(QColor background_color READ GetBackgroundColor WRITE SetBackgroundColor);
	Q_PROPERTY(QColor border_color READ GetBorderColor WRITE SetBorderColor);
	Q_PROPERTY(QColor text_color READ GetTextColor WRITE SetTextColor);

public:
	QLabel *label;
	QGridLayout*layout;
	int radius = 4;

	JuButton(QString text,
			 QWidget *parent = nullptr,
			 ju_button::ColorSet base_color = ju_button::DefaultBaseColor(),
			 ju_button::ColorSet hover_color = ju_button::DefaultHoverColor(),
			 ju_button::ColorSet press_color = ju_button::DefaultPressColor(),
			 int animation_ms = 200);
	~JuButton();

	void SetBaseColor(ju_button::ColorSet colors);
	void SetHoverColor(ju_button::ColorSet colors);
	void SetPressColor(ju_button::ColorSet colors);

	void SetAnimationDuration(int ms);

	void SetText(QString text);
	void SetFont(QFont font);
	void SetPadding(int padding);
	void SetPadding(int top, int right, int bottom, int left);

signals:
	void SignalButtonPress();
	void SignalButtonRelease();
	void SignalButtonClicked();

protected:
	void enterEvent(QEnterEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:

	std::array<QSpacerItem*, 4> spacers;

	ju_button::ColorSet base_color;
	ju_button::ColorSet hover_color;
	ju_button::ColorSet press_color;

	QColor background_color, border_color, text_color;

	QColor GetBackgroundColor();
	void SetBackgroundColor(QColor color);

	QColor GetBorderColor();
	void SetBorderColor(QColor color);

	QColor GetTextColor();
	void SetTextColor(QColor color);

	// 0: is enter			1: is mouse_down
	int current_state = 0;
	QPropertyAnimation *background_animation, *border_animation, *text_animation;

	void ChangeColorAnime();
};
