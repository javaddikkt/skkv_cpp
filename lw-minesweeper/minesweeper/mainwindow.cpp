#include "mainwindow.h"

#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	createMenus();
	toolbar_ = addToolBar("Main Toolbar");
	this->resize(500, 500);
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus()
{
	game_menu_ = menuBar()->addMenu(tr("Игра"));
	game_menu_->addAction(tr("Новая игра"), this, &MainWindow::createOldBoard);
	game_menu_->addAction(tr("Параметры"), this, &MainWindow::showSettingsDialog);
	game_menu_->addAction(tr("Выход"), this, &MainWindow::close);
}

void MainWindow::createToolbar()
{
	toolbar_->clear();
	QAction *action = new QAction(QIcon(":/images/left-right.png"), "", this);
	connect(action, &QAction::triggered, game_board_, &GameBoard::changeHands);
	toolbar_->addAction(action);

	QAction *langAction = new QAction(QIcon(":/images/en-rus.png"), "", this);
	connect(langAction, &QAction::triggered, this, &MainWindow::changeLanguage);
	toolbar_->addAction(langAction);

	if (debug_)
	{
		QCheckBox *checkBox = new QCheckBox(this);
		checkBox->setStyleSheet(R"(
                                QCheckBox::indicator {
                                    width: 153px;
                                    height: 51px;
                                }
                                QCheckBox::indicator:checked {
                                    image: url(:/images/dbg_check.png);
                                }
                                QCheckBox::indicator:unchecked {
                                    image: url(:/images/dbg_unch.png);
                                })");
		toolbar_->addWidget(checkBox);
		connect(checkBox, &QCheckBox::stateChanged, game_board_, &GameBoard::watch);
	}

	QWidget *spacer = new QWidget(this);
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	toolbar_->addWidget(spacer);

	mine_counter_label_ = new QLabel(this);
	mine_counter_label_->setFixedSize(80, 41);
	mine_counter_label_->setAlignment(Qt::AlignRight);
	mine_counter_label_->setStyleSheet(R"(
                                    QLabel {
                                        color: #b53624;
                                        background-color: #d8d8d8;
                                        border: 2px solid #000000;
                                        font-size: 30px;
                                        font-weight: bold;
                                    }
                                    )");
	toolbar_->addWidget(mine_counter_label_);
	updateMineCounter();
}

void MainWindow::createBoard(int r, int c, int m)
{
	game_board_ = new GameBoard(this, r, c, m);
	game_board_->initializeBoard();
	newGame();
}

void MainWindow::createOldBoard()
{
	createBoard(game_board_->getRowsNumber(), game_board_->getColsNumber(), game_board_->getMinesNumber());
}

void MainWindow::newGame()
{
	connect(game_board_, &GameBoard::mineFlagChanged, this, &MainWindow::updateMineCounter);
	connect(game_board_, &GameBoard::restartGame, this, &MainWindow::createOldBoard);
	setCentralWidget(game_board_);
	createToolbar();
	updateMineCounter();
}

void MainWindow::showSettingsDialog()
{
	SettingsDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted)
	{
		if (dialog.mineCount() >= dialog.rowsCount() * dialog.colsCount())
		{
			QMessageBox mBox(this);
			mBox.setWindowTitle(tr("Ошибка"));
            mBox.setText(tr("Количество мин должно быть меньше размера поля"));
			mBox.setStandardButtons(QMessageBox::Ok);
			mBox.exec();
			return;
		}
		createBoard(dialog.rowsCount(), dialog.colsCount(), dialog.mineCount());
	}
}

void MainWindow::updateMineCounter()
{
	int remainingMines = game_board_->getMinesNumber() - game_board_->getFlagsNumber();
	mine_counter_label_->setText(QString::number(remainingMines));
}

void MainWindow::saveGame(const QString &file)
{
	QSettings settings(file, QSettings::IniFormat);
	settings.beginGroup(("Main"));
	settings.setValue("rus_", rus_);
	settings.endGroup();

	game_board_->saveBoard(file);
}

void MainWindow::restoreGame(const QString &file, bool dbg)
{
	QSettings settings(file, QSettings::IniFormat);
	settings.beginGroup(("Main"));
	rus_ = settings.value("rus_", true).toBool();
	settings.endGroup();

	debug_ = dbg;
	retranslateUi();
	game_board_ = new GameBoard(this, file);
	newGame();
}

void MainWindow::changeLanguage()
{
	rus_ = !rus_;
	retranslateUi();
}

void MainWindow::retranslateUi()
{
    QTranslator *translator = new QTranslator(this);
	if (rus_)
	{
		translator->load(":/translations/minesweeper_ru.qm");
	}
	else
	{
		translator->load(":/translations/minesweeper_en.qm");
	}
	QApplication::installTranslator(translator);
	game_menu_->setTitle(tr("Игра"));
	game_menu_->actions().at(0)->setText(tr("Новая игра"));
	game_menu_->actions().at(1)->setText(tr("Параметры"));
	game_menu_->actions().at(2)->setText(tr("Выход"));
}
