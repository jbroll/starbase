      SUBROUTINE sla_SVD (M, N, MP, NP, A, W, V, WORK, JSTAT)
*+
*     - - - -
*      S V D
*     - - - -
*
*  Singular value decomposition  (double precision)
*
*  This routine expresses a given matrix A as the product of
*  three matrices U, W, V:
*
*     A = U x W x VT
*
*  Where:
*
*     A   is any M (rows) x N (columns) matrix, where M.GE.N
*     U   is an M x N column-orthogonal matrix
*     W   is an N x N diagonal matrix with W(I,I).GE.0
*     VT  is the transpose of an N x N orthogonal matrix
*
*     Note that M and N, above, are the LOGICAL dimensions of the
*     matrices and vectors concerned, which can be located in
*     arrays of larger PHYSICAL dimensions, given by MP and NP.
*
*  Given:
*     M,N    i         numbers of rows and columns in matrix A
*     MP,NP  i         physical dimensions of array containing matrix A
*     A      d(MP,NP)  array containing MxN matrix A
*
*  Returned:
*     A      d(MP,NP)  array containing MxN column-orthogonal matrix U
*     W      d(N)      NxN diagonal matrix W (diagonal elements only)
*     V      d(NP,NP)  array containing NxN orthogonal matrix V
*     WORK   d(N)      workspace
*     JSTAT  i         0 = OK, -1 = A wrong shape, >0 = index of W
*                      for which convergence failed.  See note 2, below.
*
*   Notes:
*
*   1)  V contains matrix V, not the transpose of matrix V.
*
*   2)  If the status JSTAT is greater than zero, this need not
*       necessarily be treated as a failure.  It means that, due to
*       chance properties of the matrix A, the QR transformation
*       phase of the routine did not fully converge in a predefined
*       number of iterations, something that very seldom occurs.
*       When this condition does arise, it is possible that the
*       elements of the diagonal matrix W have not been correctly
*       found.  However, in practice the results are likely to
*       be trustworthy.  Applications should report the condition
*       as a warning, but then proceed normally.
*
*  References:
*     The algorithm is an adaptation of the routine SVD in the EISPACK
*     library (Garbow et al 1977, EISPACK Guide Extension, Springer
*     Verlag), which is a FORTRAN 66 implementation of the Algol
*     routine SVD of Wilkinson & Reinsch 1971 (Handbook for Automatic
*     Computation, vol 2, ed Bauer et al, Springer Verlag).  These
*     references give full details of the algorithm used here.  A good
*     account of the use of SVD in least squares problems is given in
*     Numerical Recipes (Press et al 1986, Cambridge University Press),
*     which includes another variant of the EISPACK code.
*
*  Last revision:   8 September 2005
*
*  Copyright P.T.Wallace.  All rights reserved.
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      INTEGER M,N,MP,NP
      DOUBLE PRECISION A(MP,NP),W(N),V(NP,NP),WORK(N)
      INTEGER JSTAT

*  Maximum number of iterations in QR phase
      INTEGER ITMAX
      PARAMETER (ITMAX=30)

      INTEGER L,L1,I,K,J,K1,ITS,I1
      LOGICAL CANCEL
      DOUBLE PRECISION G,SCALE,AN,S,X,F,H,C,Y,Z



*  Variable initializations to avoid compiler warnings.
      L = 0
      L1 = 0

*  Check that the matrix is the right shape
      IF (M.LT.N) THEN

*     No:  error status
         JSTAT = -1

      ELSE

*     Yes:  preset the status to OK
         JSTAT = 0

*
*     Householder reduction to bidiagonal form
*     ----------------------------------------

         G = 0D0
         SCALE = 0D0
         AN = 0D0
         DO I=1,N
            L = I+1
            WORK(I) = SCALE*G
            G = 0D0
            S = 0D0
            SCALE = 0D0
            IF (I.LE.M) THEN
               DO K=I,M
                  SCALE = SCALE+ABS(A(K,I))
               END DO
               IF (SCALE.NE.0D0) THEN
                  DO K=I,M
                     X = A(K,I)/SCALE
                     A(K,I) = X
                     S = S+X*X
                  END DO
                  F = A(I,I)
                  G = -SIGN(SQRT(S),F)
                  H = F*G-S
                  A(I,I) = F-G
                  IF (I.NE.N) THEN
                     DO J=L,N
                        S = 0D0
                        DO K=I,M
                           S = S+A(K,I)*A(K,J)
                        END DO
                        F = S/H
                        DO K=I,M
                           A(K,J) = A(K,J)+F*A(K,I)
                        END DO
                     END DO
                  END IF
                  DO K=I,M
                     A(K,I) = SCALE*A(K,I)
                  END DO
               END IF
            END IF
            W(I) = SCALE*G
            G = 0D0
            S = 0D0
            SCALE = 0D0
            IF (I.LE.M .AND. I.NE.N) THEN
               DO K=L,N
                  SCALE = SCALE+ABS(A(I,K))
               END DO
               IF (SCALE.NE.0D0) THEN
                  DO K=L,N
                     X = A(I,K)/SCALE
                     A(I,K) = X
                     S = S+X*X
                  END DO
                  F = A(I,L)
                  G = -SIGN(SQRT(S),F)
                  H = F*G-S
                  A(I,L) = F-G
                  DO K=L,N
                     WORK(K) = A(I,K)/H
                  END DO
                  IF (I.NE.M) THEN
                     DO J=L,M
                        S = 0D0
                        DO K=L,N
                           S = S+A(J,K)*A(I,K)
                        END DO
                        DO K=L,N
                           A(J,K) = A(J,K)+S*WORK(K)
                        END DO
                     END DO
                  END IF
                  DO K=L,N
                     A(I,K) = SCALE*A(I,K)
                  END DO
               END IF
            END IF

*        Overestimate of largest column norm for convergence test
            AN = MAX(AN,ABS(W(I))+ABS(WORK(I)))

         END DO

*
*     Accumulation of right-hand transformations
*     ------------------------------------------

         DO I=N,1,-1
            IF (I.NE.N) THEN
               IF (G.NE.0D0) THEN
                  DO J=L,N
                     V(J,I) = (A(I,J)/A(I,L))/G
                  END DO
                  DO J=L,N
                     S = 0D0
                     DO K=L,N
                        S = S+A(I,K)*V(K,J)
                     END DO
                     DO K=L,N
                        V(K,J) = V(K,J)+S*V(K,I)
                     END DO
                  END DO
               END IF
               DO J=L,N
                  V(I,J) = 0D0
                  V(J,I) = 0D0
               END DO
            END IF
            V(I,I) = 1D0
            G = WORK(I)
            L = I
         END DO

*
*     Accumulation of left-hand transformations
*     -----------------------------------------

         DO I=N,1,-1
            L = I+1
            G = W(I)
            IF (I.NE.N) THEN
               DO J=L,N
                  A(I,J) = 0D0
               END DO
            END IF
            IF (G.NE.0D0) THEN
               IF (I.NE.N) THEN
                  DO J=L,N
                     S = 0D0
                     DO K=L,M
                        S = S+A(K,I)*A(K,J)
                     END DO
                     F = (S/A(I,I))/G
                     DO K=I,M
                        A(K,J) = A(K,J)+F*A(K,I)
                     END DO
                  END DO
               END IF
               DO J=I,M
                  A(J,I) = A(J,I)/G
               END DO
            ELSE
               DO J=I,M
                  A(J,I) = 0D0
               END DO
            END IF
            A(I,I) = A(I,I)+1D0
         END DO

*
*     Diagonalisation of the bidiagonal form
*     --------------------------------------

         DO K=N,1,-1
            K1 = K-1

*        Iterate until converged
            ITS = 0
            DO WHILE (ITS.LT.ITMAX)
               ITS = ITS+1

*           Test for splitting into submatrices
               CANCEL = .TRUE.
               DO L=K,1,-1
                  L1 = L-1
                  IF (AN+ABS(WORK(L)).EQ.AN) THEN
                     CANCEL = .FALSE.
                     GO TO 10
                  END IF
*              (Following never attempted for L=1 because WORK(1) is zero)
                  IF (AN+ABS(W(L1)).EQ.AN) GO TO 10
               END DO
 10            CONTINUE

*           Cancellation of WORK(L) if L>1
               IF (CANCEL) THEN
                  C = 0D0
                  S = 1D0
                  DO I=L,K
                     F = S*WORK(I)
                     IF (AN+ABS(F).EQ.AN) GO TO 20
                     G = W(I)
                     H = SQRT(F*F+G*G)
                     W(I) = H
                     C = G/H
                     S = -F/H
                     DO J=1,M
                        Y = A(J,L1)
                        Z = A(J,I)
                        A(J,L1) = Y*C+Z*S
                        A(J,I) = -Y*S+Z*C
                     END DO
                  END DO
 20               CONTINUE
               END IF

*           Converged?
               Z = W(K)
               IF (L.EQ.K) THEN

*              Yes:  stop iterating
                  ITS = ITMAX

*              Ensure singular values non-negative
                  IF (Z.LT.0D0) THEN
                     W(K) = -Z
                     DO J=1,N
                        V(J,K) = -V(J,K)
                     END DO
                  END IF
               ELSE

*              Not converged yet:  set status if iteration limit reached
                  IF (ITS.EQ.ITMAX) JSTAT = K

*              Shift from bottom 2x2 minor
                  X = W(L)
                  Y = W(K1)
                  G = WORK(K1)
                  H = WORK(K)
                  F = ((Y-Z)*(Y+Z)+(G-H)*(G+H))/(2D0*H*Y)
                  IF (ABS(F).LE.1D15) THEN
                     G = SQRT(F*F+1D0)
                  ELSE
                     G = ABS(F)
                  END IF
                  F = ((X-Z)*(X+Z)+H*(Y/(F+SIGN(G,F))-H))/X

*              Next QR transformation
                  C = 1D0
                  S = 1D0
                  DO I1=L,K1
                     I = I1+1
                     G = WORK(I)
                     Y = W(I)
                     H = S*G
                     G = C*G
                     Z = SQRT(F*F+H*H)
                     WORK(I1) = Z
                     IF (Z.NE.0D0) THEN
                        C = F/Z
                        S = H/Z
                     ELSE
                        C = 1D0
                        S = 0D0
                     END IF
                     F = X*C+G*S
                     G = -X*S+G*C
                     H = Y*S
                     Y = Y*C
                     DO J=1,N
                        X = V(J,I1)
                        Z = V(J,I)
                        V(J,I1) = X*C+Z*S
                        V(J,I) = -X*S+Z*C
                     END DO
                     Z = SQRT(F*F+H*H)
                     W(I1) = Z
                     IF (Z.NE.0D0) THEN
                        C = F/Z
                        S = H/Z
                     END IF
                     F = C*G+S*Y
                     X = -S*G+C*Y
                     DO J=1,M
                        Y = A(J,I1)
                        Z = A(J,I)
                        A(J,I1) = Y*C+Z*S
                        A(J,I) = -Y*S+Z*C
                     END DO
                  END DO
                  WORK(L) = 0D0
                  WORK(K) = F
                  W(K) = X
               END IF
            END DO
         END DO
      END IF

      END

      SUBROUTINE sla_SVDCOV (N, NP, NC, W, V, WORK, CVM)
*+
*     - - - - - - -
*      S V D C O V
*     - - - - - - -
*
*  From the W and V matrices from the SVD factorisation of a matrix
*  (as obtained from the sla_SVD routine), obtain the covariance matrix.
*
*  (double precision)
*
*  Given:
*     N      i         number of rows and columns in matrices W and V
*     NP     i         first dimension of array containing matrix V
*     NC     i         first dimension of array to receive CVM
*     W      d(N)      NxN diagonal matrix W (diagonal elements only)
*     V      d(NP,NP)  array containing NxN orthogonal matrix V
*
*  Returned:
*     WORK   d(N)      workspace
*     CVM    d(NC,NC)  array to receive covariance matrix
*
*  Reference:
*     Numerical Recipes, section 14.3.
*
*  P.T.Wallace   Starlink   December 1988
*
*  Copyright (C) 1995 Rutherford Appleton Laboratory
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      INTEGER N,NP,NC
      DOUBLE PRECISION W(N),V(NP,NP),WORK(N),CVM(NC,NC)

      INTEGER I,J,K
      DOUBLE PRECISION S



      DO I=1,N
         S=W(I)
         IF (S.NE.0D0) THEN
            WORK(I)=1D0/(S*S)
         ELSE
            WORK(I)=0D0
         END IF
      END DO
      DO I=1,N
         DO J=1,I
            S=0D0
            DO K=1,N
               S=S+V(I,K)*V(J,K)*WORK(K)
            END DO
            CVM(I,J)=S
            CVM(J,I)=S
         END DO
      END DO

      END

      SUBROUTINE sla_SVDSOL (M, N, MP, NP, B, U, W, V, WORK, X)
*+
*     - - - - - - -
*      S V D S O L
*     - - - - - - -
*
*  From a given vector and the SVD of a matrix (as obtained from
*  the SVD routine), obtain the solution vector (double precision)
*
*  This routine solves the equation:
*
*     A . x = b
*
*  where:
*
*     A   is a given M (rows) x N (columns) matrix, where M.GE.N
*     x   is the N-vector we wish to find
*     b   is a given M-vector
*
*  by means of the Singular Value Decomposition method (SVD).  In
*  this method, the matrix A is first factorised (for example by
*  the routine sla_SVD) into the following components:
*
*     A = U x W x VT
*
*  where:
*
*     A   is the M (rows) x N (columns) matrix
*     U   is an M x N column-orthogonal matrix
*     W   is an N x N diagonal matrix with W(I,I).GE.0
*     VT  is the transpose of an NxN orthogonal matrix
*
*     Note that M and N, above, are the LOGICAL dimensions of the
*     matrices and vectors concerned, which can be located in
*     arrays of larger PHYSICAL dimensions MP and NP.
*
*  The solution is found from the expression:
*
*     x = V . [diag(1/Wj)] . (transpose(U) . b)
*
*  Notes:
*
*  1)  If matrix A is square, and if the diagonal matrix W is not
*      adjusted, the method is equivalent to conventional solution
*      of simultaneous equations.
*
*  2)  If M>N, the result is a least-squares fit.
*
*  3)  If the solution is poorly determined, this shows up in the
*      SVD factorisation as very small or zero Wj values.  Where
*      a Wj value is small but non-zero it can be set to zero to
*      avoid ill effects.  The present routine detects such zero
*      Wj values and produces a sensible solution, with highly
*      correlated terms kept under control rather than being allowed
*      to elope to infinity, and with meaningful values for the
*      other terms.
*
*  Given:
*     M,N    i         numbers of rows and columns in matrix A
*     MP,NP  i         physical dimensions of array containing matrix A
*     B      d(M)      known vector b
*     U      d(MP,NP)  array containing MxN matrix U
*     W      d(N)      NxN diagonal matrix W (diagonal elements only)
*     V      d(NP,NP)  array containing NxN orthogonal matrix V
*
*  Returned:
*     WORK   d(N)      workspace
*     X      d(N)      unknown vector x
*
*  Reference:
*     Numerical Recipes, section 2.9.
*
*  P.T.Wallace   Starlink   29 October 1993
*
*  Copyright (C) 1995 Rutherford Appleton Laboratory
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      INTEGER M,N,MP,NP
      DOUBLE PRECISION B(M),U(MP,NP),W(N),V(NP,NP),WORK(N),X(N)

      INTEGER J,I,JJ
      DOUBLE PRECISION S



*  Calculate [diag(1/Wj)] . transpose(U) . b (or zero for zero Wj)
      DO J=1,N
         S=0D0
         IF (W(J).NE.0D0) THEN
            DO I=1,M
               S=S+U(I,J)*B(I)
            END DO
            S=S/W(J)
         END IF
         WORK(J)=S
      END DO

*  Multiply by matrix V to get result
      DO J=1,N
         S=0D0
         DO JJ=1,N
            S=S+V(J,JJ)*WORK(JJ)
         END DO
         X(J)=S
      END DO

      END

      SUBROUTINE sla_FITXY (ITYPE,NP,XYE,XYM,COEFFS,J)
*+
*     - - - - - -
*      F I T X Y
*     - - - - - -
*
*  Fit a linear model to relate two sets of [X,Y] coordinates.
*
*  Given:
*     ITYPE    i        type of model: 4 or 6 (note 1)
*     NP       i        number of samples (note 2)
*     XYE     d(2,np)   expected [X,Y] for each sample
*     XYM     d(2,np)   measured [X,Y] for each sample
*
*  Returned:
*     COEFFS  d(6)      coefficients of model (note 3)
*     J        i        status:  0 = OK
*                               -1 = illegal ITYPE
*                               -2 = insufficient data
*                               -3 = no solution
*
*  Notes:
*
*  1)  ITYPE, which must be either 4 or 6, selects the type of model
*      fitted.  Both allowed ITYPE values produce a model COEFFS which
*      consists of six coefficients, namely the zero points and, for
*      each of XE and YE, the coefficient of XM and YM.  For ITYPE=6,
*      all six coefficients are independent, modelling squash and shear
*      as well as origin, scale, and orientation.  However, ITYPE=4
*      selects the "solid body rotation" option;  the model COEFFS
*      still consists of the same six coefficients, but now two of
*      them are used twice (appropriately signed).  Origin, scale
*      and orientation are still modelled, but not squash or shear -
*      the units of X and Y have to be the same.
*
*  2)  For NC=4, NP must be at least 2.  For NC=6, NP must be at
*      least 3.
*
*  3)  The model is returned in the array COEFFS.  Naming the
*      elements of COEFFS as follows:
*
*                  COEFFS(1) = A
*                  COEFFS(2) = B
*                  COEFFS(3) = C
*                  COEFFS(4) = D
*                  COEFFS(5) = E
*                  COEFFS(6) = F
*
*      the model is:
*
*            XE = A + B*XM + C*YM
*            YE = D + E*XM + F*YM
*
*      For the "solid body rotation" option (ITYPE=4), the
*      magnitudes of B and F, and of C and E, are equal.  The
*      signs of these coefficients depend on whether there is a
*      sign reversal between XE,YE and XM,YM;  fits are performed
*      with and without a sign reversal and the best one chosen.
*
*  4)  Error status values J=-1 and -2 leave COEFFS unchanged;
*      if J=-3 COEFFS may have been changed.
*
*  See also sla_PXY, sla_INVF, sla_XY2XY, sla_DCMPF
*
*  Called:  sla_DMAT, sla_DMXV
*
*  Last revision:   8 September 2005
*
*  Copyright P.T.Wallace.  All rights reserved.
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      INTEGER ITYPE,NP
      DOUBLE PRECISION XYE(2,NP),XYM(2,NP),COEFFS(6)
      INTEGER J

      INTEGER I,JSTAT,IW(4),NSOL
      DOUBLE PRECISION A,B,C,D,AOLD,BOLD,COLD,DOLD,SOLD,
     :                 P,SXE,SXEXM,SXEYM,SYE,SYEYM,SYEXM,SXM,
     :                 SYM,SXMXM,SXMYM,SYMYM,XE,YE,
     :                 XM,YM,V(4),DM3(3,3),DM4(4,4),DET,
     :                 SGN,SXXYY,SXYYX,SX2Y2,SDR2,XR,YR



*  Preset the status
      J=0

*  Variable initializations to avoid compiler warnings
      A = 0D0
      B = 0D0
      C = 0D0
      D = 0D0
      AOLD = 0D0
      BOLD = 0D0
      COLD = 0D0
      DOLD = 0D0
      SOLD = 0D0

*  Float the number of samples
      P=DBLE(NP)

*  Check ITYPE
      IF (ITYPE.EQ.6) THEN

*
*     Six-coefficient linear model
*     ----------------------------

*     Check enough samples
         IF (NP.GE.3) THEN

*     Form summations
         SXE=0D0
         SXEXM=0D0
         SXEYM=0D0
         SYE=0D0
         SYEYM=0D0
         SYEXM=0D0
         SXM=0D0
         SYM=0D0
         SXMXM=0D0
         SXMYM=0D0
         SYMYM=0D0
         DO I=1,NP
            XE=XYE(1,I)
            YE=XYE(2,I)
            XM=XYM(1,I)
            YM=XYM(2,I)
            SXE=SXE+XE
            SXEXM=SXEXM+XE*XM
            SXEYM=SXEYM+XE*YM
            SYE=SYE+YE
            SYEYM=SYEYM+YE*YM
            SYEXM=SYEXM+YE*XM
            SXM=SXM+XM
            SYM=SYM+YM
            SXMXM=SXMXM+XM*XM
            SXMYM=SXMYM+XM*YM
            SYMYM=SYMYM+YM*YM
         END DO

*        Solve for A,B,C in  XE = A + B*XM + C*YM
            V(1)=SXE
            V(2)=SXEXM
            V(3)=SXEYM
            DM3(1,1)=P
            DM3(1,2)=SXM
            DM3(1,3)=SYM
            DM3(2,1)=SXM
            DM3(2,2)=SXMXM
            DM3(2,3)=SXMYM
            DM3(3,1)=SYM
            DM3(3,2)=SXMYM
            DM3(3,3)=SYMYM
            CALL sla_DMAT(3,DM3,V,DET,JSTAT,IW)
            IF (JSTAT.EQ.0) THEN
               DO I=1,3
                  COEFFS(I)=V(I)
               END DO

*           Solve for D,E,F in  YE = D + E*XM + F*YM
               V(1)=SYE
               V(2)=SYEXM
               V(3)=SYEYM
               CALL sla_DMXV(DM3,V,COEFFS(4))

            ELSE

*           No 6-coefficient solution possible
               J=-3

            END IF

         ELSE

*        Insufficient data for 6-coefficient fit
            J=-2

         END IF

      ELSE IF (ITYPE.EQ.4) THEN

*
*     Four-coefficient solid body rotation model
*     ------------------------------------------

*     Check enough samples
         IF (NP.GE.2) THEN

*        Try two solutions, first without then with flip in X
            DO NSOL=1,2
               IF (NSOL.EQ.1) THEN
                  SGN=1D0
               ELSE
                  SGN=-1D0
               END IF

*           Form summations
               SXE=0D0
               SXXYY=0D0
               SXYYX=0D0
               SYE=0D0
               SXM=0D0
               SYM=0D0
               SX2Y2=0D0
               DO I=1,NP
                  XE=XYE(1,I)*SGN
                  YE=XYE(2,I)
                  XM=XYM(1,I)
                  YM=XYM(2,I)
                  SXE=SXE+XE
                  SXXYY=SXXYY+XE*XM+YE*YM
                  SXYYX=SXYYX+XE*YM-YE*XM
                  SYE=SYE+YE
                  SXM=SXM+XM
                  SYM=SYM+YM
                  SX2Y2=SX2Y2+XM*XM+YM*YM
               END DO

*
*           Solve for A,B,C,D in:  +/- XE = A + B*XM - C*YM
*                                    + YE = D + C*XM + B*YM
               V(1)=SXE
               V(2)=SXXYY
               V(3)=SXYYX
               V(4)=SYE
               DM4(1,1)=P
               DM4(1,2)=SXM
               DM4(1,3)=-SYM
               DM4(1,4)=0D0
               DM4(2,1)=SXM
               DM4(2,2)=SX2Y2
               DM4(2,3)=0D0
               DM4(2,4)=SYM
               DM4(3,1)=SYM
               DM4(3,2)=0D0
               DM4(3,3)=-SX2Y2
               DM4(3,4)=-SXM
               DM4(4,1)=0D0
               DM4(4,2)=SYM
               DM4(4,3)=SXM
               DM4(4,4)=P
               CALL sla_DMAT(4,DM4,V,DET,JSTAT,IW)
               IF (JSTAT.EQ.0) THEN
                  A=V(1)
                  B=V(2)
                  C=V(3)
                  D=V(4)

*              Determine sum of radial errors squared
                  SDR2=0D0
                  DO I=1,NP
                     XM=XYM(1,I)
                     YM=XYM(2,I)
                     XR=A+B*XM-C*YM-XYE(1,I)*SGN
                     YR=D+C*XM+B*YM-XYE(2,I)
                     SDR2=SDR2+XR*XR+YR*YR
                  END DO

               ELSE

*              Singular: set flag
                  SDR2=-1D0

               END IF

*           If first pass and non-singular, save variables
               IF (NSOL.EQ.1.AND.JSTAT.EQ.0) THEN
                  AOLD=A
                  BOLD=B
                  COLD=C
                  DOLD=D
                  SOLD=SDR2
               END IF

            END DO

*        Pick the best of the two solutions
            IF (SOLD.GE.0D0.AND.(SOLD.LE.SDR2.OR.NP.EQ.2)) THEN
               COEFFS(1)=AOLD
               COEFFS(2)=BOLD
               COEFFS(3)=-COLD
               COEFFS(4)=DOLD
               COEFFS(5)=COLD
               COEFFS(6)=BOLD
            ELSE IF (JSTAT.EQ.0) THEN
               COEFFS(1)=-A
               COEFFS(2)=-B
               COEFFS(3)=C
               COEFFS(4)=D
               COEFFS(5)=C
               COEFFS(6)=B
            ELSE

*           No 4-coefficient fit possible
               J=-3
            END IF
         ELSE

*        Insufficient data for 4-coefficient fit
            J=-2
         END IF
      ELSE

*     Illegal ITYPE - not 4 or 6
         J=-1
      END IF

      END

      SUBROUTINE sla_XY2XY (X1,Y1,COEFFS,X2,Y2)
*+
*     - - - - - -
*      X Y 2 X Y
*     - - - - - -
*
*  Transform one [X,Y] into another using a linear model of the type
*  produced by the sla_FITXY routine.
*
*  Given:
*     X1       d        x-coordinate
*     Y1       d        y-coordinate
*     COEFFS  d(6)      transformation coefficients (see note)
*
*  Returned:
*     X2       d        x-coordinate
*     Y2       d        y-coordinate
*
*  The model relates two sets of [X,Y] coordinates as follows.
*  Naming the elements of COEFFS:
*
*     COEFFS(1) = A
*     COEFFS(2) = B
*     COEFFS(3) = C
*     COEFFS(4) = D
*     COEFFS(5) = E
*     COEFFS(6) = F
*
*  the present routine performs the transformation:
*
*     X2 = A + B*X1 + C*Y1
*     Y2 = D + E*X1 + F*Y1
*
*  See also sla_FITXY, sla_PXY, sla_INVF, sla_DCMPF
*
*  P.T.Wallace   Starlink   5 December 1994
*
*  Copyright (C) 1995 Rutherford Appleton Laboratory
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      DOUBLE PRECISION X1,Y1,COEFFS(6),X2,Y2


      X2=COEFFS(1)+COEFFS(2)*X1+COEFFS(3)*Y1
      Y2=COEFFS(4)+COEFFS(5)*X1+COEFFS(6)*Y1

      END


      SUBROUTINE sla_DMXV (DM, VA, VB)
*+
*     - - - - -
*      D M X V
*     - - - - -
*
*  Performs the 3-D forward unitary transformation:
*
*     vector VB = matrix DM * vector VA
*
*  (double precision)
*
*  Given:
*     DM       dp(3,3)    matrix
*     VA       dp(3)      vector
*
*  Returned:
*     VB       dp(3)      result vector
*
*  To comply with the ANSI Fortran 77 standard, VA and VB must be
*  different arrays.  However, the routine is coded so as to work
*  properly on many platforms even if this rule is violated.
*
*  Last revision:   26 December 2004
*
*  Copyright P.T.Wallace.  All rights reserved.
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      DOUBLE PRECISION DM(3,3),VA(3),VB(3)

      INTEGER I,J
      DOUBLE PRECISION W,VW(3)


*  Matrix DM * vector VA -> vector VW
      DO J=1,3
         W=0D0
         DO I=1,3
            W=W+DM(J,I)*VA(I)
         END DO
         VW(J)=W
      END DO

*  Vector VW -> vector VB
      DO J=1,3
         VB(J)=VW(J)
      END DO

      END


      SUBROUTINE sla_DMAT (N, A, Y, D, JF, IW)
*+
*     - - - - -
*      D M A T
*     - - - - -
*
*  Matrix inversion & solution of simultaneous equations
*  (double precision)
*
*  For the set of n simultaneous equations in n unknowns:
*     A.Y = X
*
*  where:
*     A is a non-singular N x N matrix
*     Y is the vector of N unknowns
*     X is the known vector
*
*  DMATRX computes:
*     the inverse of matrix A
*     the determinant of matrix A
*     the vector of N unknowns
*
*  Arguments:
*
*     symbol  type   dimension           before              after
*
*       N      i                    no. of unknowns       unchanged
*       A      d      (N,N)             matrix             inverse
*       Y      d       (N)            known vector      solution vector
*       D      d                           -             determinant
*     * JF     i                           -           singularity flag
*       IW     i       (N)                 -              workspace
*
*  * JF is the singularity flag.  If the matrix is non-singular, JF=0
*    is returned.  If the matrix is singular, JF=-1 & D=0D0 are
*    returned.  In the latter case, the contents of array A on return
*    are undefined.
*
*  Algorithm:
*     Gaussian elimination with partial pivoting.
*
*  Speed:
*     Very fast.
*
*  Accuracy:
*     Fairly accurate - errors 1 to 4 times those of routines optimized
*     for accuracy.
*
*  P.T.Wallace   Starlink   4 December 2001
*
*  Copyright (C) 2001 Rutherford Appleton Laboratory
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      INTEGER N
      DOUBLE PRECISION A(N,N),Y(N),D
      INTEGER JF
      INTEGER IW(N)

      DOUBLE PRECISION SFA
      PARAMETER (SFA=1D-20)

      INTEGER K,IMX,I,J,NP1MK,KI
      DOUBLE PRECISION AMX,T,AKK,YK,AIK


      JF=0
      D=1D0
      DO K=1,N
         AMX=DABS(A(K,K))
         IMX=K
         IF (K.NE.N) THEN
            DO I=K+1,N
               T=DABS(A(I,K))
               IF (T.GT.AMX) THEN
                  AMX=T
                  IMX=I
               END IF
            END DO
         END IF
         IF (AMX.LT.SFA) THEN
            JF=-1
         ELSE
            IF (IMX.NE.K) THEN
               DO J=1,N
                  T=A(K,J)
                  A(K,J)=A(IMX,J)
                  A(IMX,J)=T
               END DO
               T=Y(K)
               Y(K)=Y(IMX)
               Y(IMX)=T
               D=-D
            END IF
            IW(K)=IMX
            AKK=A(K,K)
            D=D*AKK
            IF (DABS(D).LT.SFA) THEN
               JF=-1
            ELSE
               AKK=1D0/AKK
               A(K,K)=AKK
               DO J=1,N
                  IF (J.NE.K) A(K,J)=A(K,J)*AKK
               END DO
               YK=Y(K)*AKK
               Y(K)=YK
               DO I=1,N
                  AIK=A(I,K)
                  IF (I.NE.K) THEN
                     DO J=1,N
                        IF (J.NE.K) A(I,J)=A(I,J)-AIK*A(K,J)
                     END DO
                     Y(I)=Y(I)-AIK*YK
                  END IF
               END DO
               DO I=1,N
                  IF (I.NE.K) A(I,K)=-A(I,K)*AKK
               END DO
            END IF
         END IF
      END DO
      IF (JF.NE.0) THEN
         D=0D0
      ELSE
         DO K=1,N
            NP1MK=N+1-K
            KI=IW(NP1MK)
            IF (NP1MK.NE.KI) THEN
               DO I=1,N
                  T=A(I,NP1MK)
                  A(I,NP1MK)=A(I,KI)
                  A(I,KI)=T
               END DO
            END IF
         END DO
      END IF

      END
      SUBROUTINE sla_PXY (NP,XYE,XYM,COEFFS,XYP,XRMS,YRMS,RRMS)
*+
*     - - - -
*      P X Y
*     - - - -
*
*  Given arrays of "expected" and "measured" [X,Y] coordinates, and a
*  linear model relating them (as produced by sla_FITXY), compute
*  the array of "predicted" coordinates and the RMS residuals.
*
*  Given:
*     NP       i        number of samples
*     XYE     d(2,np)   expected [X,Y] for each sample
*     XYM     d(2,np)   measured [X,Y] for each sample
*     COEFFS  d(6)      coefficients of model (see below)
*
*  Returned:
*     XYP     d(2,np)   predicted [X,Y] for each sample
*     XRMS     d        RMS in X
*     YRMS     d        RMS in Y
*     RRMS     d        total RMS (vector sum of XRMS and YRMS)
*
*  The model is supplied in the array COEFFS.  Naming the
*  elements of COEFF as follows:
*
*     COEFFS(1) = A
*     COEFFS(2) = B
*     COEFFS(3) = C
*     COEFFS(4) = D
*     COEFFS(5) = E
*     COEFFS(6) = F
*
*  the model is applied thus:
*
*     XP = A + B*XM + C*YM
*     YP = D + E*XM + F*YM
*
*  The residuals are (XP-XE) and (YP-YE).
*
*  If NP is less than or equal to zero, no coordinates are
*  transformed, and the RMS residuals are all zero.
*
*  See also sla_FITXY, sla_INVF, sla_XY2XY, sla_DCMPF
*
*  Called:  sla_XY2XY
*
*  P.T.Wallace   Starlink   22 May 1996
*
*  Copyright (C) 1996 Rutherford Appleton Laboratory
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      INTEGER NP
      DOUBLE PRECISION XYE(2,NP),XYM(2,NP),COEFFS(6),
     :                 XYP(2,NP),XRMS,YRMS,RRMS

      INTEGER I
      DOUBLE PRECISION SDX2,SDY2,XP,YP,DX,DY,DX2,DY2,P



*  Initialize summations
      SDX2=0D0
      SDY2=0D0

*  Loop by sample
      DO I=1,NP

*     Transform "measured" [X,Y] to "predicted" [X,Y]
         CALL sla_XY2XY(XYM(1,I),XYM(2,I),COEFFS,XP,YP)
         XYP(1,I)=XP
         XYP(2,I)=YP

*     Compute residuals in X and Y, and update summations
         DX=XYE(1,I)-XP
         DY=XYE(2,I)-YP
         DX2=DX*DX
         DY2=DY*DY
         SDX2=SDX2+DX2
         SDY2=SDY2+DY2

*     Next sample
      END DO

*  Compute RMS values
      P=MAX(1D0,DBLE(NP))
      XRMS=SQRT(SDX2/P)
      YRMS=SQRT(SDY2/P)
      RRMS=SQRT(XRMS*XRMS+YRMS*YRMS)

      END

      SUBROUTINE sla_DCMPF (COEFFS,XZ,YZ,XS,YS,PERP,ORIENT)
*+
*     - - - - - -
*      D C M P F
*     - - - - - -
*
*  Decompose an [X,Y] linear fit into its constituent parameters:
*  zero points, scales, nonperpendicularity and orientation.
*
*  Given:
*     COEFFS  d(6)      transformation coefficients (see note)
*
*  Returned:
*     XZ       d        x zero point
*     YZ       d        y zero point
*     XS       d        x scale
*     YS       d        y scale
*     PERP     d        nonperpendicularity (radians)
*     ORIENT   d        orientation (radians)
*
*  Called:  sla_DRANGE
*
*  The model relates two sets of [X,Y] coordinates as follows.
*  Naming the elements of COEFFS:
*
*     COEFFS(1) = A
*     COEFFS(2) = B
*     COEFFS(3) = C
*     COEFFS(4) = D
*     COEFFS(5) = E
*     COEFFS(6) = F
*
*  the model transforms coordinates [X1,Y1] into coordinates
*  [X2,Y2] as follows:
*
*     X2 = A + B*X1 + C*Y1
*     Y2 = D + E*X1 + F*Y1
*
*  The transformation can be decomposed into four steps:
*
*     1)  Zero points:
*
*             x' = XZ + X1
*             y' = YZ + Y1
*
*     2)  Scales:
*
*             x'' = XS*x'
*             y'' = YS*y'
*
*     3)  Nonperpendicularity:
*
*             x''' = cos(PERP/2)*x'' + sin(PERP/2)*y''
*             y''' = sin(PERP/2)*x'' + cos(PERP/2)*y''
*
*     4)  Orientation:
*
*             X2 = cos(ORIENT)*x''' + sin(ORIENT)*y'''
*             Y2 =-sin(ORIENT)*y''' + cos(ORIENT)*y'''
*
*  See also sla_FITXY, sla_PXY, sla_INVF, sla_XY2XY
*
*  P.T.Wallace   Starlink   19 December 2001
*
*  Copyright (C) 2001 Rutherford Appleton Laboratory
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      DOUBLE PRECISION COEFFS(6),XZ,YZ,XS,YS,PERP,ORIENT

      DOUBLE PRECISION A,B,C,D,E,F,RB2E2,RC2F2,XSC,YSC,P1,P2,P,WS,WC,
     :                 OR,HP,SHP,CHP,SOR,COR,DET,X0,Y0,sla_DRANGE



*  Copy the six coefficients.
      A = COEFFS(1)
      B = COEFFS(2)
      C = COEFFS(3)
      D = COEFFS(4)
      E = COEFFS(5)
      F = COEFFS(6)

*  Scales.
      RB2E2 = SQRT(B*B+E*E)
      RC2F2 = SQRT(C*C+F*F)
      IF (B*F-C*E.GE.0D0) THEN
         XSC = RB2E2
      ELSE
         B = -B
         E = -E
         XSC = -RB2E2
      END IF
      YSC = RC2F2

*  Non-perpendicularity.
      IF (C.NE.0D0.OR.F.NE.0D0) THEN
         P1 = ATAN2(C,F)
      ELSE
         P1 = 0D0
      END IF
      IF (E.NE.0D0.OR.B.NE.0D0) THEN
         P2 = ATAN2(E,B)
      ELSE
         P2 = 0D0
      END IF
      P = sla_DRANGE(P1+P2)

*  Orientation.
      WS = C*RB2E2-E*RC2F2
      WC = B*RC2F2+F*RB2E2
      IF (WS.NE.0D0.OR.WC.NE.0D0) THEN
         OR = ATAN2(WS,WC)
      ELSE
         OR = 0D0
      END IF

*  Zero points.
      HP = P/2D0
      SHP = SIN(HP)
      CHP = COS(HP)
      SOR = SIN(OR)
      COR = COS(OR)
      DET = XSC*YSC*(CHP+SHP)*(CHP-SHP)
      IF (ABS(DET).GT.0D0) THEN
         X0 = YSC*(A*(CHP*COR-SHP*SOR)-D*(CHP*SOR+SHP*COR))/DET
         Y0 = XSC*(A*(CHP*SOR-SHP*COR)+D*(CHP*COR+SHP*SOR))/DET
      ELSE
         X0 = 0D0
         Y0 = 0D0
      END IF

*  Results.
      XZ = X0
      YZ = Y0
      XS = XSC
      YS = YSC
      PERP = P
      ORIENT = OR

      END

      SUBROUTINE sla_INVF (FWDS,BKWDS,J)
*+
*     - - - - -
*      I N V F
*     - - - - -
*
*  Invert a linear model of the type produced by the sla_FITXY routine.
*
*  Given:
*     FWDS    d(6)      model coefficients
*
*  Returned:
*     BKWDS   d(6)      inverse model
*     J        i        status:  0 = OK, -1 = no inverse
*
*  The models relate two sets of [X,Y] coordinates as follows.
*  Naming the elements of FWDS:
*
*     FWDS(1) = A
*     FWDS(2) = B
*     FWDS(3) = C
*     FWDS(4) = D
*     FWDS(5) = E
*     FWDS(6) = F
*
*  where two sets of coordinates [X1,Y1] and [X2,Y1] are related
*  thus:
*
*     X2 = A + B*X1 + C*Y1
*     Y2 = D + E*X1 + F*Y1
*
*  the present routine generates a new set of coefficients:
*
*     BKWDS(1) = P
*     BKWDS(2) = Q
*     BKWDS(3) = R
*     BKWDS(4) = S
*     BKWDS(5) = T
*     BKWDS(6) = U
*
*  such that:
*
*     X1 = P + Q*X2 + R*Y2
*     Y1 = S + T*X2 + U*Y2
*
*  Two successive calls to sla_INVF will thus deliver a set
*  of coefficients equal to the starting values.
*
*  To comply with the ANSI Fortran standard, FWDS and BKWDS must
*  not be the same array, even though the routine is coded to
*  work on many platforms even if this rule is violated.
*
*  See also sla_FITXY, sla_PXY, sla_XY2XY, sla_DCMPF
*
*  Last revision:   26 December 2004
*
*  Copyright P.T.Wallace.  All rights reserved.
*
*  License:
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program (see SLA_CONDITIONS); if not, write to the
*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
*    Boston, MA  02110-1301  USA
*
*-

      IMPLICIT NONE

      DOUBLE PRECISION FWDS(6),BKWDS(6)
      INTEGER J

      DOUBLE PRECISION A,B,C,D,E,F,DET



      A=FWDS(1)
      B=FWDS(2)
      C=FWDS(3)
      D=FWDS(4)
      E=FWDS(5)
      F=FWDS(6)
      DET=B*F-C*E
      IF (DET.NE.0D0) THEN
         BKWDS(1)=(C*D-A*F)/DET
         BKWDS(2)=F/DET
         BKWDS(3)=-C/DET
         BKWDS(4)=(A*E-B*D)/DET
         BKWDS(5)=-E/DET
         BKWDS(6)=B/DET
         J=0
      ELSE
         J=-1
      END IF

      END

