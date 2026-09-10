#include "example.h"

#include <JuButton.h>

#include <QLabel>

example::example(QWidget* parent) : QMainWindow(parent), ui(new Ui::exampleClass())
{
	ui->setupUi(this);

	auto button = new JuButton("My button", this);
	button->move(50, 50);

	auto label = new QLabel("not press", this);
	label->move(180, 50);

	connect(button, &JuButton::SignalButtonPress, this, [this, label]()
		{
			label->setText("button press!");
		});

	connect(button, &JuButton::SignalButtonRelease, this, [this, label]()
		{
			label->setText("button release!");
		});
}

example::~example()
{
	delete ui;
}

