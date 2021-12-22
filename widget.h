#ifndef WIDGET_H
#define WIDGET_H

#include "complex.h"
#include "matrix.h"
#include <QWidget>
#include <QVector>
#include <vector>
#include <complex>

typedef std::complex<double> complexNumber;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = nullptr);
	~Widget();

private slots:
	void slotOnButtonGetSpectrumClicked();
	void slotOnButtonGetTransformClicked();

private:
	Ui::Widget *ui;

	void convertToGray();
	void imageBPF(bool direct);
	void fourierTransform(QVector<complexNumber> &t_vector, bool t_isDirect);
	int32_t numReverse(int32_t t_num, int32_t t_lgNum);
	void freqFilter();
	void lowFreqFilter();


	QImage			m_initialImage;
	QImage			m_spectrumImage;
	QImage			m_transformImage;

	QVector<int>	m_decompositionBase;
	int32_t			m_index;

	Matrix<complexNumber>	m_imageSpectrum;
};
#endif // WIDGET_H
