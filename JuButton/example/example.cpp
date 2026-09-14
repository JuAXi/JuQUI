#include "example.h"

#include <JuButton.h>

#include <QTextEdit>

example::example(QWidget* parent) : QMainWindow(parent), ui(new Ui::exampleClass())
{
	ui->setupUi(this);

	auto button = new JuButton("My button", this);
	button->move(50, 50);

	auto text_edit = new QTextEdit(this);
	text_edit->setGeometry(180, 50, 300, 200);

	connect(button, &JuButton::SignalButtonPress, this, [this, text_edit]()
		{
			text_edit->append("button press!");
		});

	connect(button, &JuButton::SignalButtonRelease, this, [this, text_edit]()
		{
			text_edit->append("button release!");
		});

	connect(button, &JuButton::SignalButtonClicked, this, [this, text_edit]()
		{
			text_edit->append("button clicked!");
		});
}

example::~example()
{
	delete ui;
}

