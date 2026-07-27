#pragma once

#include <QWidget>

#include <type_traits>

class QPropertyAnimation;
class QMouseEvent;
class QKeyEvent;

namespace ju_switch
{
	enum class EventListen
	{
		MOUSE_LDOWN = 1,
		MOUSE_LUP = 1 << 1,
		MOUSE_RDOWN = 1 << 2,
		MOUSE_RUP = 1 << 3,
		KEY_ENTER_DOWN = 1 << 4,
		KEY_ENTER_UP = 1 << 5,
		KEY_SPACE_DOWN = 1 << 6,
		KEY_SPACE_UP = 1 << 7
	};
	using UTT = std::underlying_type_t<EventListen>;
	constexpr EventListen operator|(EventListen a, EventListen b) noexcept
	{
		return static_cast<EventListen>(UTT(a) | UTT(b));
	}
	constexpr bool operator&(EventListen a, EventListen b) noexcept
	{
		return (static_cast<UTT>(a) & static_cast<UTT>(b)) != 0;
	}
	
	enum class PaddingBase
	{
		WIDGET,
		BUTTON,
	};

	struct BaseColorSet
	{
		/*		background-color		*/
		// color when state is "true"
		QColor color_light_on;
		QColor color_dark_on;
		// color when state is "false"
		QColor color_light_off;
		QColor color_dark_off;

		/*		border-color		*/
		// color when state is "true"
		QColor border_light_on;
		QColor border_dark_on;
		// color when state is "false"
		QColor border_light_off;
		QColor border_dark_off;

		static BaseColorSet ButtonDefaultColor()
		{
			return
			{
				.color_light_on = QColor{ "#fff" },
				.color_dark_on = QColor{ "#fff" },
				.color_light_off = QColor{ "#fff" },
				.color_dark_off = QColor{ "#fff" },
				.border_light_on = QColor{ "#a6a6a6" },
				.border_dark_on = QColor{ "#a6a6a6" },
				.border_light_off = QColor{ "#a6a6a6" },
				.border_dark_off = QColor{ "#a6a6a6" },
			};
		}

		static BaseColorSet SliderDefaultColor()
		{
			return
			{
				.color_light_on = QColor{ "#9cd6ff" },
				.color_dark_on = QColor{ "#9cd6ff" },
				.color_light_off = QColor{ "#a6a6a6" },
				.color_dark_off = QColor{ "#a6a6a6" },
				.border_light_on = QColor{ "#9cd6ff" },
				.border_dark_on = QColor{ "#9cd6ff" },
				.border_light_off = QColor{ "#a6a6a6" },
				.border_dark_off = QColor{ "#a6a6a6" },
			};
		}
	};

	struct ColorSet
	{
		BaseColorSet button_color = BaseColorSet::ButtonDefaultColor();
		BaseColorSet slider_color = BaseColorSet::SliderDefaultColor();
	};

	struct StyleSet
	{
		double min_width_height_rate = 2.0;					// width / height, don't lesser than 1.0 !!!

		QString slider_radius = "50%";						// slider radius, use "%" or "px"
		QString button_radius = "50%";						// button radius, use "%" or "px"
		QString margin = "0%";								// distance between button and full widget, use "%" or "px"

		PaddingBase padding_x_base = PaddingBase::WIDGET;	// padding base on which QWidget
		QString padding_h = "0%";							// use "%" or "px"
		QString padding_v = "0%";							// use "%" or "px"

		QPixmap icon_when_true;								// button icon when state is "true"
		QPixmap icon_when_false;							// button icon when state is "false"
	};

	struct ParamSet
	{
		bool default_state = false;					// state when init
		int animation_duration_ms = 150;			// animation duration (milliseconds)

		// (keyboard / mouse) event listener
		EventListen events = EventListen::MOUSE_LUP | EventListen::KEY_SPACE_UP;
	};
}


/*		button		*/
class JuSwitchButton : public QWidget
{
	Q_OBJECT
public:
	JuSwitchButton(QWidget* parent);
	~JuSwitchButton();

	void ResetStyle(ju_switch::StyleSet style_set);

	bool icon_state = false;

	QColor GetColor();
	void SetColor(QColor color);
	Q_PROPERTY(QColor my_color READ GetColor WRITE SetColor)

		QColor GetBorderColor();
	void SetBorderColor(QColor color);
	Q_PROPERTY(QColor my_border_color READ GetBorderColor WRITE SetBorderColor)
protected:
	void paintEvent(QPaintEvent* event) override;

private:
	ju_switch::StyleSet _style_set;

	bool _is_button_radius_px = false;
	double _radius = 0.0;

	QPixmap _icon_when_true;
	QPixmap _icon_when_false;

	QColor _border_color, _color;
};



/*		slider		*/
class JuSwitchSlider : public QWidget
{
	Q_OBJECT

public:
	JuSwitchSlider(QWidget* parent, JuSwitchButton* button_ptr);
	~JuSwitchSlider();

	int radius = 0;

	QColor border_on, border_off, color_on, color_off;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	JuSwitchButton* _button_ptr;

};



/*		main		*/
class JuSwitch : public QWidget
{
	Q_OBJECT

public:
	JuSwitch(QWidget* parent = nullptr, ju_switch::ParamSet param = {}, ju_switch::ColorSet color_set = {}, ju_switch::StyleSet style_set = {});
	~JuSwitch();

	JuSwitchSlider* slider_ptr;
	JuSwitchButton* button_ptr;

	ju_switch::StyleSet StyleSet();
	void StyleSet(ju_switch::StyleSet new_style_set);

	ju_switch::ParamSet ParamSet();
	void ParamSet(ju_switch::ParamSet new_param_set);

	ju_switch::ColorSet ColorSet();
	void ColorSet(ju_switch::ColorSet new_color_set);

	bool CurrentState();

public slots:
	void ChangeState(bool new_state);

signals:
	void SignalStateChange(bool current_state);

protected:
	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	QPropertyAnimation* _button_move_animation_ptr, * _button_color_animation_ptr, * _button_border_animation_ptr;

	bool _current_state = false;
	bool _animation_running = false;

	bool _is_padding_h_px = false;
	bool _is_padding_v_px = false;
	bool _is_margin_px = true;
	bool _is_slider_radius_px = false;

	double _padding_h = 0.0;
	double _padding_v = 0.0;
	double _margin = 0.0;
	double _slider_radius = 0.0;

	ju_switch::ParamSet _param;
	ju_switch::ColorSet _color_set;
	ju_switch::StyleSet _style_set;

private slots:
	void ColorSchemeChangedSlot(Qt::ColorScheme colorScheme);
};