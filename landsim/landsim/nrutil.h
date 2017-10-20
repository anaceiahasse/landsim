#ifndef _NR_UTIL_H_
#define _NR_UTIL_H_

#include <string>
#include <cmath>
#include <complex>
#include <iostream>

using namespace std;
typedef double DP;

/* ---------------------------------------------------
   Miscellaneous Functions
   ---------------------------------------------------
*/

template<class T>
inline const T SQR(const T a) {return a*a;}

template<class T>
inline const T MAX(const T &a, const T &b)
{return b > a ? (b) : (a);}

inline float MAX(const double &a, const float &b)
{return b > a ? (b) : float(a);}

inline float MAX(const float &a, const double &b)
{return b > a ? float(b) : (a);}

template<class T>
inline const T MIN(const T &a, const T &b)
{return b < a ? (b) : (a);}

inline float MIN(const double &a, const float &b)
{return b < a ? (b) : float(a);}

inline float MIN(const float &a, const double &b)
{return b < a ? float(b) : (a);}

template<class T>
inline const T SIGN(const T &a, const T &b)
   {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

inline float SIGN(const float &a, const double &b)
   {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

inline float SIGN(const double &a, const float &b)
   {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

template<class T>
inline void SWAP(T &a, T &b)
   {T dum=a; a=b; b=dum;}

/*namespace NR {
   void nrerror(const string error_text)
   // Numerical Recipes standard error handler
   {
   cerr << "Numerical Recipes run-time error..." << endl;
   cerr << error_text << endl;
   cerr << "...now exiting to system..." << endl;
   exit(1);
   }
}*/

/* ---------------------------------------------------
   Class NRVec
   ---------------------------------------------------
*/

#include <valarray>
#define NRVec valarray

/* ---------------------------------------------------
   Class NRMat
   ---------------------------------------------------
*/

template <class T>
class NRMat {
   private:
      int nn;
      int mm;
      T** v;
   public:
      NRMat();
      NRMat(int n, int m); // Zero-based array
      NRMat(const T &a, int n, int m); //Initialize to constant
      NRMat(const T *a, int n, int m); // Initialize to array
      NRMat(T **a, int n, int m); //new:Initialize to matrix
      NRMat(const NRMat &rhs); // Copy constructor
      NRMat & operator=(const NRMat &rhs); //assignment
      NRMat & operator=(const T &a); //assign a to every element
      inline T* operator[](const int i); //subscripting: pointer to row i
      inline const T* operator[](const int i) const;
      inline int nrows() const;
      inline int ncols() const;
     //My extensions
 //     valarray<T>& array() { valarray(v[0],mm*nn); }
      //Overloaded operators
      NRMat & operator-();
      NRMat & operator+=(const T&);
      NRMat & operator-=(const T&);

      //Functions
      T min() const;
      T max() const;
      //NRMat apply(T func(T)) const;

      ~NRMat();
};

/* ---------------------------------------------------
   Class NRMat3d
   ---------------------------------------------------
*/

template <class T>
class NRMat3d {
   private:
      int nn;
      int mm;
      int kk;
      T ***v;
   public:
      NRMat3d();
      NRMat3d(int n, int m, int k);
      inline T** operator[](const int i); //subscripting: pointer to row i
      inline const T* const * operator[](const int i) const;
      inline int dim1() const;
      inline int dim2() const;
      inline int dim3() const;
      ~NRMat3d();
};


//Overloaded complex operations to handle mixed float and double
//This takes care of e.g. 1.0/z, z complex<float>

inline const complex<float> operator+(const double &a,
const complex<float> &b) { return float(a)+b; }

inline const complex<float> operator+(const complex<float> &a,
const double &b) { return a+float(b); }

inline const complex<float> operator-(const double &a,
const complex<float> &b) { return float(a)-b; }

inline const complex<float> operator-(const complex<float> &a,
const double &b) { return a-float(b); }

inline const complex<float> operator*(const double &a,
const complex<float> &b) { return float(a)*b; }

inline const complex<float> operator*(const complex<float> &a,
const double &b) { return a*float(b); }

inline const complex<float> operator/(const double &a,
const complex<float> &b) { return float(a)/b; }

inline const complex<float> operator/(const complex<float> &a,
const double &b) { return a/float(b); }

//some compilers choke on pow(float,double) in single precision. also atan2

inline float pow (float x, double y) {return pow(double(x),y);}

inline float pow (double x, float y) {return pow(x,double(y));}

inline float atan2 (float x, double y) {return atan2(double(x),y);}

inline float atan2 (double x, float y) {return atan2(x,double(y));}



//implementation

/* ---------------------------------------------------
   Class NRMat
   ---------------------------------------------------
*/

template <class T>
NRMat<T>::NRMat() : nn(0), mm(0), v(0) {}

template <class T>
NRMat<T>::NRMat(int n, int m) : nn(n), mm(m), v(new T*[n])
{
 v[0] = new T[m*n];
 for (int i=1; i< n; i++)
   v[i] = v[i-1] + m;
}

template <class T>
NRMat<T>::NRMat(const T &a, int n, int m) : nn(n), mm(m), v(new T*[n])
{
 int i,j;
 v[0] = new T[m*n];
 for (i=1; i< n; i++)
   v[i] = v[i-1] + m;
 for (i=0; i< n; i++)
   for (j=0; j<m; j++)
      v[i][j] = a;
}

template <class T>
NRMat<T>::NRMat(const T *a, int n, int m) : nn(n), mm(m), v(new T*[n])
{
 int i,j;
 v[0] = new T[m*n];
 for (i=1; i< n; i++)
   v[i] = v[i-1] + m;
 for (i=0; i< n; i++)
   for (j=0; j<m; j++)
      v[i][j] = *a++;
}

template <class T>
NRMat<T>::NRMat(T **a, int n, int m) : nn(n), mm(m), v(new T*[n])
{
 int i,j;
 v[0] = new T[m*n];
 for (i=1; i< n; i++)
   v[i] = v[i-1] + m;
 for (i=0; i< n; i++)
   for (j=0; j<m; j++)
      v[i][j] = a[i][j];
}

template <class T>
NRMat<T>::NRMat(const NRMat &rhs) : nn(rhs.nn), mm(rhs.mm), v(new T*[nn])
{
 int i,j;
 v[0] = new T[mm*nn];
 for (i=1; i< nn; i++)
   v[i] = v[i-1] + mm;
 for (i=0; i< nn; i++)
   for (j=0; j<mm; j++)
      v[i][j] = rhs[i][j];
}

template <class T>
NRMat<T> & NRMat<T>::operator=(const NRMat<T> &rhs)
// postcondition: normal assignment via copying has been performed;
// if matrix and rhs were different sizes, matrix
// has been resized to match the size of rhs
{
 if (this != &rhs) {
   int i,j;
   if (nn != rhs.nn || mm != rhs.mm) {
      if (v != 0) {
         delete[] (v[0]);
         delete[] (v);
         }
      nn=rhs.nn;
      mm=rhs.mm;
      v = new T*[nn];
      v[0] = new T[mm*nn];
      }
   for (i=1; i< nn; i++)
      v[i] = v[i-1] + mm;
   for (i=0; i< nn; i++)
      for (j=0; j<mm; j++)
         v[i][j] = rhs[i][j];
   }
 return *this;
}

template <class T>
NRMat<T> & NRMat<T>::operator=(const T &a) //assign a to every element
{
 for (int i=0; i< nn; i++)
   for (int j=0; j<mm; j++)
      v[i][j] = a;
 return *this;
}

template <class T>
inline T* NRMat<T>::operator[](const int i) //subscripting: pointer to row i
{
 return v[i];
}

template <class T>
inline const T* NRMat<T>::operator[](const int i) const
{
 return v[i];
}

template <class T>
inline int NRMat<T>::nrows() const
{
 return nn;
}

template <class T>
inline int NRMat<T>::ncols() const
{
 return mm;
}

template <class T>
NRMat<T> & NRMat<T>::operator-()
{
  for (int i=0; i< nn; i++)
   for (int j=0; j<mm; j++)
      v[i][j] = - v[i][j];
 return *this;
}

template <class T>
NRMat<T> & NRMat<T>::operator+=(const T &a)
{
 for (int i=0; i< nn; i++)
   for (int j=0; j<mm; j++)
      v[i][j] += a;
 return *this;
}

template <class T>
NRMat<T> & NRMat<T>::operator-=(const T &a)
{
 for (int i=0; i< nn; i++)
   for (int j=0; j<mm; j++)
      v[i][j] -= a;
 return *this;
}

template <class T>
T NRMat<T>::max() const
{
 T x = v[0][0];

 for (int i=0; i< nn; i++)
   for (int j=0; j<mm; j++)
      if (x < v[i][j]) x=v[i][j];
 return x;
}

template <class T>
T NRMat<T>::min() const
{
 T x = v[0][0];

 for (int i=0; i< nn; i++)
   for (int j=0; j<mm; j++)
      if (x > v[i][j]) x=v[i][j];
 return x;
}

template <class T>
NRMat<T>::~NRMat()
{
 if (v != 0) {
   delete[] (v[0]);
   delete[] (v);
   }
}

/* ---------------------------------------------------
   Class NRMat3d
   ---------------------------------------------------
*/

template <class T>
NRMat3d<T>::NRMat3d(): nn(0), mm(0), kk(0), v(0) {}

template <class T>
NRMat3d<T>::NRMat3d(int n, int m, int k) : nn(n), mm(m), kk(k), v(new T**[n])
{
 int i,j;
 v[0] = new T*[n*m];
 v[0][0] = new T[n*m*k];
 for(j=1; j<m; j++)
   v[0][j] = v[0][j-1] + k;
 for(i=1; i<n; i++) {
   v[i] = v[i-1] + m;
   v[i][0] = v[i-1][0] + m*k;
   for(j=1; j<m; j++)
      v[i][j] = v[i][j-1] + k;
   }
}

template <class T>
inline T** NRMat3d<T>::operator[](const int i) //subscripting: pointer to row i
{
 return v[i];
}

template <class T>
inline const T* const * NRMat3d<T>::operator[](const int i) const
{
 return v[i];
}

template <class T>
inline int NRMat3d<T>::dim1() const
{
 return nn;
}

template <class T>
inline int NRMat3d<T>::dim2() const
{
 return mm;
}

template <class T>
inline int NRMat3d<T>::dim3() const
{
 return kk;
}

template <class T>
NRMat3d<T>::~NRMat3d()
{
 if (v != 0) {
   delete[] (v[0][0]);
   delete[] (v[0]);
   delete[] (v);
   }
}

#endif /* _NR_UTIL_H_ */
