#include "vector.h"
#include <math.h>

Vector_t createVec(double x, double y) {

	Vector_t v1;
	v1.x = x;
	v1.y = y;

	return v1;

}

double vecSize(Vector_t v1) {
	return sqrt(pow(v1.x, 2) + pow(v1.y, 2));
}

Vector_t subtractVectors(Vector_t v1, Vector_t v2) {

	Vector_t v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;

	return v;

}

Vector_t vectorMultiplyFactor(Vector_t v1, double factor) {

	v1.x *= factor;
	v1.y *= factor;

	return v1;

}

double dotProduct(Vector_t v1, Vector_t v2) {

	return v1.x * v2.x + v1.y * v2.y;

}

Vector_t divideVectors(Vector_t v1, Vector_t v2) {

	Vector_t v;
	v.x = v1.x / v2.x;
	v.y = v1.y / v2.y;

	return v;
}

Vector_t addVectors(Vector_t v1, Vector_t v2) {
	Vector_t v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;

	return v;
}

Vector_t multiplyVectors(Vector_t v1, Vector_t v2) {

	Vector_t v;
	v.x = v1.x * v2.x;
	v.y = v1.y * v2.y;

	return v;
}

Vector_t vectorAddFactor(Vector_t v1, double factor) {

	v1.x += factor;
	v1.y += factor;

	return v1;

}

Vector_t vectorSubtractFactor(Vector_t v1, double factor) {

	v1.x -= factor;
	v1.y -= factor;

	return v1;

}

void normalize(Vector_t * v1) {

	double size = vecSize(*v1);
	v1->x /= size;
	v1->y /= size;

}
