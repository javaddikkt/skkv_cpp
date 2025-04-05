#pragma once

#include "gameboard.h"

#include <QApplication>
#include <QCheckBox>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QSettings>
#include <QToolBar>
#include <QTranslator>

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
	MainWindow(QWidget *parent = nullptr);
	void saveGame(const QString &);
	void restoreGame(const QString &, bool);
	void createBoard(int = 10, int = 10, int = 10);
	~MainWindow();

  private:
	GameBoard *game_board_;
	QLabel *mine_counter_label_;
	QToolBar *toolbar_;
	QMenu *game_menu_;
	int mines_;
	int rows_;
	int columns_;
	bool debug_;
	bool rus_;

	void createMenus();
	void createToolbar();
	void retranslateUi();

  private slots:
	void newGame();
	void showSettingsDialog();
	void updateMineCounter();
	void changeLanguage();
	void createOldBoard();
};
