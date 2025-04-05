#pragma once

#include "cell.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSettings>
#include <QTimer>
#include <QVector>
#include <QWidget>

class GameBoard : public QWidget
{
	Q_OBJECT

  public:
	GameBoard(QWidget *, int, int, int);
	GameBoard(QWidget *, const QString &);
	void setupBoard(int, int, int);
	void placeMines(int, int);
	void checkWin();
	int getRowsNumber();
	int getColsNumber();
	int getFlagsNumber();
	int getMinesNumber();
	void highlightArea(int, int, int, int);
	void unhighlightArea(int, int, int, int);
	void initializeBoard();
	void saveBoard(const QString &);
	void restoreBoard(const QString &);

  public slots:
	void changeHands();
	void watch();

  private:
	QGridLayout *layout_;
	QVector< QVector< Cell * > > cells_;
	int rows_;
	int cols_;
	int mine_count_;
	int flag_count_;
	bool first_click_made_;
	bool right_hand_ = true;
	bool blocked_;

	void setLayout();
	void endGame(bool);
	void clear();
	void revealEmptyCells(int, int);
	bool recursiveOpen(int, int);
	void resizeEvent(QResizeEvent *event) override;

  private slots:
	void handleLeftClick(int, int);
	void handleRightClick(int, int);
	void handleMiddleClick(int, int);

  signals:
	void restartGame();
	void mineFlagChanged();
};
