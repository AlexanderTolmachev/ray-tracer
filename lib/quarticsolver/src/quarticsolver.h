/*------------------------------------------------------------------------------
 * quarticsolver.h
 *
 * Class describing a quartic equation (this class was taken from github and slightly modified by removing inheritance)
 *
 * Copyright Jonathan Zrake, NYU CCPP
 *------------------------------------------------------------------------------
 */

#ifndef __QuarticEquation_HEADER__
#define __QuarticEquation_HEADER__

class QuarticEquation
{
private:
  const double d4, d3, d2, d1, d0;

public:
  QuarticEquation(double d4, double d3, double d2, double d1, double d0) :
      d4(d4), d3(d3), d2(d2), d1(d1), d0(d0) { }

  void Function(const double *x, double *y) const
  {
    const double x2 = x[0]*x[0];
    const double x3 = x[0]*x2;
    const double x4 = x[0]*x3;
    y[0] = d4*x4 + d3*x3 + d2*x2 + d1*x[0] + d0;
  }
  void Jacobian(const double *x, double *J) const
  {
    const double x2 = x[0]*x[0];
    const double x3 = x[0]*x2;
    J[0] = 4*d4*x3 + 3*d3*x2 + 2*d2*x[0] + d1;
  }
  int Solve(double *const roots) const;

private:
  void solve_cubic_equation
  (double c3, double c2, double c1, double c0,
   double &x1, double &x2, double &x3, int &nr) const;
};

#endif // __QuarticEquation_HEADER__