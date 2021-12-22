#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>

template <typename T>
class Matrix
{
public:
	Matrix();
	Matrix(int t_rows, int t_cols, bool t_isE = false);
	Matrix(QVector<T> t_array, bool t_isHorizontal = true);
	Matrix(Matrix &t_otherMatrix);

	int getColsCount() const;
	int getRowsCount() const;
	QVector<T> getRow(int t_rowIndex) const;
	QVector<T> getColumn(int t_colIndex) const;
	void setColumn(const QVector<T> &t_column, int t_colIndex);
	T& operator()(const int &t_row, const int &t_column);
	void swapRows(int t_firstRowIndex, int t_secondRowIndex);
	void swapColumns(int t_firstColIndex, int t_secondColIndex);
	Matrix<T> operator+(const Matrix<T> &t_otherMatrix);
	Matrix<T> operator*(Matrix<T> &t_otherMatrix);
	Matrix<T> operator*(const double &t_num);
	QVector<T>& operator[](const double &t_num);
//	Matrix<T>& operator=(const Matrix<T> &t_otherMatrix);
	Matrix<T> transp(Matrix<T> &t_matrix);
	bool isEmpty();
	void clear();


private:
	int m_rows;
	int m_cols;
	QVector<QVector<T>> m_matrix;
};
#endif // MATRIX_H
