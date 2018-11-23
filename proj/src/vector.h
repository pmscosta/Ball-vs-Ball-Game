/**
 * @file vector.h
 * @author Pedro Costa, João Fidalgo
 * @date 29 Dec 2017
 * @brief File containing all the functions and informations regarding vectors directions and operations in game.
 *
 */

#ifndef __VECTOR_H
#define __VECTOR_H

/** @defgroup vector vector
 * @{
 *
 * Module that handles everything related to the vectors.
 */

/** @name Vector_t Structure */
/**@{
 *
 *@brief Vector_t Structure containing all of its attributes
 *
 */
typedef struct{

	double x;/**< Coordinate in x axis of the vector. */
	double y;/**< Coordinate in y axis of the vector. */

} Vector_t;
/** @} end of Vector_t Structure*/
/**
 * @brief Creates the vector
 *
 * @param x - Coordinate in x axis of the vector to create.
 * @param y - Coordinate in y axis of the vector to create.
 * @return v1 - A Vector_t after created.
 *
 */
Vector_t createVec(double x, double y);
/**
 * @brief Calculates the size of a vector.
 *
 * @param v1 - vector to calculate the size.
 *
 * @return The size of the given vector.
 *
 */
double vecSize(Vector_t v1);
/**
 * @brief Subtracts two vectors
 *
 * @param v1 - Vector to be subtracted.
 * @param v2 - Vector that subtracts.
 * @return v - The result of the subtraction.
 *
 */
Vector_t subtractVectors(Vector_t v1, Vector_t v2);
/**
 * @brief Divides two vectors
 *
 * @param v1 - Dividend.
 * @param v2 - Divider.
 * @return v - The result of the division.
 *
 */
Vector_t divideVectors(Vector_t v1, Vector_t v2);
/**
 * @brief Adds two vectors
 *
 * @param v1 - One of the vectors to be added.
 * @param v2 - Second of the vectors to be added.
 * @return v - The result of the addition.
 *
 */
Vector_t addVectors(Vector_t v1, Vector_t v2);
/**
 * @brief Multiplies two vectors
 *
 * @param v1 - Vector to multiply.
 * @param v2 - Vector to multiply.
 * @return v - The result of the multiplication.
 *
 */
Vector_t multiplyVectors(Vector_t v1, Vector_t v2);
/**
 * @brief Multiplies a vector with a constant.
 *
 * @param v1 - Vector to multiply.
 * @param factor - Number to multiply the vector.
 * @return v - The result of the multiplication.
 *
 */
Vector_t vectorMultiplyFactor(Vector_t v1, double factor);
/**
 * @brief Adds a vector with a constant.
 *
 *	Adds factor to coordinates , x and y, of the vector.
 * @param v1 - Vector to be added.
 * @param factor - Number to add the vector.
 * @return v - The result of the addition.
 *
 */
Vector_t vectorAddFactor(Vector_t v1, double factor);
/**
 * @brief Subtracts a vector with a constant.
 *
 *	Subtracts factor to coordinates , x and y, of the vector.
 * @param v1 - Vector to be subtracted.
 * @param factor - Number to subtract to the vector.
 * @return v - The result of the subtraction.
 *
 */
Vector_t vectorSubtractFactor(Vector_t v1, double factor);
/**
 * @brief Normalizes a vector.
 *
 *	Divides the vector coordinates by his size.
 * @param v1 - Vector to be normalized.
 *
 */
void normalize(Vector_t * v1);
/**
 * @brief Dot product of two vectors.
 *
 * @param v1 - First vector.
 * @param v2 - Second vector.
 * @return Result of the vectorial product.
 *
 */
double dotProduct(Vector_t v1, Vector_t v2);

/** @} end of vector */
#endif
