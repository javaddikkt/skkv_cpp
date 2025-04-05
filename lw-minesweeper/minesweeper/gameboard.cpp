#include "gameboard.h"

GameBoard::GameBoard(QWidget *parent, int r, int c, int m) :
	QWidget(parent), rows_(r), cols_(c), mine_count_(m), flag_count_(0), first_click_made_(false), right_hand_(true),
	blocked_(false)
{
	setLayout();
}

GameBoard::GameBoard(QWidget *parent, const QString &file) : QWidget(parent)
{
	setLayout();
	restoreBoard(file);
}

void GameBoard::setLayout()
{
	layout_ = new QGridLayout(this);
	layout_->setSpacing(0);
}

void GameBoard::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	int size = qMin(this->width() / cols_, this->height() / rows_);
	this->resize(size * cols_, size * rows_);
}

void GameBoard::initializeBoard()
{
	cells_.resize(rows_);
	for (int i = 0; i < rows_; ++i)
	{
		cells_[i].resize(cols_);
		for (int j = 0; j < cols_; ++j)
		{
			cells_[i][j] = new Cell(this, i, j);
			layout_->addWidget(cells_[i][j], i, j);
			connect(cells_[i][j], &Cell::leftClick, this, &GameBoard::handleLeftClick);
			connect(cells_[i][j], &Cell::rightClick, this, &GameBoard::handleRightClick);
			connect(cells_[i][j], &Cell::middleClick, this, &GameBoard::handleMiddleClick);
		}
	}
}

void GameBoard::placeMines(int initialRow, int initialCol)
{
	int placedMines = 0;
	while (placedMines < mine_count_)
	{
		int r = QRandomGenerator::global()->bounded(rows_);
		int c = QRandomGenerator::global()->bounded(cols_);

		if ((r != initialRow || c != initialCol) && !cells_[r][c]->hasMine())
		{
			cells_[r][c]->setMine(true);
			placedMines++;
			int r_st = r > 0 ? r - 1 : r;
			int r_en = r < rows_ - 1 ? r + 1 : r;
			int c_st = c > 0 ? c - 1 : c;
			int c_en = c < cols_ - 1 ? c + 1 : c;
			for (int i = r_st; i <= r_en; ++i)
			{
				for (int j = c_st; j <= c_en; ++j)
				{
					if (!(i == r && j == c) && !(cells_[i][j]->hasMine()))
					{
						cells_[i][j]->setNumber(cells_[i][j]->getNumber() + 1);
					}
				}
			}
		}
	}
}

void GameBoard::checkWin()
{
	for (const auto &row : cells_)
	{
		for (const auto &cell : row)
		{
			if ((cell->hasMine() && !cell->hasFlag()) || (!cell->hasMine() && !cell->isOpened()))
			{
				return;
			}
		}
	}
	endGame(true);
	return;
}

void GameBoard::endGame(bool good)
{
	blocked_ = true;
	for (const auto &row : cells_)
	{
		for (const auto &cell : row)
		{
			cell->open();
		}
	}
	QMessageBox mBox(this);
	mBox.setWindowTitle(tr("Игра завершена"));
	mBox.setText(good ? tr("Победа!") : tr("Вы попали на мину!"));
	mBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);
	mBox.setDefaultButton(QMessageBox::Retry);
	int ret = mBox.exec();
	switch (ret)
	{
	case QMessageBox::Retry:
		emit restartGame();
	case QMessageBox::Close:
		return;
	}
}

bool GameBoard::recursiveOpen(int x, int y)
{
	cells_[x][y]->open();
	if (cells_[x][y]->hasMine() && cells_[x][y]->isOpened())
	{
		cells_[x][y]->showLast();
		return false;
	}
	if (cells_[x][y]->getNumber() == 0 && !cells_[x][y]->hasFlag() && !cells_[x][y]->hasQuestionMark())
	{
		int r_st = x > 0 ? x - 1 : x;
		int r_en = x < rows_ - 1 ? x + 1 : x;
		int c_st = y > 0 ? y - 1 : y;
		int c_en = y < cols_ - 1 ? y + 1 : y;
		for (int i = r_st; i <= r_en; ++i)
		{
			for (int j = c_st; j <= c_en; ++j)
			{
				if (!cells_[i][j]->isOpened())
				{
					recursiveOpen(i, j);
				}
			}
		}
	}
	return true;
}

void GameBoard::handleLeftClick(int x, int y)
{
	if (blocked_)
	{
		return;
	}
	if (!right_hand_)
	{
		right_hand_ = true;
		handleRightClick(x, y);
		right_hand_ = false;
		return;
	}
	if (!first_click_made_)
	{
		placeMines(x, y);
		first_click_made_ = true;
	}
	if (!recursiveOpen(x, y))
	{
		endGame(false);
		return;
	}
	checkWin();
}

void GameBoard::handleRightClick(int x, int y)
{
	if (blocked_)
	{
		return;
	}
	if (!right_hand_)
	{
		right_hand_ = true;
		handleLeftClick(x, y);
		right_hand_ = false;
		return;
	}
	if (!cells_[x][y]->isOpened())
	{
		if (cells_[x][y]->hasFlag())
		{
			cells_[x][y]->setFlag(false);
			cells_[x][y]->setQuestionMark(true);
			flag_count_--;
		}
		else if (cells_[x][y]->hasQuestionMark())
		{
			cells_[x][y]->setQuestionMark(false);
		}
		else
		{
			cells_[x][y]->setFlag(true);
			flag_count_++;
		}
		emit mineFlagChanged();
		checkWin();
	}
}

void GameBoard::handleMiddleClick(int x, int y)
{
	if (blocked_)
	{
		return;
	}
	int r_st = x > 0 ? x - 1 : x;
	int r_en = x < rows_ - 1 ? x + 1 : x;
	int c_st = y > 0 ? y - 1 : y;
	int c_en = y < cols_ - 1 ? y + 1 : y;
	if (cells_[x][y]->isOpened() && first_click_made_)
	{
		int cntFlags = 0;
		for (int i = r_st; i <= r_en; ++i)
		{
			for (int j = c_st; j <= c_en; ++j)
			{
				if (cells_[i][j]->hasFlag())
				{
					cntFlags++;
				}
			}
		}
		if (cntFlags >= cells_[x][y]->getNumber())
		{
			for (int i = r_st; i <= r_en; ++i)
			{
				for (int j = c_st; j <= c_en; ++j)
				{
					if (!recursiveOpen(i, j))
					{
						endGame(false);
						return;
					}
				}
			}
			checkWin();
		}
		else
		{
			highlightArea(r_st, c_st, r_en, c_en);
			QTimer::singleShot(500, this, [this, r_st, c_st, r_en, c_en]() { unhighlightArea(r_st, c_st, r_en, c_en); });
		}
	}
}

void GameBoard::highlightArea(int x1, int y1, int x2, int y2)
{
	for (int i = x1; i <= x2; ++i)
	{
		for (int j = y1; j <= y2; ++j)
		{
			if (!cells_[i][j]->hasFlag() && !cells_[i][j]->hasQuestionMark() && !cells_[i][j]->isOpened())
			{
				cells_[i][j]->showEmpty();
			}
		}
	}
}

void GameBoard::unhighlightArea(int x1, int y1, int x2, int y2)
{
	for (int i = x1; i <= x2; ++i)
	{
		for (int j = y1; j <= y2; ++j)
		{
			if (!cells_[i][j]->hasFlag() && !cells_[i][j]->isOpened())
			{
				cells_[i][j]->closeEmpty();
			}
		}
	}
}

int GameBoard::getRowsNumber()
{
	return rows_;
}

int GameBoard::getColsNumber()
{
	return cols_;
}

int GameBoard::getMinesNumber()
{
	return mine_count_;
}

int GameBoard::getFlagsNumber()
{
	return flag_count_;
}

void GameBoard::changeHands()
{
	right_hand_ = !right_hand_;
}

void GameBoard::watch()
{
	if (mine_count_ - flag_count_ == 0)
	{
		return;
	}
	blocked_ = !blocked_;
	if (blocked_)
	{
		for (const auto &row : cells_)
		{
			for (const auto &cell : row)
			{
				cell->dbgOpen();
			}
		}
	}
	else
	{
		for (const auto &row : cells_)
		{
			for (const auto &cell : row)
			{
				cell->dbgClose();
			}
		}
	}
}

void GameBoard::saveBoard(const QString &file)
{
	if (blocked_)
	{
		for (const auto &row : cells_)
		{
			for (const auto &cell : row)
			{
				cell->dbgClose();
			}
		}
	}

	QSettings settings(file, QSettings::IniFormat);
	settings.beginGroup("Board");
	settings.setValue("rows_", rows_);
	settings.setValue("cols_", cols_);
	settings.setValue("mine_count_", mine_count_);
	settings.setValue("flag_count_", flag_count_);
	settings.setValue("first_click_made_", first_click_made_);
	settings.endGroup();

	settings.beginGroup("Cells");
	for (int row = 0; row < rows_; ++row)
	{
		for (int col = 0; col < cols_; ++col)
		{
			QString key = QString("cell_%1_%2").arg(row).arg(col);
			settings.setValue(key, cells_[row][col]->save());
		}
	}
	settings.endGroup();
}

void GameBoard::restoreBoard(const QString &file)
{
	QSettings settings(file, QSettings::IniFormat);
	settings.beginGroup("Board");
	rows_ = settings.value("rows_", 10).toInt();
	cols_ = settings.value("cols_", 10).toInt();
	flag_count_ = settings.value("flag_count_", 0).toInt();
	mine_count_ = settings.value("mine_count_", 10).toInt();
	first_click_made_ = settings.value("first_click_made_", false).toBool();
	settings.endGroup();

	if (mine_count_ - flag_count_ == 0)
	{
		blocked_ = true;
	}
	else
	{
		blocked_ = false;
	}
	initializeBoard();
	settings.beginGroup("Cells");
	for (int row = 0; row < rows_; ++row)
	{
		for (int col = 0; col < cols_; ++col)
		{
			QString key = QString("cell_%1_%2").arg(row).arg(col);
			QMap< QString, QVariant > state = settings.value(key).toMap();
			cells_[row][col]->restore(&state);
			if (cells_[row][col]->hasMine() && cells_[row][col]->isOpened())
			{
				blocked_ = true;
			}
		}
	}
	settings.endGroup();
}
