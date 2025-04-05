#pragma once

#include <QImage>
#include <QMap>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

class Cell : public QPushButton
{
	Q_OBJECT

  public:
	explicit Cell(QWidget *, int, int);
	void setMine(bool);
	bool hasMine() const;
	void setFlag(bool);
	bool hasFlag() const;
	void setQuestionMark(bool);
	bool hasQuestionMark() const;
	bool isOpened() const;
	int getNumber() const;
	void setNumber(int);
	void open();
	void dbgOpen();
	void dbgClose();
	void close();
	QMap< QString, QVariant > save() const;
	void restore(QMap< QString, QVariant > *);
	void showEmpty();
	void closeEmpty();
	void showLast();

  private:
	int x_;
	int y_;
	int number_;
	bool opened_;
	bool mine_;
	bool flag_;
	bool question_mark_;
	bool dbg_opened_;
	QImage image_;

	void setPicture();
	void paintEvent(QPaintEvent *) override;
	void mousePressEvent(QMouseEvent *) override;

  signals:
	void leftClick(int, int);
	void rightClick(int, int);
	void middleClick(int, int);
};
