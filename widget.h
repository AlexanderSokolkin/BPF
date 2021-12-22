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
//	void slotOnRadioButtonClicked();
	void slotOnButtonGetSpectrumClicked();
	void slotOnButtonGetTransformClicked();

private:
	Ui::Widget *ui;

	void convertToGray();
	void imageBPF(bool direct);
//	QVector<Complex> stringDPF(const QVector<Complex> &t_vector, int t_coeff = 1);
//	QVector<Complex> DPF(const QVector<Complex> &t_vector, const int &t_N, bool t_isForward = true, int t_step = 0);
//	QVector<Complex> restoreOrder(const QVector<Complex> &t_vector);
	void fourierTransform(QVector<complexNumber> &t_vector, bool t_isDirect);
	int numReverse(int32_t t_num, int32_t t_lgNum);


	QImage			m_initialImage;
	QImage			m_spectrumImage;
	QImage			m_transformImage;

	QVector<int>	m_decompositionBase;
	int32_t			m_index;

	Matrix<complexNumber>	m_imageSpectrum;
};
#endif // WIDGET_H
