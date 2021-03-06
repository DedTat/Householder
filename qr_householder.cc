#include <iostream>
#include <iomanip>
#include <math.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)<(b)?(b):(a))

class cMatrix {
  private:
  protected:
  public:
	double *A;
	int m, n;

	cMatrix(const int m, const int n);
	cMatrix(const int m, const int n, const double A[]);

	~cMatrix();

	cMatrix& set(const double A[]);
	cMatrix& eye();
	cMatrix& zero();
	cMatrix transpose();
	cMatrix  operator*(const cMatrix& B);
	cMatrix  operator*(const double s);
	cMatrix  operator+(const cMatrix& B);
	cMatrix  operator-(const cMatrix& B);
	cMatrix& operator=(const cMatrix& B);
	void householderDecomposition(cMatrix& Q, cMatrix& R);
	void output();
};


void cMatrix::output() {
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			std::cout << std::setw(16) << A[j * n + i] << "   ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

cMatrix::cMatrix(const int m, const int n) : m(m), n(n) {
	A = new double[m * n];
	this->eye();
}

cMatrix::cMatrix(const int m, const int n, const double A[]) : m(m), n(n) {
	this->A = new double[m * n];
	for (int i = 0; i < m * n; i++) this->A[i] = A[i];
}

cMatrix::~cMatrix() {
	delete [] A;
}

cMatrix& cMatrix::set(const double A[]) {
	for (int i = 0; i < m * n; i++) this->A[i] = A[i];
	return *this;
}

cMatrix& cMatrix::eye() {
	for (int j = 0; j < m; j++)
		for (int i = 0; i < n; i++)
			A[j * n + i] = i == j ? 1.0 : 0.0;
	return *this;
}

cMatrix& cMatrix::zero() {
	for (int j = 0; j < m; j++)
		for (int i = 0; i < n; i++)
			A[j * n + i] = 0.0;
	return *this;
}

cMatrix cMatrix::transpose() {
	cMatrix R(n, m);
	for (int j = 0; j < m; j++)
		for (int i = 0; i < n; i++)
			R.A[i * m + j] = A[j * n + i];
	return R;
}

cMatrix cMatrix::operator*(const cMatrix& B) {
	cMatrix R(m, B.n);
	for (int j = 0; j < m; j++)
		for (int i = 0; i < B.n; i++) {
			R.A[j * B.n + i] = 0.0;
			for (int k = 0; k < n; k++)
				R.A[j * B.n + i] += A[j * n + k] * B.A[k * B.n + i];
		}
	return R;
}

cMatrix cMatrix::operator*(const double s) {
	cMatrix R(m, n);
	for (int j = 0; j < m; j++)
		for (int i = 0; i < n; i++)
			R.A[j * n + i] = A[j * n + i] * s;
	return R;
}

cMatrix cMatrix::operator+(const cMatrix& B) {
	cMatrix R(m, n);
	for (int j = 0; j < m; j++)
		for (int i = 0; i < n; i++)
			R.A[j * n + i] = A[j * n + i] + B.A[j * n + i];
	return R;
}

cMatrix cMatrix::operator-(const cMatrix& B) {
	cMatrix R(m, n);
	for (int j = 0; j < m; j++)
		for (int i = 0; i < n; i++)
			R.A[j * n + i] = A[j * n + i] - B.A[j * n + i];
	return R;
}

cMatrix& cMatrix::operator=(const cMatrix& B) {
	if (m * n != B.m * B.n) {
		delete [] A;
		A = new double[B.m * B.n];
	}
	m = B.m;
	n = B.n;

	for (int i = 0; i < m * n; i++) A[i] = B.A[i];
	return *this;
}

void cMatrix::householderDecomposition(cMatrix& Q, cMatrix& R) {
	double mag, alpha;
	cMatrix u(m, 1), v(m, 1);
	cMatrix P(m, m), I(m, m);

	Q = cMatrix(m, m);
	R = *this;

	for (int i = 0; i < n; i++) {
		u.zero(); v.zero();
		
		mag = 0.0;
		for (int j = i; j < m; j++) {
			u.A[j] = R.A[j * n + i];
			mag += u.A[j] * u.A[j];
		}
		mag = sqrt(mag);
		
		alpha = u.A[i] < 0 ? mag : -mag;

		mag = 0.0;
		for (int j = i; j < m; j++) {
			v.A[j] = j == i ? u.A[j] + alpha : u.A[j];
			mag += v.A[j] * v.A[j];
		}
		mag = sqrt(mag);

		if (mag < 0.0000000001) continue;

		for (int j = i; j < m; j++) v.A[j] /= mag;

		P = I - (v * v.transpose()) * 2.0;

		R = P * R;
		Q = Q * P;
	}
}

int main(int argc, char* argv[]) {

	double temp[] = {2, 1, 3, 5,    -1, 0, 7, 1,    0, -1, -1, 3,   -3, 7, 4, 3,   1, 6, 4, -3};
	cMatrix A__(5,4,temp);
	cMatrix Q__(1,1), R__(1,1);
	A__.householderDecomposition(Q__, R__);

	A__.output();
	Q__.output();
	R__.output();
	(Q__*R__).output();

	return 0;
}