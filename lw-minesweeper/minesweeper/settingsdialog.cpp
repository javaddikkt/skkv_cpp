#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	QLabel *rowsLabel = new QLabel(tr("Число строк:"), this);
	rows_spin_box_ = new QSpinBox(this);
	rows_spin_box_->setRange(5, 30);
	rows_spin_box_->setValue(10);

	QLabel *columnsLabel = new QLabel(tr("Число столбцов:"), this);
	columns_spin_box_ = new QSpinBox(this);
	columns_spin_box_->setRange(5, 30);
	columns_spin_box_->setValue(10);

	QLabel *minesLabel = new QLabel(tr("Число мин:"), this);
	mines_spin_box_ = new QSpinBox(this);
	mines_spin_box_->setMinimum(1);
	mines_spin_box_->setValue(10);

	layout->addWidget(rowsLabel);
	layout->addWidget(rows_spin_box_);
	layout->addWidget(columnsLabel);
	layout->addWidget(columns_spin_box_);
	layout->addWidget(minesLabel);
	layout->addWidget(mines_spin_box_);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	layout->addWidget(buttonBox);
}

int SettingsDialog::mineCount() const
{
	return mines_spin_box_->value();
}

int SettingsDialog::rowsCount() const
{
	return rows_spin_box_->value();
}

int SettingsDialog::colsCount() const
{
	return columns_spin_box_->value();
}
