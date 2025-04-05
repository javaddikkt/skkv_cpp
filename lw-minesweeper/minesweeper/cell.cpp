#include "cell.h"

Cell::Cell(QWidget *parent, int x_, int y_) :
	QPushButton(parent), x_(x_), y_(y_), number_(0), opened_(false), mine_(false), flag_(false), question_mark_(false),
	dbg_opened_(false), image_(NULL)
{
	setPicture();
	setMinimumSize(50, 50);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Cell::setPicture()
{
	if (mine_ && (opened_ || dbg_opened_))
	{
		image_ = QImage(":/images/bomb-1.png");
	}
	else if (opened_ || dbg_opened_)
	{
		switch (number_)
		{
		case 1:
			image_ = QImage(":/images/1.png");
			break;
		case 2:
			image_ = QImage(":/images/2.png");
			break;
		case 3:
			image_ = QImage(":/images/3.png");
			break;
		case 4:
			image_ = QImage(":/images/4.png");
			break;
		case 5:
			image_ = QImage(":/images/5.png");
			break;
		case 6:
			image_ = QImage(":/images/6.png");
			break;
		case 7:
			image_ = QImage(":/images/7.png");
			break;
		case 8:
			image_ = QImage(":/images/8.png");
			break;
		default:
			image_ = QImage(":/images/0.png");
		}
	}
	else if (flag_)
	{
		image_ = QImage(":/images/flag-1.png");
	}
	else if (question_mark_)
	{
		image_ = QImage(":/images/question-mark.png");
	}
	else
	{
		image_ = QImage(":/images/empty.png");
	}
	update();
}

void Cell::showEmpty()
{
	image_ = QImage(":/images/0.png");
	update();
}

void Cell::closeEmpty()
{
	setPicture();
}

void Cell::showLast()
{
	image_ = QImage(":/images/bang1.png");
	update();
}

void Cell::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawImage(rect(), image_);
}

void Cell::setNumber(int num)
{
	number_ = num;
}

int Cell::getNumber() const
{
	return number_;
}

void Cell::setMine(bool m)
{
	number_ = -1;
	mine_ = m;
}

bool Cell::hasMine() const
{
	return mine_;
}

void Cell::setFlag(bool f)
{
	flag_ = f;
	setPicture();
}

bool Cell::hasFlag() const
{
	return flag_;
}

void Cell::setQuestionMark(bool q)
{
	question_mark_ = q;
	setPicture();
}

bool Cell::hasQuestionMark() const
{
	return question_mark_;
}

bool Cell::isOpened() const
{
	return opened_;
}

void Cell::mousePressEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
		emit leftClick(x_, y_);
		break;
	case Qt::RightButton:
		emit rightClick(x_, y_);
		break;
	case Qt::MiddleButton:
		emit middleClick(x_, y_);
		break;
	default:
		return;
	}
}

void Cell::open()
{
	if (opened_ || flag_ || question_mark_)
	{
		return;
	}
	opened_ = true;
	setPicture();
}

void Cell::dbgOpen()
{
	if (opened_)
	{
		return;
	}
	dbg_opened_ = true;
	setPicture();
}

void Cell::dbgClose()
{
	if (!dbg_opened_)
	{
		return;
	}
	dbg_opened_ = false;
	setPicture();
}

void Cell::close()
{
	opened_ = false;
	number_ = 0;
	mine_ = false;
	flag_ = false;
	question_mark_ = false;
	setPicture();
}

QMap< QString, QVariant > Cell::save() const
{
	QMap< QString, QVariant > data;
	data["number_"] = number_;
	data["opened_"] = opened_;
	data["mine_"] = mine_;
	data["flag_"] = flag_;
	data["question_mark_"] = question_mark_;
	data["image_"] = image_;
	return data;
}

void Cell::restore(QMap< QString, QVariant > *data)
{
	number_ = data->value("number_", 0).toInt();
	opened_ = data->value("opened_", false).toBool();
	mine_ = data->value("mine_", false).toBool();
	flag_ = data->value("flag_", false).toBool();
	question_mark_ = data->value("question_mark_", false).toBool();
	image_ = data->value("image_", QImage(":/images/empty.png")).value< QImage >();
	update();
}
