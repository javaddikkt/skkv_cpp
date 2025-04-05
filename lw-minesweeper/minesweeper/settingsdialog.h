#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

class QSpinBox;

class SettingsDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit SettingsDialog(QWidget *parent = nullptr);
	int mineCount() const;
	int rowsCount() const;
	int colsCount() const;

  private:
	QSpinBox *rows_spin_box_;
	QSpinBox *columns_spin_box_;
	QSpinBox *mines_spin_box_;
};
