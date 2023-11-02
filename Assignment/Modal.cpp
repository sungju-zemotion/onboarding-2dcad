#include "Modal.h"	

PropertiesModal::PropertiesModal(Shape* shape, QWidget* parent)
	: QDialog(parent), mShape(shape), mColorModal(new QColorDialog(this))
	, mColorButton(new QPushButton(this)), mCurrentColor(shape->GetColor())
{
	QVBoxLayout* mainLayout = new QVBoxLayout;

	// name
	QHBoxLayout* nameLayout = new QHBoxLayout;
	QLabel* nameLabel = new QLabel("name");
	QLineEdit* nameEdit = new QLineEdit(QVariant(shape->GetId()).toString());
	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(nameEdit);
	mainLayout->addLayout(nameLayout);

	// name
	QHBoxLayout* colorLayout = new QHBoxLayout;
	QLabel* colorLabel = new QLabel("color");
	mColorButton->setText(QVariant(shape->GetColor()).toString());
	colorLayout->addWidget(colorLabel);
	colorLayout->addWidget(mColorButton);
	mainLayout->addLayout(colorLayout);

	// buttons
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	QPushButton* okButton = new QPushButton("ok");
	QPushButton* cancelButton = new QPushButton("cancel");
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);
	mainLayout->addLayout(buttonLayout);

	// set layout to the modal
	setLayout(mainLayout);

	// connects
	connect(mColorButton, &QPushButton::clicked, [this]() { mColorModal->open(); });
	connect(mColorModal, &QColorDialog::colorSelected, [this]() {
		mColorButton->setText(QVariant(mColorModal->currentColor()).toString());
		mCurrentColor = mColorModal->currentColor();
		});
	connect(okButton, &QPushButton::clicked, [this]() { ApplyChanges(); close(); });
	connect(cancelButton, &QPushButton::clicked, [this]() { close(); });

	// run the modal
	open();
}

void PropertiesModal::ApplyChanges()
{
	mShape->SetColor(mCurrentColor);
	// name: TBA

	accept();
}

