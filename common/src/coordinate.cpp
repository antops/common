#include "../include/coordinate.h"
#include "../include/Matrix4D.h"
#include "../include/Vector3D.h"
#include "../include/constant_var.h"
#include "eigen/Eigen/Dense"
#include "eigen/Eigen/eigenvalues"

using namespace Eigen;

namespace Common {
	void Coordinate::Save(std::ofstream & savefile) const {
		U.Save(savefile);
		V.Save(savefile);
		N.Save(savefile);
		pos.Save(savefile);
		rotate_axis.Save(savefile);
		savefile << " " << rotate_theta << " " << scale;
	}

	void Coordinate::Open(std::ifstream & readfile)
	{
		U.Open(readfile);
		V.Open(readfile);
		N.Open(readfile);
		pos.Open(readfile);
		rotate_axis.Open(readfile);
		readfile >> rotate_theta >> scale;
	}

	std::string Coordinate::Serialize() const
	{
		std::string buffer = U.Serialize() + V.Serialize()
			+ N.Serialize() + pos.Serialize() + rotate_axis.Serialize();
		buffer.append(reinterpret_cast<const char*>(&rotate_theta), sizeof(rotate_theta));
		buffer.append(reinterpret_cast<const char*>(&scale), sizeof(scale));
		return std::string();
	}

	void Coordinate::UnSerialize(const char * p, size_t size)
	{
		if (size != Size()) return;
		U.UnSerialize(p, U.Size());
		p += U.Size();
		V.UnSerialize(p, V.Size());
		p += V.Size();
		N.UnSerialize(p, N.Size());
		p += N.Size();
		pos.UnSerialize(p, pos.Size());
		p += pos.Size();
		rotate_axis.UnSerialize(p, rotate_axis.Size());
		p += rotate_axis.Size();
		memcpy(&rotate_theta, p, sizeof(double));
		p += sizeof(double);
		memcpy(&scale, p, sizeof(double));
	}

	void Coordinate::UnSerialize(const std::string & buffer)
	{
		return UnSerialize(buffer.c_str(), buffer.size());
	}

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