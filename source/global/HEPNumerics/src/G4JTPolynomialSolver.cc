//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id: G4JTPolynomialSolver.cc,v 1.1 2005-02-09 15:31:46 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// --------------------------------------------------------------------
// GEANT 4 class source file
//
// G4JTPolynomialSolver
//
// Implementation based on Jenkins-Traub algorithm.
// --------------------------------------------------------------------

#include "G4JTPolynomialSolver.hh"

const G4double G4JTPolynomialSolver::base   = 2;
const G4double G4JTPolynomialSolver::eta    = DBL_EPSILON;
const G4double G4JTPolynomialSolver::infin  = DBL_MAX;
const G4double G4JTPolynomialSolver::smalno = DBL_MIN;
const G4double G4JTPolynomialSolver::are    = DBL_EPSILON;
const G4double G4JTPolynomialSolver::mre    = DBL_EPSILON;
const G4double G4JTPolynomialSolver::lo     = DBL_MIN/DBL_EPSILON ;

G4int G4JTPolynomialSolver::FindRoots(G4double *op, G4int degree,
                                      G4double *zeror, G4double *zeroi) 
{
  G4double t,aa,bb,cc,factor,rot;
  G4double max,min,xx,yy,cosr,sinr,xxx,x,sc,bnd;
  G4double xm,ff,df,dx;
  G4int cnt,nz,i,j,jj,l,nm1,zerok;

  // Initialization of constants for shift rotation.
  //        
  xx = std::sqrt(0.5);
  yy = -xx;
  rot = 94.0*deg;
  cosr = std::cos(rot);
  sinr = std::sin(rot);
  n = degree;
  nz = 0;

  //  Algorithm fails if the leading coefficient is zero.
  //
  if (op[0] == 0.0) return -1;

  //  Remove the zeros at the origin, if any.
  //
  while (op[n] == 0.0) {
    j = degree - n;
    zeror[j] = 0.0;
    zeroi[j] = 0.0;
    n--;
  }
  if (n < 1) return -1;

  // Allocate buffers here
  //
  std::vector<G4double> temp(degree+1) ;
  std::vector<G4double> pt(degree+1) ;

  p.assign(degree+1,0) ;
  qp.assign(degree+1,0) ;
  k.assign(degree+1,0) ;
  qk.assign(degree+1,0) ;
  svk.assign(degree+1,0) ;

  // Make a copy of the coefficients.
  //
  for (i=0;i<=n;i++)
    p[i] = op[i];

  do
  {
    if (n == 1)  // Start the algorithm for one zero.
    {
      zeror[degree-1] = -p[1]/p[0];
      zeroi[degree-1] = 0.0;
      n -= 1;
      return degree - n ;
    }
    if (n == 2)  // Calculate the final zero or pair of zeros.
    {
      Quadratic(p[0],p[1],p[2],&zeror[degree-2],&zeroi[degree-2],
                &zeror[degree-1],&zeroi[degree-1]);
      n -= 2;
      return degree - n ;
    }

    // Find largest and smallest moduli of coefficients.
    //
    max = 0.0;
    min = infin;
    for (i=0;i<=n;i++)
    {
      x = std::fabs(p[i]);
      if (x > max) max = x;
      if (x != 0.0 && x < min) min = x;
    }

    // Scale if there are large or very small coefficients.
    // Computes a scale factor to multiply the coefficients of the
    // polynomial. The scaling is done to avoid overflow and to
    // avoid undetected underflow interfering with the convergence
    // criterion. The factor is a power of the base.
    //
    sc = lo/min;

    if ( sc <= 1.0 && max >= 10.0 
      || sc > 1.0 && infin/sc >= max 
      || infin/sc >= max && max >= 10 )
    {
      if ( sc == 0.0 ) 
        sc = smalno ;
      l = (G4int)(std::log(sc)/std::log(base) + 0.5);
      factor = std::pow(base*1.0,l);
      if (factor != 1.0)
      {
        for (i=0;i<=n;i++) 
          p[i] = factor*p[i];  // Scale polynomial.
      }
    }

    // Compute lower bound on moduli of roots.
    //
    for (i=0;i<=n;i++)
    {
      pt[i] = (std::fabs(p[i]));
    }
    pt[n] = - pt[n];

    // Compute upper estimate of bound.
    //
    x = std::exp((std::log(-pt[n])-std::log(pt[0])) / (G4double)n);

    // If Newton step at the origin is better, use it.
    //
    if (pt[n-1] != 0.0)
    {
      xm = -pt[n]/pt[n-1];
      if (xm < x)  x = xm;
    }

    // Chop the interval (0,x) until ff <= 0
    //
    while (1)
    {
      xm = x*0.1;
      ff = pt[0];
      for (i=1;i<=n;i++) 
        ff = ff*xm + pt[i];
      if (ff <= 0.0) break;
      x = xm;
    }
    dx = x;

    // Do Newton interation until x converges to two decimal places. 
    //
    while (std::fabs(dx/x) > 0.005)
    {
      ff = pt[0];
      df = ff;
      for (i=1;i<n;i++)
      {
        ff = ff*x + pt[i];
        df = df*x + ff;
      }
      ff = ff*x + pt[n];
      dx = ff/df;
      x -= dx;
    }
    bnd = x;

    // Compute the derivative as the initial k polynomial
    // and do 5 steps with no shift.
    //
    nm1 = n - 1;
    for (i=1;i<n;i++)
      k[i] = (G4double)(n-i)*p[i]/(G4double)n;
    k[0] = p[0];
    aa = p[n];
    bb = p[n-1];
    zerok = (k[n-1] == 0);
    for(jj=0;jj<5;jj++)
    {
      cc = k[n-1];
      if (!zerok)  // Use a scaled form of recurrence if k at 0 is nonzero.
      {
        // Use a scaled form of recurrence if value of k at 0 is nonzero.
        //
        t = -aa/cc;
        for (i=0;i<nm1;i++)
        {
          j = n-i-1;
          k[j] = t*k[j-1]+p[j];
        }
        k[0] = p[0];
        zerok = (std::fabs(k[n-1]) <= std::fabs(bb)*eta*10.0);
      }
      else  // Use unscaled form of recurrence.
      {
        for (i=0;i<nm1;i++)
        {
          j = n-i-1;
          k[j] = k[j-1];
        }
        k[0] = 0.0;
        zerok = (k[n-1] == 0.0);
      }
    }

    // Save k for restarts with new shifts.
    //
    for (i=0;i<n;i++) 
      temp[i] = k[i];

    // Loop to select the quadratic corresponding to each new shift.
    //
    for (cnt = 0;cnt < 20;cnt++)
    {
      // Quadratic corresponds to a double shift to a            
      // non-real point and its complex conjugate. The point
      // has modulus bnd and amplitude rotated by 94 degrees
      // from the previous shift.
      //
      xxx = cosr*xx - sinr*yy;
      yy = sinr*xx + cosr*yy;
      xx = xxx;
      sr = bnd*xx;
      si = bnd*yy;
      u = -2.0 * sr;
      v = bnd;
      ComputeFixedShiftPolynomial(20*(cnt+1),&nz);
      if (nz != 0)
      {
        // The second stage jumps directly to one of the third
        // stage iterations and returns here if successful.
        // Deflate the polynomial, store the zero or zeros and
        // return to the main algorithm.
        //
        j = degree - n;
        zeror[j] = szr;
        zeroi[j] = szi;
        n -= nz;
        for (i=0;i<=n;i++)
          p[i] = qp[i];
        if (nz != 1)
        {
          zeror[j+1] = lzr;
          zeroi[j+1] = lzi;
        }
        break;
      }
      else
      {
        // If the iteration is unsuccessful another quadratic
        // is chosen after restoring k.
        //
        for (i=0;i<n;i++)
        {
          k[i] = temp[i];
        }
      }
    }
  }
  while (nz != 0);   // End of initial DO loop

  // Return with failure if no convergence with 20 shifts.
  //
  return degree - n;
}

void G4JTPolynomialSolver::ComputeFixedShiftPolynomial(G4int l2, G4int *nz)
{
  // Computes up to L2 fixed shift k-polynomials, testing for convergence
  // in the linear or quadratic case. Initiates one of the variable shift
  // iterations and returns with the number of zeros found.

  G4double svu,svv,ui,vi,s;
  G4double betas,betav,oss,ovv,ss,vv,ts,tv;
  G4double ots=0.;
  G4double otv=0.;
  G4double tvv,tss;
  G4int type, i,j,iflag,vpass,spass,vtry,stry;

  *nz = 0;
  betav = 0.25;
  betas = 0.25;
  oss = sr;
  ovv = v;

  // Evaluate polynomial by synthetic division.
  //
  QuadraticSyntheticDivision(n,&u,&v,p,qp,&a,&b);
  ComputeScalarFactors(&type);
  for (j=0;j<l2;j++)
  {
    // Calculate next k polynomial and estimate v.
    //
    ComputeNextPolynomial(&type);
    ComputeScalarFactors(&type);
    ComputeNewEstimate(type,&ui,&vi);
    vv = vi;

    // Estimate s.
    //
    ss = 0.0;
    if (k[n-1] != 0.0) ss = -p[n]/k[n-1];
    tv = 1.0;
    ts = 1.0;
    if (j == 0 || type == 3)
    {
      ovv = vv;
      oss = ss;
      otv = tv;
      ots = ts;
      continue;
    }

    // Compute relative measures of convergence of s and v sequences.
    //
    if (vv != 0.0) tv = std::fabs((vv-ovv)/vv);
    if (ss != 0.0) ts = std::fabs((ss-oss)/ss);

    // If decreasing, multiply two most recent convergence measures.
    tvv = 1.0;
    if (tv < otv) tvv = tv*otv;
    tss = 1.0;
    if (ts < ots) tss = ts*ots;

    // Compare with convergence criteria.
    vpass = (tvv < betav);
    spass = (tss < betas);
    if (!(spass || vpass))
    {
      ovv = vv;
      oss = ss;
      otv = tv;
      ots = ts;
      continue;
    }

    // At least one sequence has passed the convergence test.
    // Store variables before iterating.
    //
    svu = u;
    svv = v;
    for (i=0;i<n;i++)
    {
      svk[i] = k[i];
    }
    s = ss;

    // Choose iteration according to the fastest converging sequence.
    //
    vtry = 0;
    stry = 0;
    if (spass && (!vpass) || tss < tvv)
    {
      RealPolynomialIteration(&s,nz,&iflag);
      if (*nz > 0) return;

      // Linear iteration has failed. Flag that it has been
      // tried and decrease the convergence criterion.
      //
      stry = 1;
      betas *=0.25;
      if (iflag == 0) goto _restore_variables;

      // If linear iteration signals an almost double real
      // zero attempt quadratic iteration.
      //
      ui = -(s+s);
      vi = s*s;
    }

_quadratic_iteration:

    do
    {
      QuadraticPolynomialIteration(&ui,&vi,nz);
      if (*nz > 0) return;

      // Quadratic iteration has failed. Flag that it has
      // been tried and decrease the convergence criterion.
      //
      vtry = 1;
      betav *= 0.25;

      // Try linear iteration if it has not been tried and
      // the S sequence is converging.
      //
      if (stry || !spass) break;
      for (i=0;i<n;i++)
      {
        k[i] = svk[i];
      }
      RealPolynomialIteration(&s,nz,&iflag);
      if (*nz > 0) return;

      // Linear iteration has failed. Flag that it has been
      // tried and decrease the convergence criterion.
      //
      stry = 1;
      betas *=0.25;
      if (iflag == 0) break;

      // If linear iteration signals an almost double real
      // zero attempt quadratic iteration.
      //
      ui = -(s+s);
      vi = s*s;
    }
    while (iflag != 0);

    // Restore variables.

_restore_variables:

    u = svu;
    v = svv;
    for (i=0;i<n;i++)
    {
      k[i] = svk[i];
    }

    // Try quadratic iteration if it has not been tried
    // and the V sequence is converging.
    //
    if (vpass && !vtry) goto _quadratic_iteration;

    // Recompute QP and scalar values to continue the
    // second stage.
    //
    QuadraticSyntheticDivision(n,&u,&v,p,qp,&a,&b);
    ComputeScalarFactors(&type);

    ovv = vv;
    oss = ss;
    otv = tv;
    ots = ts;
  }
}

void G4JTPolynomialSolver::
QuadraticPolynomialIteration(G4double *uu, G4double *vv, G4int *nz)
{
  // Variable-shift k-polynomial iteration for a
  // quadratic factor converges only if the zeros are
  // equimodular or nearly so.
  // uu, vv - coefficients of starting quadratic.
  // nz - number of zeros found.
  //
  G4double ui,vi;
  G4double omp=0.;
  G4double relstp=0.;
  G4double mp,ee,t,zm;
  G4int type,i,j,tried;

  *nz = 0;
  tried = 0;
  u = *uu;
  v = *vv;
  j = 0;

  // Main loop.

  while (1)
  {
    Quadratic(1.0,u,v,&szr,&szi,&lzr,&lzi);

    // Return if roots of the quadratic are real and not
    // close to multiple or nearly equal and of opposite
    // sign.
    //
    if (std::fabs(std::fabs(szr)-std::fabs(lzr)) > 0.01 * std::fabs(lzr))
      return;

    // Evaluate polynomial by quadratic synthetic division.
    //
    QuadraticSyntheticDivision(n,&u,&v,p,qp,&a,&b);
    mp = std::fabs(a-szr*b) + std::fabs(szi*b);

    // Compute a rigorous bound on the rounding error in evaluating p.
    //
    zm = std::sqrt(std::fabs(v));
    ee = 2.0*std::fabs(qp[0]);
    t = -szr*b;
    for (i=1;i<n;i++)
    {
      ee = ee*zm + std::fabs(qp[i]);
    }
    ee = ee*zm + std::fabs(a+t);
    ee *= (5.0 *mre + 4.0*are);
    ee = ee - (5.0*mre+2.0*are)*(std::fabs(a+t)+std::fabs(b)*zm)
            + 2.0*are*std::fabs(t);

    // Iteration has converged sufficiently if the
    // polynomial value is less than 20 times this bound.
    //
    if (mp <= 20.0*ee)
    {
      *nz = 2;
      return;
    }
    j++;

    // Stop iteration after 20 steps.
    //
    if (j > 20) return;
    if (j >= 2)
      if (!(relstp > 0.01 || mp < omp || tried))
      {
        // A cluster appears to be stalling the convergence.
        // Five fixed shift steps are taken with a u,v close to the cluster.
        //
        if (relstp < eta) relstp = eta;
        relstp = std::sqrt(relstp);
        u = u - u*relstp;
        v = v + v*relstp;
        QuadraticSyntheticDivision(n,&u,&v,p,qp,&a,&b);
        for (i=0;i<5;i++)
        {
          ComputeScalarFactors(&type);
          ComputeNextPolynomial(&type);
        }
        tried = 1;
        j = 0;
      }
    omp = mp;

    // Calculate next k polynomial and new u and v.
    //
    ComputeScalarFactors(&type);
    ComputeNextPolynomial(&type);
    ComputeScalarFactors(&type);
    ComputeNewEstimate(type,&ui,&vi);

    // If vi is zero the iteration is not converging.
    //
    if (vi == 0.0) return;
    relstp = std::fabs((vi-v)/vi);
    u = ui;
    v = vi;
  }
}

void G4JTPolynomialSolver::
RealPolynomialIteration(G4double *sss, G4int *nz, G4int *iflag)
{
  // Variable-shift H polynomial iteration for a real zero.
  // sss - starting iterate
  // nz  - number of zeros found
  // iflag - flag to indicate a pair of zeros near real axis.

  G4double t=0.;
  G4double omp=0.;
  G4double pv,kv,s;
  G4double ms,mp,ee;
  G4int i,j;

  *nz = 0;
  s = *sss;
  *iflag = 0;
  j = 0;

  // Main loop
  //
  while (1)
  {
    pv = p[0];

    // Evaluate p at s.
    //
    qp[0] = pv;
    for (i=1;i<=n;i++)
    {
      pv = pv*s + p[i];
      qp[i] = pv;
    }
    mp = std::fabs(pv);

    // Compute a rigorous bound on the error in evaluating p.
    //
    ms = std::fabs(s);
    ee = (mre/(are+mre))*std::fabs(qp[0]);
    for (i=1;i<=n;i++)
    {
      ee = ee*ms + std::fabs(qp[i]);
    }

    // Iteration has converged sufficiently if the polynomial
    // value is less than 20 times this bound.
    //
    if (mp <= 20.0*((are+mre)*ee-mre*mp))
    {
      *nz = 1;
      szr = s;
      szi = 0.0;
      return;
    }
    j++;

    // Stop iteration after 10 steps.
    //
    if (j > 10) return;
    if (j >= 2)
      if (!(std::fabs(t) > 0.001*std::fabs(s-t) || mp < omp))
      {
        // A cluster of zeros near the real axis has been encountered.
        // Return with iflag set to initiate a quadratic iteration.
        //
        *iflag = 1;
        *sss = s;
        return;
      }  // Return if the polynomial value has increased significantly.

    omp = mp;

    //  Compute t, the next polynomial, and the new iterate.
    //
    kv = k[0];
    qk[0] = kv;
    for (i=1;i<n;i++)
    {
      kv = kv*s + k[i];
      qk[i] = kv;
    }
    if (std::fabs(kv) <= std::fabs(k[n-1])*10.0*eta)  // Use unscaled form.
    {
      k[0] = 0.0;
      for (i=1;i<n;i++)
      {
        k[i] = qk[i-1];
      }
    }
    else  // Use the scaled form of the recurrence if k at s is nonzero.
    {
      t = -pv/kv;
      k[0] = qp[0];
      for (i=1;i<n;i++)
      {
        k[i] = t*qk[i-1] + qp[i];
      }
    }
    kv = k[0];
    for (i=1;i<n;i++)
    {
      kv = kv*s + k[i];
    }
    t = 0.0;
    if (std::fabs(kv) > std::fabs(k[n-1]*10.0*eta)) t = -pv/kv;
    s += t;
  }
}

void G4JTPolynomialSolver::ComputeScalarFactors(G4int *type)
{
  // This function calculates scalar quantities used to
  // compute the next k polynomial and new estimates of
  // the quadratic coefficients.
  // type - integer variable set here indicating how the
  // calculations are normalized to avoid overflow.

  //  Synthetic division of k by the quadratic 1,u,v
  //
  QuadraticSyntheticDivision(n-1,&u,&v,k,qk,&c,&d);
  if (std::fabs(c) <= std::fabs(k[n-1]*100.0*eta))
    if (std::fabs(d) <= std::fabs(k[n-2]*100.0*eta))
    {
      *type = 3; // Type=3 indicates the quadratic is almost a factor of k.
      return;
    }

  if (std::fabs(d) < std::fabs(c))
  {
    *type = 1;   // Type=1 indicates that all formulas are divided by c.
    e = a/c;
    f = d/c;
    g = u*e;
    h = v*b;
    a3 = a*e + (h/c+g)*b;
    a1 = b - a*(d/c);
    a7 = a + g*d + h*f;
    return;
  }
  *type = 2;     // Type=2 indicates that all formulas are divided by d.
  e = a/d;
  f = c/d;
  g = u*b;
  h = v*b;
  a3 = (a+g)*e + h*(b/d);
  a1 = b*f-a;
  a7 = (f+u)*a + h;
}

void G4JTPolynomialSolver::ComputeNextPolynomial(G4int *type)
{
  // Computes the next k polynomials using scalars 
  // computed in ComputeScalarFactors.

  G4double temp;
  G4int i;

  if (*type == 3)  // Use unscaled form of the recurrence if type is 3.
  {
    k[0] = 0.0;
    k[1] = 0.0;
    for (i=2;i<n;i++)
    {
      k[i] = qk[i-2];
    }
    return;
  }
  temp = a;
  if (*type == 1) temp = b;
  if (std::fabs(a1) <= std::fabs(temp)*eta*10.0)
  {
    // If a1 is nearly zero then use a special form of the recurrence.
    //
    k[0] = 0.0;
    k[1] = -a7*qp[0];
    for(i=2;i<n;i++)
    {
      k[i] = a3*qk[i-2] - a7*qp[i-1];
    }
    return;
  }

  // Use scaled form of the recurrence.
  //
  a7 /= a1;
  a3 /= a1;
  k[0] = qp[0];
  k[1] = qp[1] - a7*qp[0];
  for (i=2;i<n;i++)
  {
    k[i] = a3*qk[i-2] - a7*qp[i-1] + qp[i];
  }
}

void G4JTPolynomialSolver::
ComputeNewEstimate(G4int type, G4double *uu, G4double *vv)
{
  // Compute new estimates of the quadratic coefficients
  // using the scalars computed in calcsc.

  G4double a4,a5,b1,b2,c1,c2,c3,c4,temp;

  // Use formulas appropriate to setting of type.
  //
  if (type == 3)    //  If type=3 the quadratic is zeroed.
  {
    *uu = 0.0;
    *vv = 0.0;
    return;
  }
  if (type == 2)
  {
    a4 = (a+g)*f + h;
    a5 = (f+u)*c + v*d;
  }
  else
  {
    a4 = a + u*b +h*f;
    a5 = c + (u+v*f)*d;
  }

  //  Evaluate new quadratic coefficients.
  //
  b1 = -k[n-1]/p[n];
  b2 = -(k[n-2]+b1*p[n-1])/p[n];
  c1 = v*b2*a1;
  c2 = b1*a7;
  c3 = b1*b1*a3;
  c4 = c1 - c2 - c3;
  temp = a5 + b1*a4 - c4;
  if (temp == 0.0)
  {
    *uu = 0.0;
    *vv = 0.0;
    return;
  }
  *uu = u - (u*(c3+c2)+v*(b1*a1+b2*a7))/temp;
  *vv = v*(1.0+c4/temp);
  return;
}

void G4JTPolynomialSolver::
QuadraticSyntheticDivision(G4int nn, G4double *u, G4double *v,
                           std::vector<G4double> &p, std::vector<G4double> &q,  
                           G4double *a, G4double *b)
{
  // Divides p by the quadratic 1,u,v placing the quotient
  // in q and the remainder in a,b.

  G4double c;
  G4int i;
  *b = p[0];
  q[0] = *b;
  *a = p[1] - (*b)*(*u);
  q[1] = *a;
  for (i=2;i<=nn;i++)
  {
    c = p[i] - (*a)*(*u) - (*b)*(*v);
    q[i] = c;
    *b = *a;
    *a = c;
  }
}

void G4JTPolynomialSolver::Quadratic(G4double a,G4double b1,
                                     G4double c,G4double *sr,G4double *si,
                                     G4double *lr,G4double *li)
{

  // Calculate the zeros of the quadratic a*z^2 + b1*z + c.
  // The quadratic formula, modified to avoid overflow, is used 
  // to find the larger zero if the zeros are real and both
  // are complex. The smaller real zero is found directly from 
  // the product of the zeros c/a.

  G4double b,d,e;

  if (a == 0.0)     // less than two roots
  {
    if (b1 != 0.0)     
      *sr = -c/b1;
    else 
      *sr = 0.0;
    *lr = 0.0;
    *si = 0.0;
    *li = 0.0;
    return;
  }
  if (c == 0.0)     // one real root, one zero root
  {
    *sr = 0.0;
    *lr = -b1/a;
    *si = 0.0;
    *li = 0.0;
    return;
  }

  // Compute discriminant avoiding overflow.
  //
  b = b1/2.0;
  if (std::fabs(b) < std::fabs(c))
  { 
    if (c < 0.0) 
      e = -a;
    else
      e = a;
    e = b*(b/std::fabs(c)) - e;
    d = std::sqrt(std::fabs(e))*std::sqrt(std::fabs(c));
  }
  else
  {
    e = 1.0 - (a/b)*(c/b);
    d = std::sqrt(std::fabs(e))*std::fabs(b);
  }
  if (e < 0.0)      // complex conjugate zeros
  {
    *sr = -b/a;
    *lr = *sr;
    *si = std::fabs(d/a);
    *li = -(*si);
  }
  else
  {
    if (b >= 0.0)   // real zeros.
      d = -d;
    *lr = (-b+d)/a;
    *sr = 0.0;
    if (*lr != 0.0) 
      *sr = (c/ *lr)/a;
    *si = 0.0;
    *li = 0.0;
  }
}
