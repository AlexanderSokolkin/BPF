#include "matrix.h"
#include "complex.h"
#include <complex>

template <typename T>
Matrix<T>::Matrix()
{
	m_rows = 0;
	m_cols = 0;
}

template <typename T>
Matrix<T>::Matrix(int t_rows, int t_cols, bool t_isE)
{
	m_rows = t_rows;
	m_cols = t_cols;

	m_matrix.reserve(t_rows);
	T initNum = t_isE ? 1 : 5;
	for (int i = 0; i < t_rows; ++i) {
		QVector<T> row(m_cols, initNum);
		m_matrix.append(row);
	}
}

template <typename T>
Matrix<T>::Matrix(QVector<T> t_array, bool t_isHorizontal)
{
	if (t_isHorizontal)
	{
	   m_rows = 1;
	   m_cols = t_array.size();
	   m_matrix.append(t_array);
	}
	else
	{
		m_rows = t_array.size();
		m_cols = 1;
		m_matrix.reserve(m_cols);
		for (int i = 0; i < m_rows; ++i) {
			QVector<T> row(1);
			row.append(t_array[i]);
			m_matrix.append(row);
		}
	}
}

template <typename T>
Matrix<T>::Matrix(Matrix &t_otherMatrix)
{
	m_rows = t_otherMatrix.m_rows;
	m_cols = t_otherMatrix.m_cols;
	m_matrix = t_otherMatrix.m_matrix;
}

template <typename T>
int Matrix<T>::getColsCount() const
{
	return m_cols;
}

template <typename T>
int Matrix<T>::getRowsCount() const
{
	return m_rows;
}

template <typename T>
QVector<T> Matrix<T>::getRow(int t_rowIndex) const
{
	return m_matrix[t_rowIndex];
}

template <typename T>
QVector<T> Matrix<T>::getColumn(int t_colIndex) const
{
	QVector<T> resultCol;
	resultCol.reserve(m_rows);
	for (auto &row : m_matrix) {
		resultCol.append(row[t_colIndex]);
	}

	return resultCol;
}

template <typename T>
void Matrix<T>::setColumn(const QVector<T> &t_column, int t_colIndex)
{
	for (int row = 0; row < m_rows; ++row) {
		m_matrix[row][t_colIndex] = t_column[row];
	}
}

template <typename T>
T& Matrix<T>::operator()(const int &t_row, const int &t_column)
{
	return m_matrix[t_row][t_column];
}

template <typename T>
void Matrix<T>::swapRows(int t_firstRowIndex, int t_secondRowIndex)
{
	for (int i = 0; i < m_cols; ++i) {
		std::swap(m_matrix[t_firstRowIndex][i], m_matrix[t_secondRowIndex][i]);
	}
}

template <typename T>
void Matrix<T>::swapColumns(int t_firstColIndex, int t_secondColIndex)
{
	for (int i = 0; i < m_rows; ++i) {
		std::swap(m_matrix[i][t_firstColIndex], m_matrix[i][t_secondColIndex]);
	}
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &t_otherMatrix)
{
	Matrix<T> result(m_rows, m_cols);
	for (int i = 0; i < m_rows; ++i) {
		result[i] = this->m_matrix[i] + t_otherMatrix.getRow(i);
	}

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix<T> &t_otherMatrix)
{
	Matrix<T> result(m_cols, t_otherMatrix.getRowsCount());

	for (int i = 0; i < m_cols; ++i) {
	   for (int j = 0; j < t_otherMatrix.getRowsCount(); ++j) {
		  for (int k = 0; k < m_rows; ++k) {
			 result(i, j) = m_matrix[i][k] * t_otherMatrix(k, j);
		  }
	   }
	}

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const double &t_num)
{
	Matrix<T> result(*this);
	for (int i = 0; i < m_rows; ++i) {
	   for (int j = 0; j < m_cols; ++j) {
			result(i, j) = m_matrix[i][j] * t_num;
		}
	}

	return result;
}

template <typename T>
QVector<T>& Matrix<T>::operator[](const double &t_num)
{
	return m_matrix[t_num];
}

template <typename T>
Matrix<T> Matrix<T>::transp(Matrix<T> &t_matrix)
{
	Matrix<T> result(t_matrix.getRowsCount(), t_matrix.getColsCount());
	for (int i = 0; i < t_matrix.getColsCount(); ++i) {
	   for (int j = 0; j < t_matrix.getRowsCount(); ++j) {
		  result(j, i) = t_matrix(i, j);
	   }
	}

	return result;
}

template class Matrix<double>;
template class Matrix<int>;
template class Matrix<Complex>;
template class Matrix<std::complex<double>>;
