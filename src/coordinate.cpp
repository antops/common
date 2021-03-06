#include "../include/coordinate.h"
#include "../include/Matrix4D.h"
#include "../include/Vector3D.h"
#include "../include/constant_var.h"
#include "eigen/Eigen/Dense"
#include "eigen/Eigen/eigenvalues"

using namespace Eigen;

namespace Common {

	bool Coordinate::SetUV(const Vector3 & u, const Vector3 & v) {
		Vector3 tmpU = u;
		Vector3 tmpV = v;
		tmpU.Normalization();
		tmpV.Normalization();
		U = tmpU;
		V = tmpV;
		N = U.Cross(V);
		N.Normalization();
		Matrix3d A;
		A << U.x, V.x, N.x, U.y, V.y, N.y, U.z, V.z, N.z;
		EigenSolver<Matrix3d> es(A);
		Matrix3d DM = es.pseudoEigenvalueMatrix();
		Matrix3d VM = es.pseudoEigenvectors();
		Vector3 rotate_axis_tmp;
		for (int i = 0; i < 3; i++) {
			double eigenvalue = DM(i, i);
			if (eigenvalue > 0.999 && eigenvalue < 1.001) {
				rotate_axis_tmp.Set(VM(0, i), VM(1, i), VM(2, i));
			}
		}
		rotate_axis_tmp.Normalization();
		//update Rotation Angle:
		double theta;
		Vector3 tempz(0, 0, 1);
		Vector3 tempx(1, 0, 0);
		Vector3 tempy(0, 1, 0);
		double rotate_theta_tmp;

		if (abs(tempz.Dot(rotate_axis_tmp)) < 0.9) {
			Vector3 v = tempz.Cross(rotate_axis_tmp);
			v.Normalization();
			Vector3 rv = Vector3(A(0, 0)*v.x + A(0, 1)*v.y + A(0, 2)*v.z,
				A(1, 0)*v.x + A(1, 1)*v.y + A(1, 2)*v.z,
				A(2, 0)*v.x + A(2, 1)*v.y + A(2, 2)*v.z);
			rotate_theta_tmp = acos(v.Dot(rv) / v.Length() / rv.Length());
			rotate_theta_tmp = rotate_theta_tmp * 180 / Pi;
			rotate_axis = rotate_axis_tmp;
			rotate_theta = rotate_theta_tmp;

			Matrix4D CheckM;
			CheckM = Matrix4D::getRotateMatrix(rotate_theta_tmp, rotate_axis_tmp.x, rotate_axis_tmp.y, rotate_axis_tmp.z);
			Vector3 Uc, Vc, Nc;
			Uc = CheckM*Vector3(1, 0, 0);	Vc = CheckM*Vector3(0, 1, 0);	Nc = CheckM*Vector3(0, 0, 1);
			if ((Uc.Dot(U) > 0.999) && (Vc.Dot(V) > 0.999) && (Nc.Dot(N) > 0.999))	return true;
			else {
				rotate_axis_tmp = rotate_axis_tmp*(-1);
				v = tempz.Cross(rotate_axis_tmp);
				v.Normalization();
				rv = Vector3(A(0, 0)*v.x + A(0, 1)*v.y + A(0, 2)*v.z,
					A(1, 0)*v.x + A(1, 1)*v.y + A(1, 2)*v.z,
					A(2, 0)*v.x + A(2, 1)*v.y + A(2, 2)*v.z);
				rotate_theta_tmp = acos(v.Dot(rv) / v.Length() / rv.Length());
				rotate_theta_tmp = rotate_theta_tmp * 180 / Pi;
				rotate_axis = rotate_axis_tmp;
				rotate_theta = rotate_theta_tmp;
				return true;
			}
		}
		else if (abs(tempx.Dot(rotate_axis_tmp)) < 0.9) {
			Vector3 v = tempx.Cross(rotate_axis_tmp);
			v.Normalization();
			Vector3 rv = Vector3(A(0, 0)*v.x + A(0, 1)*v.y + A(0, 2)*v.z,
				A(1, 0)*v.x + A(1, 1)*v.y + A(1, 2)*v.z,
				A(2, 0)*v.x + A(2, 1)*v.y + A(2, 2)*v.z);
			rotate_theta_tmp = acos(v.Dot(rv) / v.Length() / rv.Length());
			rotate_theta_tmp = rotate_theta_tmp * 180 / Pi;
			rotate_axis = rotate_axis_tmp;
			rotate_theta = rotate_theta_tmp;
			Matrix4D CheckM;
			CheckM = Matrix4D::getRotateMatrix(rotate_theta_tmp, rotate_axis_tmp.x, rotate_axis_tmp.y, rotate_axis_tmp.z);
			Vector3 Uc, Vc, Nc;
			Uc = CheckM*Vector3(1, 0, 0);	Vc = CheckM*Vector3(0, 1, 0);	Nc = CheckM*Vector3(0, 0, 1);
			if ((Uc.Dot(U) > 0.999) && (Vc.Dot(V) > 0.999) && (Nc.Dot(N) > 0.999))	return true;
			else {
				rotate_axis_tmp = rotate_axis_tmp*(-1);
				v = tempx.Cross(rotate_axis_tmp);
				v.Normalization();
				rv = Vector3(A(0, 0)*v.x + A(0, 1)*v.y + A(0, 2)*v.z,
					A(1, 0)*v.x + A(1, 1)*v.y + A(1, 2)*v.z,
					A(2, 0)*v.x + A(2, 1)*v.y + A(2, 2)*v.z);
				rotate_theta_tmp = acos(v.Dot(rv) / v.Length() / rv.Length());
				rotate_theta_tmp = rotate_theta_tmp * 180 / Pi;
				rotate_axis = rotate_axis_tmp;
				rotate_theta = rotate_theta_tmp;
				return true;
			}
		}
		else if (abs(tempy.Dot(rotate_axis_tmp)) < 0.9) {
			Vector3 v = tempy.Cross(rotate_axis_tmp);
			v.Normalization();
			Vector3 rv = Vector3(A(0, 0)*v.x + A(0, 1)*v.y + A(0, 2)*v.z,
				A(1, 0)*v.x + A(1, 1)*v.y + A(1, 2)*v.z,
				A(2, 0)*v.x + A(2, 1)*v.y + A(2, 2)*v.z);
			rotate_theta_tmp = acos(v.Dot(rv) / v.Length() / rv.Length());
			rotate_theta_tmp = rotate_theta_tmp * 180 / Pi;
			rotate_axis = rotate_axis_tmp;
			rotate_theta = rotate_theta_tmp;
			Matrix4D CheckM;
			CheckM = Matrix4D::getRotateMatrix(rotate_theta_tmp, rotate_axis_tmp.x, rotate_axis_tmp.y, rotate_axis_tmp.z);
			Vector3 Uc, Vc, Nc;
			Uc = CheckM*Vector3(1, 0, 0);	Vc = CheckM*Vector3(0, 1, 0);	Nc = CheckM*Vector3(0, 0, 1);
			if ((Uc.Dot(U) > 0.999) && (Vc.Dot(V) > 0.999) && (Nc.Dot(N) > 0.999))	return true;
			else {
				rotate_axis_tmp = rotate_axis_tmp*(-1);
				v = tempy.Cross(rotate_axis_tmp);
				v.Normalization();
				rv = Vector3(A(0, 0)*v.x + A(0, 1)*v.y + A(0, 2)*v.z,
					A(1, 0)*v.x + A(1, 1)*v.y + A(1, 2)*v.z,
					A(2, 0)*v.x + A(2, 1)*v.y + A(2, 2)*v.z);
				rotate_theta_tmp = acos(v.Dot(rv) / v.Length() / rv.Length());
				rotate_theta_tmp = rotate_theta_tmp * 180 / Pi;
				rotate_axis = rotate_axis_tmp;
				rotate_theta = rotate_theta_tmp;
				return true;
			}
		}

		return false;
	}

	void Coordinate::SetRotate(const Vector3 & axis, double theta) {
		rotate_theta = theta;
		rotate_axis = axis;
		Vector3D RotateAsix(rotate_axis.x, rotate_axis.y, rotate_axis.z);
		Matrix4D matrix = Matrix4D::getRotateMatrix(rotate_theta, RotateAsix);
		U = matrix * Vector3(1, 0, 0);
		V = matrix * Vector3(0, 1, 0);
		N = matrix * Vector3(0, 0, 1);
	}
}