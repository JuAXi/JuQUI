#include "example.h"

#include <QLabel>
#include <QApplication>
#include <QStyleHints>

#include <JuSwitch.h>

example::example(QWidget* parent) : QMainWindow(parent), ui(new Ui::exampleClass())
{
	ui->setupUi(this);

	const auto default_layout_ptr = qobject_cast<QGridLayout*>(ui->centralWidget->layout());



	/*		Default Style		*/
	auto label0 = new QLabel("Default Style: ");
	default_layout_ptr->addWidget(label0, 0, 0);
	auto default_switch = new JuSwitch();
	default_layout_ptr->addWidget(default_switch, 0, 1);



	/*		Change Color Example		*/
	auto label1 = new QLabel("Change Color Example: ");
	default_layout_ptr->addWidget(label1, 0, 2);
	ju_switch::ColorSet color_set
	{
		.button_color =
		{
			.color_light_on = QColor{"#a1e7ff"},
			.color_dark_on = QColor{"#a1e7ff"},
			.color_light_off = QColor{"#f2f542"},
			.color_dark_off = QColor{"#f2f542"},
			.border_light_on = QColor{"#3474eb"},
			.border_dark_on = QColor{"#3474eb"},
			.border_light_off = QColor{"#b8bdff"},
			.border_dark_off = QColor{"#b8bdff"},
		},
		.slider_color =
		{
			.color_light_on = QColor{"#34eb68"},
			.color_dark_on = QColor{"#34eb68"},
			.color_light_off = QColor{"#ffb8e1"},
			.color_dark_off = QColor{"#ffb8e1"},
			.border_light_on = QColor{"#ffffb8"},
			.border_dark_on = QColor{"#ffffb8"},
			.border_light_off = QColor{"#b8bdff"},
			.border_dark_off = QColor{"#b8bdff"},
		}
	};
	auto change_color_layout = new QVBoxLayout();
	default_layout_ptr->addLayout(change_color_layout, 0, 3);

	// Method 1
	auto change_color_switch0 = new JuSwitch(nullptr, ju_switch::ParamSet{}, color_set);
	change_color_layout->addWidget(change_color_switch0);

	// Method 2
	auto change_color_switch1 = new JuSwitch();
	change_color_layout->addWidget(change_color_switch1);
	change_color_switch1->ColorSet(color_set);



	/*		Change Style Example		*/
	auto label2 = new QLabel("Change Style Example: ");
	default_layout_ptr->addWidget(label2, 1, 0);
	ju_switch::StyleSet style_set
	{
		.min_width_height_rate = 1.2,			// resize the window and see what different with default style (width and height)
		.slider_radius = "0px",					// only accept 'px' and '%'
		.button_radius = "0%",
		.margin = "10%",
		.icon_when_true = QPixmap(":/example/img/true.png"),
		.icon_when_false = QPixmap(":/example/img/false.png"),
	};

	auto change_style_layout = new QVBoxLayout();
	default_layout_ptr->addLayout(change_style_layout, 1, 1);

	// Method 1
	auto change_style_switch0 = new JuSwitch(nullptr, ju_switch::ParamSet{}, ju_switch::ColorSet{}, style_set);
	change_style_layout->addWidget(change_style_switch0);

	// Method 2
	auto change_style_switch1 = new JuSwitch();
	change_style_switch1->StyleSet(style_set);
	change_style_layout->addWidget(change_style_switch1);



	/*		Change Parameter Example		*/
	auto label3 = new QLabel("Change Param Example: ");
	default_layout_ptr->addWidget(label3, 1, 2);
	ju_switch::ParamSet param_set
	{
		.default_state = true,								// 'default_state' only worked in constructor !!!!!

		.animation_duration_ms = 1500,						// ~~~~~ too ~~~~~ slow ~~~~~
															// switch won't be triggered again until its animation run over

		.events = ju_switch::EventListen::MOUSE_RUP			// now this switch can be triggered by 'mouse right button' and 'enter' key when it get focus
				| ju_switch::EventListen::MOUSE_LUP
				| ju_switch::EventListen::KEY_SPACE_DOWN
				| ju_switch::EventListen::KEY_ENTER_DOWN,
	};
	auto change_param_layout = new QVBoxLayout();
	default_layout_ptr->addLayout(change_param_layout, 1, 3);

	// Method 1
	auto change_param_switch0 = new JuSwitch(nullptr, param_set);
	change_param_layout->addWidget(change_param_switch0);

	// Method 2, switch's state won't be update if reset param_set after object has been created
	auto change_param_switch1 = new JuSwitch();
	change_param_switch1->ParamSet(param_set);
	change_param_layout->addWidget(change_param_switch1);



	/*		Mixed Example		*/
	auto label4 = new QLabel("Mixed Example: ");
	default_layout_ptr->addWidget(label4, 2, 0);

	auto mixed_example_switch = new JuSwitch(nullptr,
		ju_switch::ParamSet
		{
			.events = ju_switch::EventListen::MOUSE_LUP | ju_switch::EventListen::KEY_SPACE_DOWN | ju_switch::EventListen::KEY_ENTER_DOWN
		},
		ju_switch::ColorSet
		{
			.button_color =
			{
				.color_light_on = QColor{"#5cbbff"},
				.color_dark_on = QColor{"#5cbbff"},
				.color_light_off = QColor{"#a6a6a6"},
				.color_dark_off = QColor{"#a6a6a6"},
				.border_light_on = QColor{"#5cbbff"},
				.border_dark_on = QColor{"#5cbbff"},
				.border_light_off = QColor{"#a6a6a6"},
				.border_dark_off = QColor{"#a6a6a6"},
			}
		},
		ju_switch::StyleSet
		{
			.margin = "10%",
			.padding_x_base = ju_switch::PaddingBase::BUTTON,
			.padding_h = "-50%",
			.padding_v = "20%",
		});

	default_layout_ptr->addWidget(mixed_example_switch, 2, 1);

	auto label5 = new QLabel("mixed example value is: <font color='red'>false</font>");
	default_layout_ptr->addWidget(label5, 2, 2, 1, 2);

	connect(mixed_example_switch, &JuSwitch::SignalStateChange, [this, label5](bool state)
		{
			label5->setText(QString("mixed example value is: %1").arg(state ? "<font color='green'>true</font>" : "<font color='red'>false</font>"));
		});



	/*		Switch Change Scheme Example		*/
	auto label6 = new QLabel("change color scheme example: ");
	default_layout_ptr->addWidget(label6, 3, 0);
	QPixmap pixmap0, pixmap1;
	pixmap0.loadFromData(QByteArray::fromBase64(QString("iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsIAAA7CARUoSoAAAAGHaVRYdFhNTDpjb20uYWRvYmUueG1wAAAAAAA8P3hwYWNrZXQgYmVnaW49J++7vycgaWQ9J1c1TTBNcENlaGlIenJlU3pOVGN6a2M5ZCc/Pg0KPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyI+PHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj48cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0idXVpZDpmYWY1YmRkNS1iYTNkLTExZGEtYWQzMS1kMzNkNzUxODJmMWIiIHhtbG5zOnRpZmY9Imh0dHA6Ly9ucy5hZG9iZS5jb20vdGlmZi8xLjAvIj48dGlmZjpPcmllbnRhdGlvbj4xPC90aWZmOk9yaWVudGF0aW9uPjwvcmRmOkRlc2NyaXB0aW9uPjwvcmRmOlJERj48L3g6eG1wbWV0YT4NCjw/eHBhY2tldCBlbmQ9J3cnPz4slJgLAAAAt0lEQVRYR+2VXQ7DMAiD010ccbYcLH1CyqywGNJM2pTvkYLtlP6Ucjj8MiLSsBblhYVv898BllckIi0rws5eWEBMRFXfelG8v+7NjJg2GCLSTBDNDcYQCQ945kY0BN08M0bYIFvfAgYqQPT0EdzbtGqqqlev4a1kWEQyYTxDhFrBTqgA7GkyhITZVUQC043GLETEvLAr6H8snwxmn+oUvTlLZmZIrTUt9FiIFbaHeEKcegh3cgIcbsGqaaNEniBVAAAAAElFTkSuQmCC").toUtf8()));
	pixmap1.loadFromData(QByteArray::fromBase64(QString("iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsIAAA7CARUoSoAAAAGHaVRYdFhNTDpjb20uYWRvYmUueG1wAAAAAAA8P3hwYWNrZXQgYmVnaW49J++7vycgaWQ9J1c1TTBNcENlaGlIenJlU3pOVGN6a2M5ZCc/Pg0KPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyI+PHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj48cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0idXVpZDpmYWY1YmRkNS1iYTNkLTExZGEtYWQzMS1kMzNkNzUxODJmMWIiIHhtbG5zOnRpZmY9Imh0dHA6Ly9ucy5hZG9iZS5jb20vdGlmZi8xLjAvIj48dGlmZjpPcmllbnRhdGlvbj4xPC90aWZmOk9yaWVudGF0aW9uPjwvcmRmOkRlc2NyaXB0aW9uPjwvcmRmOlJERj48L3g6eG1wbWV0YT4NCjw/eHBhY2tldCBlbmQ9J3cnPz4slJgLAAAAcElEQVRYR+3SwQ7AIAgDUNj///N2mZdmQsmmzqTviBoqwUxEdnPesD7FiOYHFmZzLPTgz92dfhtZPgEFWB6AXiRcwubtMu4zARs0hU8m0AvGKCfPmlWnUbrcZCGsEIS69IQJYUSQ8JARBcmai4j8wgVmWzACs5KswQAAAABJRU5ErkJggg==").toUtf8()));
	auto button_color = ju_switch::BaseColorSet::ButtonDefaultColor();
	button_color.color_dark_off = QColor{ "#000" };
	button_color.border_dark_off = QColor{ "#000" };
	auto slider_color = ju_switch::BaseColorSet::SliderDefaultColor();
	slider_color.color_dark_off = QColor{ "#c9c9c9" };
	auto temp_switch = new JuSwitch(nullptr,
		ju_switch::ParamSet
		{
			.default_state = true
		},
		ju_switch::ColorSet
		{
			.button_color = button_color,
			.slider_color = slider_color
		},
		ju_switch::StyleSet
		{
			.icon_when_true = pixmap0,
			.icon_when_false = pixmap1
		});
	connect(temp_switch, &JuSwitch::SignalStateChange, [this](bool state)
		{
			if (state)
			{
				QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
			}
			else
			{
				QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Dark);
			}
		});

	default_layout_ptr->addWidget(temp_switch, 3, 1);
}

example::~example()
{
	delete ui;
}

