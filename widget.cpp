#include "widget.h"
#include "ui_widget.h"
#include "matrix.h"
#include "constants.h"
#include <cmath>
#include <QtMath>

using namespace std;

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);
	m_decompositionBase = {4, 2, 2, 4, 4, 2};
	m_index = 0;
	m_initialImage = QImage("://images/Cat.jpg");
	convertToGray();

	ui->image_initial->setPixmap(QPixmap::fromImage(m_initialImage));
	ui->rbt_noFilter->setChecked(true);

	connect(ui->pbt_getSpectrum, &QPushButton::clicked, this, &Widget::slotOnButtonGetSpectrumClicked);
	connect(ui->pbt_getTransform, &QPushButton::clicked, this, &Widget::slotOnButtonGetTransformClicked);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::convertToGray()
{
	int width = m_initialImage.width();
	int height = m_initialImage.height();

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int a, b, c;
			QColor color = m_initialImage.pixelColor(x, y);
			color.getHsv(&a, &b, &c);
			color.setHsv(0, 0, c);
			m_initialImage.setPixelColor(x, y, color);
		}
	}
}

void Widget::imageBPF(bool direct)
{
	Matrix<complexNumber> matrix(WIDTH, HEIGHT);

	// Формируем матрицу отсчетов
	if (direct) {
		for (int x = 0; x < WIDTH; ++x) {
			for (int y = 0; y < HEIGHT; ++y) {
				int32_t h, s, v;
				m_initialImage.pixelColor(x, y).getHsv(&h, &s, &v);
				matrix(x, y) = v * pow(-1, x + y);
			}
		}
	} else {
		matrix = m_imageSpectrum;

		if (!ui->rbt_noFilter->isChecked()) {
			int32_t invers = ui->rbt_hightFreqFilter->isChecked() ? 1 : -1;
			for (int x = 0; x < WIDTH; ++x) {
				for (int y = 0; y < HEIGHT; ++y) {
					if (invers * (pow((x - (WIDTH / 2)), 2) + pow((y - (HEIGHT / 2)), 2)) <= invers * (pow(RADIUS, 2))) {
						matrix(x, y) = 0;
					}
				}
			}
		}
	}

	// БПФ колонок
	for (int i = 0; i < WIDTH; ++i) {
		QVector<complexNumber> column = matrix.getColumn(i);
		fourierTransform(column, direct);
		matrix.setColumn(column, i);
	}

	// БПФ строк
	for (int i = 0; i < HEIGHT; ++i) {
		fourierTransform(matrix[i], direct);
	}

	QImage result = m_initialImage;
	for (int x = 0; x < WIDTH; ++x) {
		for (int y = 0; y < HEIGHT; ++y) {
			QColor color;
			int32_t value = matrix(x, y).real() * (direct ? 1 : pow(-1, x + y));
			if (!direct) {
				value = (value < 0) ? 0 : value > 255 ? 255 : value;
			}
			color.setHsv(0, 0, value);
			result.setPixelColor(x, y, color);
		}
	}

	if (direct) {
		m_spectrumImage = result;
		m_imageSpectrum = matrix;
	} else {
		m_transformImage = result;
	}
}


int32_t Widget::numReverse(int32_t t_num, int32_t t_lgNum) {
	int32_t res = 0;
	for (int i = 0; i < t_lgNum; ++i) {
		if (t_num & (1 << i)) {
			res |= 1 << (t_lgNum - 1 - i);
		}
	}

	return res;
}

void Widget::fourierTransform(QVector<complexNumber> &t_vector, bool t_isDirect) {
	int32_t pointCount = t_vector.size();
	int32_t lgPointCount = log2(pointCount);

	for (int i = 0; i < pointCount; ++i) {
		if (i < numReverse(i, lgPointCount)) {
			swap(t_vector[i], t_vector[numReverse(i, lgPointCount)]);
		}
	}

	for (int len = 2; len <= pointCount; len <<= 1) {
		double angel = 2 * M_PI / len * (t_isDirect ? -1 : 1);
		complexNumber exponent(cos(angel), sin(angel));
		for (int i = 0; i < pointCount; i += len) {
			complexNumber turningFactor(1);
			for (int j = 0; j < len / 2; ++j) {
				complexNumber u = t_vector[i + j];
				complexNumber v = t_vector[i + j + len / 2] * turningFactor;
				t_vector[i + j] = u + v;
				t_vector[i + j + len / 2] = u - v;
				turningFactor *= exponent;
			}
		}
	}

	if (t_isDirect) {
		for (int i = 0; i < pointCount; ++i) {
			t_vector[i] /= pointCount;
		}
	}
}

void Widget::slotOnButtonGetSpectrumClicked()
{
	ui->image_transformed->clear();
	imageBPF(true);
	ui->image_spectrum->setPixmap(QPixmap::fromImage(m_spectrumImage));
}

void Widget::slotOnButtonGetTransformClicked()
{
	if (m_imageSpectrum.isEmpty()) {
		return;
	}

	imageBPF(false);
	ui->image_transformed->setPixmap(QPixmap::fromImage(m_transformImage));
}
