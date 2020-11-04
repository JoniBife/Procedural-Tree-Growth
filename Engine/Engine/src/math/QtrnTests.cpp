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


int main(int argc, char* argv[])
{
	qtest1();
}