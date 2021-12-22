#include "widget.h"
#include "ui_widget.h"
#include "matrix.h"
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

//	connect(ui->rbt_noFilter, &QRadioButton::clicked, this, &Widget::slotOnRadioButtonClicked);
//	connect(ui->rbt_hightFreqFilter, &QRadioButton::clicked, this, &Widget::slotOnRadioButtonClicked);
//	connect(ui->rbt_lowFreqFilter, &QRadioButton::clicked, this, &Widget::slotOnRadioButtonClicked);
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
	Matrix<complexNumber> matrix(512, 512);

	// Формируем матрицу отсчетов
	if (direct) {
		for (int x = 0; x < 512; ++x) {
			for (int y = 0; y < 512; ++y) {
				int32_t h, s, v;
				m_initialImage.pixelColor(x, y).getHsv(&h, &s, &v);
				matrix(x, y) = v * pow(-1, x + y);
			}
		}
	} else {
		matrix = m_imageSpectrum;
	}

	// БПФ колонок
	for (int i = 0; i < 512; ++i) {
		QVector<complexNumber> column = matrix.getColumn(i);
		fourierTransform(column, direct);
		matrix.setColumn(column, i);
	}

	// БПФ строк
	for (int i = 0; i < 512; ++i) {
		fourierTransform(matrix[i], direct);
	}

//	int r = 60;
//	if (direct) {
//		for (int x = 0; x < 512; ++x) {
//			for (int y = 0; y < 512; ++y) {
//				if (pow((x - 256), 2) + pow((y - 256), 2) <= pow(r, 2)) {
//					matrix(x, y) = 0;
//				}
//			}
//		}
//	}

	QImage result = m_initialImage;
	int width = 512;
	int height = 512;
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			QColor color;
			int32_t value = matrix(x, y).real() * (direct ? 1 : pow(-1, x + y));
//			int32_t value = sqrt(pow(matrix(x, y).real(), 2) + pow(matrix(x, y).imag(), 2));
//			value = (value > 255) ? 255 : value;
//			value = (value < 0) ? 0 : value;
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

//QVector<Complex> Widget::stringDPF(const QVector<Complex> &t_vector, int t_coeff)
//{
//	int rows = m_decompositionBase[m_index];
//	int cols = t_vector.size() / rows;

//	// Заполнение матрицы (вектор -> матрица)
//	Matrix<Complex> matrix(rows, cols);
//	for (int x = 0; x < rows; ++x) {
//		for (int y = 0; y < cols; ++y) {
//			matrix(x, y) = t_vector[x * cols + y];
//		}
//	}

//	// ДПФ стобцов и умножения элементов на поворачивающие множители
//	Matrix<Complex> matrixDPF(rows, cols);
//	for (int i = 0; i < cols; ++i) {
//		QVector<Complex> colDPF = restoreOrder(DPF(matrix.getColumn(i), rows));
//		for (int j = 0; j < rows; ++j) {
//			matrixDPF(j, i) = colDPF[j] * Complex::exp((-2 * M_PI) * (i * j * t_coeff / 512));
//		}
//	}

//	if (m_index != m_decompositionBase.size() - 2) {
//		for (int i = 0; i < rows; ++i) {
//			stringDPF(matrixDPF.getRow(i));
//		}
//	} else {

//	}
//}

//QVector<Complex> Widget::DPF(const QVector<Complex> &t_vector, const int &t_N, bool t_isForward, int t_step)
//{
//	int vectorSize = t_vector.size();
//	if (vectorSize == 2) {
//		return QVector<Complex>() = {t_vector[0] + t_vector[1],
//									t_vector[0] - t_vector[1]};
//	}

//	QVector<Complex> vec1(vectorSize / 2);
//	QVector<Complex> vec2(vectorSize / 2);
//	for (int i = 0; i < vectorSize / 2; ++i) {
//		vec1[i] = t_vector[i] + t_vector[i + vectorSize / 2];
//		vec2[i] = (t_vector[i] - t_vector[i + vectorSize / 2]) * Complex::exp(( (t_isForward ? -1 : 1) * 2 * M_PI / t_N) * (std::pow(2, t_step) * i));
//	}

//	QVector<Complex> result;
//	result.reserve(vectorSize);
//	result.append(DPF(vec1, t_N, t_isForward, t_step + 1));
//	result.append(DPF(vec2, t_N, t_isForward, t_step + 1));

//	return result;
//}

//QVector<Complex> Widget::restoreOrder(const QVector<Complex> &t_vector)
//{
//	QVector<Complex> result(t_vector.size());
//	int digitsCount = std::log2(t_vector.size());
//	for (int i = 0; i < t_vector.size(); ++i) {
//		QString binIndex = QString::number(i, 2);
//		while (digitsCount - binIndex.size() != 0) {
//			binIndex = "0" + binIndex;
//		}
//		for (int k = 0, t = binIndex.size() - 1; k < t; ++k, --t) {
//			std::swap(binIndex[k], binIndex[t]);
//		}
//		bool ok;
//		int decIndex = binIndex.toInt(&ok, 2);
//		result[i] = t_vector[decIndex];
//	}

//	return result;
//}

int Widget::numReverse(int32_t t_num, int32_t t_lgNum) {
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

//void Widget::slotOnRadioButtonClicked()
//{
//	QRadioButton *rbt = qobject_cast<QRadioButton*>(sender());
//	if (rbt->isChecked()) {
//		return;
//	}

//	ui->rbt_noFilter->setChecked(false);
//	ui->rbt_hightFreqFilter->setChecked(false);
//	ui->rbt_lowFreqFilter->setChecked(false);
//	rbt->setChecked(true);
//}

void Widget::slotOnButtonGetSpectrumClicked()
{
	ui->image_transformed->clear();
	imageBPF(true);
	ui->image_spectrum->setPixmap(QPixmap::fromImage(m_spectrumImage));
}

void Widget::slotOnButtonGetTransformClicked()
{
	imageBPF(false);
	ui->image_transformed->setPixmap(QPixmap::fromImage(m_transformImage));
}
