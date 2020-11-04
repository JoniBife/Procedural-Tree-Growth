#include <iostream>
#include "Qtrn.h"
#include "MathAux.h"
#include <cassert>;

#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;
#define PRINT(X,Y) std::cout<<X<<Y<<std::endl;

void qtest1()
{
	HEADER("TEST 1 : Rotation of 90 degrees about the y-axis")

	HEADER("q:")
	Vec3 axis = { 0.0f, 1.0f, 0.0f };
	Qtrn q(degreesToRadians(90.0f), axis);
	PRINT("    q", q);

	Qtrn qi = { 0.0f, 7.0f, 0.0f, 0.0f };
	PRINT("   vi", qi);

	Qtrn qe = { 0.0f, 0.0f, 0.0f, -7.0f };
	PRINT("   qe", qe);

	Qtrn qinv = q.inverse();
	qinv.clean(); // "-0"
	PRINT(" qinv", qinv);

	Qtrn qf1 = (q * qi) * qinv;
	PRINT("  qf1", qf1);

	assert(qf1 == qe);

	Qtrn qconj = q.conjugate();
	PRINT("qconj", qconj);

	Qtrn qf2 = (q * qi) * qconj;
	PRINT("  qf2", qf2);

	assert(qf2 == qe);
}

void qtest2()
{
	HEADER("TEST 2 : Rotation of 90 degrees about the y-axis with matrix")

	Vec3 axis = { 0.0f, 1.0f, 0.0f};
	Qtrn q(degreesToRadians(90.0f), axis);
	PRINT(" q", q);

	Vec4 vi = { 7.0f, 0.0f, 0.0f, 1.0f };
	PRINT("vi", vi);

	Vec4 ve = { 0.0f, 0.0f, -7.0f, 1.0f };
	PRINT("ve", ve);

	Mat4 m;
	m = q.toRotationMatrix();
	PRINT(" M", "   ", m);

	Vec4 vf = m * vi;
	PRINT("vf", vf);

	assert(vf == ve);
}

void qtest3()
{
	HEADER("TEST 3 : Yaw 900 = Roll 180 + Pitch 180")

	const Vec3 axis_x = { 1.0f, 0.0f, 0.0f };
	const Vec3 axis_y = { 0.0f, 1.0f, 0.0f };
	const Vec3 axis_z = { 0.0f, 0.0f, 1.0f };

	Qtrn qyaw900(degreesToRadians(900.0f), axis_y);
	("  qyaw900", qyaw900);

	Qtrn qroll180(degreesToRadians(180.0f), axis_x);
	PRINT(" qroll180", qroll180);
	Qtrn qpitch180(degreesToRadians(180.0f), axis_z);
	PRINT("qpitch180", qpitch180);
	Qtrn qrp = qpitch180 * qroll180;
	PRINT("      qrp", qrp);
	Qtrn qpr = qroll180 * qpitch180;
	PRINT("      qpr", qpr);

	Qtrn qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
	PRINT("       qi", qi);
	Qtrn qe = { 0.0f, -1.0f, 0.0f, 0.0f };
	PRINT("       qe", qe);

	Qtrn qfy = (qyaw900 * qi) * qyaw900.inverse();
	PRINT("       qy", qfy);
	assert(qe == qfy);

	Qtrn qfrp = (qrp * qi) * qrp.inverse();
	PRINT("     qfrp", qfrp);
	assert(qe == qfrp);

	Qtrn qfpr = (qpr * qi) * qpr.inverse();
	PRINT("     qfpr", qfpr);
	assert(qe == qfpr);
}

void qtest4()
{
	HEADER("TEST 4: Q <-> (angle, axis)")

	float thetai = 45.0f;
	Vec3 axis_i = { 0.0f, 1.0f, 0.0f };
	Qtrn q(degreesToRadians(thetai), axis_i);
	std::cout << thetai << " ";
	PRINT("axis_i", axis_i);

	float thetaf;
	Vec3 axis_f;
	q.toAngleAxis(thetaf, axis_f);
	
	std::cout << thetaf << " ";
	PRINT("axis_f", axis_f);

	assert(cmpf(degreesToRadians(thetai), thetaf) && axis_i == axis_f);
}

void qtest5()
{
	HEADER("TEST 5: LERP")

	Vec3 axis = { 0.0f, 1.0f, 0.0f};
	Qtrn q0(0.0f, axis);
	PRINT("       q0", q0);
	Qtrn q1(90.0f, axis);
	PRINT("       q1", q1);
	Qtrn qe(30.0f, axis);
	PRINT("       qe", qe);

	Qtrn qLerp0 = Qtrn::lerp(q0, q1, 0.0f);
	PRINT("  lerp(0)", qLerp0);
	assert(qLerp0 == q0);

	Qtrn qLerp1 = Qtrn::lerp(q0, q1, 1.0f);
	PRINT("  lerp(1)", qLerp1);
	assert(qLerp1 == q1);

	Qtrn qlerp = Qtrn::lerp(q0, q1, 1 / 3.0f);
	PRINT("lerp(1/3)", qlerp);
	PRINT("lerp(1/3)", qlerp);

	assert(qlerp != qe);
}

void qtest6()
{
	HEADER("TEST 6: SLERP")

	Vec3 axis = { 0.0f, 1.0f, 0.0f};
	Qtrn q0(0.0f, axis);
	PRINT("        q0", q0);
	Qtrn q1(degreesToRadians(90.0f), axis);
	PRINT("        q1", q1);
	Qtrn qe(degreesToRadians(30.0f), axis);
	PRINT("        qe", qe);

	Qtrn qSlerp0 = Qtrn::slerp(q0, q1, 0.0f);
	PRINT("  slerp(0)", qSlerp0);
	assert(qSlerp0 == q0);

	Qtrn qSlerp1 = Qtrn::slerp(q0, q1, 1.0f);
	PRINT("  slerp(1)", qSlerp1);
	assert(qSlerp1 == q1);

	Qtrn qslerp = Qtrn::slerp(q0, q1, 1 / 3.0f);
	PRINT("slerp(1/3)", qslerp);
	PRINT("slerp(1/3)", qslerp);

	assert(qslerp == qe);
}

int main(int argc, char* argv[])
{
	//qtest1();
	//qtest2();
	//qtest3();
	//qtest4();
	//qtest5();
	qtest6();

}