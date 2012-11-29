///
/// @file CurvesTests.cpp Tests for ParamCurves.
/// @author Enrique Juan Gil Izquierdo
///
/**
Copyright (c) 2012 Enrique Juan Gil Izquierdo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#include <stdio.h>
#include "../ParamCurves/ParamCurve.h"
#include "../ParamCurves/Interpolator.h"
#include "../ParamCurves/LinearInterpolator.h"
#include "../ParamCurves/ClampInterpolator.h"
#include "../ParamCurves/ClampUpInterpolator.h"
#include "../ParamCurves/CatmullRomInterpolator.h"

void testLinear();
void testClamp();
void testCompNonDivClamp();
void testCompNonDivLinear();
void testCompClassClamp();
void testCompClassClampUp();
void testCompClassLinear();
void testCatmullRomFloat();
void testCatmullRom();

const size_t testsSize = 5;

int main(int argc, char* argv[])
{
	printf("Testing linear interpolation:\n");
	testLinear();
	printf("\nTesting clamp interpolation:\n");
	testClamp();

	printf("\nTesting nondiv clamp interpolation:\n");
	testCompNonDivClamp();
	printf("\nTesting nondiv linear interpolation:\n");
	testCompNonDivLinear();

	printf("\nTesting div clamp interpolation:\n");
	testCompClassClamp();
	printf("\nTesting div clamp up interpolation:\n");
	testCompClassClampUp();
	printf("\nTesting div linear interpolation:\n");
	testCompClassLinear();

	printf("\nTesting Catmull-Rom interpolation:\n");
	testCatmullRomFloat();
	testCatmullRom();

	return 0;
}

template<typename T>
bool almostEqual(T v1, T v2) {
	if (v2 > v1)
		return v2 - v1 < .0001;
	else
		return v1 - v2 < .0001;
}

template<typename TInput, typename TOutput, size_t curveSize>
bool check(TInput input, TOutput expectedOutput, ParamCurve<TInput, TOutput, curveSize>* const curve) {
	TOutput output = curve->getValue(input);

	bool result = almostEqual<float>(expectedOutput, output);
	if (result) {
		printf("Success: getValue(%f) -> %f == %f\n", (float)input, (float)output, (float)expectedOutput);
	}
	else {
		printf("Failure: getValue(%f) -> %f != %f\n", (float)input, (float)output, (float)expectedOutput);
	}

	return result;
}

void testLinear() {
	ParamCurve<float, float, testsSize> curve;
	float inputs[testsSize] = { 0.f, .5f, 1.5f, 2.f, 5.f };
	float outputs[testsSize] = { 4.f, 3.f, 4.f, 5.f, 5.f };
	Interpolator<float, float>* interpolator = LinearInterpolator<float, float>::getInstance();
	curve.initialize(interpolator, testsSize, inputs, outputs);

	check<float, float, testsSize>(-1.f,		4.f,	&curve);
	check<float, float, testsSize>(0.f,			4.f,	&curve);
	check<float, float, testsSize>(0.000001f,	4.f,	&curve);
	check<float, float, testsSize>(.125f,		3.75f,	&curve);
	check<float, float, testsSize>(.25f,		3.5f,	&curve);
	check<float, float, testsSize>(.5f,			3.f,	&curve);
	check<float, float, testsSize>(1.f,			3.5f,	&curve);
	check<float, float, testsSize>(1.6f,		4.2f,	&curve);
	check<float, float, testsSize>(2.f,			5.f,	&curve);
	check<float, float, testsSize>(3.f,			5.f,	&curve);
	check<float, float, testsSize>(4.f,			5.f,	&curve);
	check<float, float, testsSize>(5.f,			5.f,	&curve);
	check<float, float, testsSize>(20.f,		5.f,	&curve);
	check<float, float, testsSize>(5.01f,		5.f,	&curve);
}

void testClamp() {
	ParamCurve<float, float, testsSize> curve;
	float inputs[testsSize] = { 0.f, .5f, 1.5f, 2.f, 5.f };
	float outputs[testsSize] = { 4.f, 3.f, 4.f, 5.f, 5.f };
	Interpolator<float, float>* interpolator = ClampInterpolator<float, float>::getInstance();
	curve.initialize( interpolator, testsSize, inputs, outputs );

	check<float, float, testsSize>(-1.f,	4.f,	&curve);
	check<float, float, testsSize>(0.f,		4.f,	&curve);
	check<float, float, testsSize>(.125f,	4.f,	&curve);
	check<float, float, testsSize>(.25f,	4.f,	&curve);
	check<float, float, testsSize>(.5f,		3.f,	&curve);
	check<float, float, testsSize>(1.f,		3.f,	&curve);
	check<float, float, testsSize>(1.6f,	4.f,	&curve);
	check<float, float, testsSize>(2.f,		5.f,	&curve);
	check<float, float, testsSize>(3.f,		5.f,	&curve);
	check<float, float, testsSize>(4.f,		5.f,	&curve);
	check<float, float, testsSize>(5.f,		5.f,	&curve);
	check<float, float, testsSize>(20.f,	5.f,	&curve);
	check<float, float, testsSize>(5.01f,	5.f,	&curve);
}

class CompNonDivClass {
public:
	float value;

	CompNonDivClass() { value = 0.f; }
	CompNonDivClass(float newValue) { value = newValue; }

	bool operator== (const CompNonDivClass &c2) const { return value == c2.value; }
	bool operator< (const CompNonDivClass &c2) const { return value < c2.value; }
	bool operator> (const CompNonDivClass &c2) const { return value > c2.value; }
	bool operator<= (const CompNonDivClass &c2) const { return value <= c2.value; }
	bool operator>= (const CompNonDivClass &c2) const { return value >= c2.value; }

	operator float() { return value; }
};

void testCompNonDivClamp() {
	// This should compile, because clamp only requires input to be comparable
	ParamCurve<CompNonDivClass, float, testsSize> curve;
	CompNonDivClass inputs[testsSize] = { CompNonDivClass(0.f), CompNonDivClass(1.f), CompNonDivClass(2.f), CompNonDivClass(3.f), CompNonDivClass(4.f) };
	float outputs[testsSize] = { 4.f, 3.f, 4.f, 5.f, 5.f };
	Interpolator<CompNonDivClass, float>* interpolator = ClampInterpolator<CompNonDivClass, float>::getInstance();
	curve.initialize(interpolator, testsSize, inputs, outputs);
	
	check<CompNonDivClass, float, testsSize>(-1.f,			4.f,	&curve);
	check<CompNonDivClass, float, testsSize>(0.f,			4.f,	&curve);
	check<CompNonDivClass, float, testsSize>(0.000001f,		4.f,	&curve);
	check<CompNonDivClass, float, testsSize>(.125f,			4.f,	&curve);
	check<CompNonDivClass, float, testsSize>(.25f,			4.f,	&curve);
	check<CompNonDivClass, float, testsSize>(.5f,			4.f,	&curve);
	check<CompNonDivClass, float, testsSize>(1.f,			3.f,	&curve);
	check<CompNonDivClass, float, testsSize>(1.6f,			3.f,	&curve);
	check<CompNonDivClass, float, testsSize>(2.f,			4.f,	&curve);
	check<CompNonDivClass, float, testsSize>(3.f,			5.f,	&curve);
	check<CompNonDivClass, float, testsSize>(4.f,			5.f,	&curve);
	check<CompNonDivClass, float, testsSize>(5.f,			5.f,	&curve);
	check<CompNonDivClass, float, testsSize>(20.f,			5.f,	&curve);
	check<CompNonDivClass, float, testsSize>(5.01f,			5.f,	&curve);
}

void testCompNonDivLinear() {
	// This should fail to compile because linear interpolator requires Output to support operators +, -, *, / and float
	////ParamCurve<float, CompNonDivClass, testsSize> curve;
	////
	////float inputs[testsSize] = { 0.f, 1.f, 2.f, 3.f, 4.f };
	////CompNonDivClass outputs[testsSize] = { CompNonDivClass(0.f), CompNonDivClass(1.f), CompNonDivClass(2.f), CompNonDivClass(3.f), CompNonDivClass(4.f) };
	////Interpolator<float, CompNonDivClass>* interpolator = LinearInterpolator<float, CompNonDivClass>::getInstance();
	////curve.initialize(interpolator, testsSize, inputs, outputs);
	////
	////check<float, CompNonDivClass, testsSize>(.5f, .5f, &curve);
}

class CompClass {
public:
	float value;

	CompClass() { value = 0.f; }
	CompClass(float newValue) { value = newValue; }

	bool operator== (const CompClass &c2) const { return value == c2.value; }
	bool operator< (const CompClass &c2) const { return value < c2.value; }
	bool operator> (const CompClass &c2) const { return value > c2.value; }
	bool operator<= (const CompClass &c2) const { return value <= c2.value; }
	bool operator>= (const CompClass &c2) const { return value >= c2.value; }

	// All this return a copy of the resulting CompClass.
	CompClass operator+ (const CompClass &c2) const { return CompClass( value + c2.value ); }
	CompClass operator- (const CompClass &c2) const { return CompClass( value - c2.value ); }
	CompClass operator* (const CompClass &c2) const { return CompClass( value * c2.value ); }
	CompClass operator/ (const CompClass &c2) const { return CompClass( value / c2.value ); }

	CompClass operator* (const float &f) const { return CompClass( value * f ); }
	CompClass operator/ (const float &f) const { return CompClass( value / f ); }

	operator float() { return value; }
};

void testCompClassClamp() {
	ParamCurve<CompClass, float, testsSize> curve;
	
	CompClass inputs[testsSize] = { CompClass(0.f), CompClass(1.f), CompClass(2.f), CompClass(3.f), CompClass(4.f) };
	float outputs[testsSize] = { 0.f, 1.f, 4.f, 9.f, 16.f };
	Interpolator<CompClass, float>* interpolator = ClampInterpolator<CompClass, float>::getInstance();
	curve.initialize(interpolator, testsSize, inputs, outputs);
	
	check<CompClass, float, testsSize>(-1.f,	0.f,	&curve);
	check<CompClass, float, testsSize>(0.f,		0.f,	&curve);
	check<CompClass, float, testsSize>(.125f,	0.f,	&curve);
	check<CompClass, float, testsSize>(.25f,	0.f,	&curve);
	check<CompClass, float, testsSize>(.5f,		0.f,	&curve);
	check<CompClass, float, testsSize>(1.f,		1.f,	&curve);
	check<CompClass, float, testsSize>(1.6f,	1.f,	&curve);
	check<CompClass, float, testsSize>(2.f,		4.f,	&curve);
	check<CompClass, float, testsSize>(3.f,		9.f,	&curve);
	check<CompClass, float, testsSize>(3.9f,	9.f,	&curve);
	check<CompClass, float, testsSize>(5.f,		16.f,	&curve);
	check<CompClass, float, testsSize>(20.f,	16.f,	&curve);
	check<CompClass, float, testsSize>(4.01f,	16.f,	&curve);
}

void testCompClassClampUp() {
	ParamCurve<CompClass, float, testsSize> curve;
	
	CompClass inputs[testsSize] = { CompClass(0.f), CompClass(1.f), CompClass(2.f), CompClass(3.f), CompClass(4.f) };
	float outputs[testsSize] = { 0.f, 1.f, 4.f, 9.f, 16.f };
	Interpolator<CompClass, float>* interpolator = ClampUpInterpolator<CompClass, float>::getInstance();
	curve.initialize(interpolator, testsSize, inputs, outputs);
	
	check<CompClass, float, testsSize>(-1.f,	0.f,	&curve);
	check<CompClass, float, testsSize>(.0001f,	1.f,	&curve);
	check<CompClass, float, testsSize>(.125f,	1.f,	&curve);
	check<CompClass, float, testsSize>(.25f,	1.f,	&curve);
	check<CompClass, float, testsSize>(.5f,		1.f,	&curve);
	check<CompClass, float, testsSize>(1.f,		4.f,	&curve);
	check<CompClass, float, testsSize>(1.6f,	4.f,	&curve);
	check<CompClass, float, testsSize>(2.f,		9.f,	&curve);
	check<CompClass, float, testsSize>(3.f,		16.f,	&curve);
	check<CompClass, float, testsSize>(3.9f,	16.f,	&curve);
	check<CompClass, float, testsSize>(5.f,		16.f,	&curve);
	check<CompClass, float, testsSize>(20.f,	16.f,	&curve);
	check<CompClass, float, testsSize>(4.01f,	16.f,	&curve);
}

void testCompClassLinear() {
	ParamCurve<CompClass, float, testsSize> curve;
	
	CompClass inputs[testsSize] = { CompClass(0.f), CompClass(1.f), CompClass(2.f), CompClass(3.f), CompClass(4.f) };
	float outputs[testsSize] = { 0.f, 1.f, 4.f, 9.f, 16.f };
	Interpolator<CompClass, float>* interpolator = LinearInterpolator<CompClass, float>::getInstance();
	curve.initialize(interpolator, testsSize, inputs, outputs);
	
	check<CompClass, float, testsSize>(-1.f,	0.f,	&curve);
	check<CompClass, float, testsSize>(0.f,		0.f,	&curve);
	check<CompClass, float, testsSize>(.125f,	.125f,	&curve);
	check<CompClass, float, testsSize>(.25f,	.25f,	&curve);
	check<CompClass, float, testsSize>(.5f,		.5f,	&curve);
	check<CompClass, float, testsSize>(1.f,		1.f,	&curve);
	check<CompClass, float, testsSize>(1.6f,	2.8f,	&curve);
	check<CompClass, float, testsSize>(2.f,		4.f,	&curve);
	check<CompClass, float, testsSize>(3.f,		9.f,	&curve);
	check<CompClass, float, testsSize>(3.9f,	15.3f,	&curve);
	check<CompClass, float, testsSize>(5.f,		16.f,	&curve);
	check<CompClass, float, testsSize>(20.f,	16.f,	&curve);
	check<CompClass, float, testsSize>(4.01f,	16.f,	&curve);
}

class CatmullRomClass {
public:
	float value;

	CatmullRomClass() { value = 0.f; }
	CatmullRomClass(float newValue) { value = newValue; }

	bool operator== (const CatmullRomClass &c2) const { return value == c2.value; }
	bool operator< (const CatmullRomClass &c2) const { return value < c2.value; }
	bool operator> (const CatmullRomClass &c2) const { return value > c2.value; }
	bool operator<= (const CatmullRomClass &c2) const { return value <= c2.value; }
	bool operator>= (const CatmullRomClass &c2) const { return value >= c2.value; }

	// All this return a copy of the resulting CatmullRomClass.
	CatmullRomClass operator+ (const CatmullRomClass &c2) const { return CatmullRomClass( value + c2.value ); }
	CatmullRomClass operator- (const CatmullRomClass &c2) const { return CatmullRomClass( value - c2.value ); }
	CatmullRomClass operator* (const CatmullRomClass &c2) const { return CatmullRomClass( value * c2.value ); }
	CatmullRomClass operator/ (const CatmullRomClass &c2) const { return CatmullRomClass( value / c2.value ); }

	CatmullRomClass operator* (const float &f) const { return CatmullRomClass( value * f ); }
	CatmullRomClass operator/ (const float &f) const { return CatmullRomClass( value / f ); }
	CatmullRomClass operator+ (const float &f) const { return CatmullRomClass( value + f ); }
	CatmullRomClass operator- (const float &f) const { return CatmullRomClass( value - f ); }

	operator float() { return value; }
};

void testCatmullRomFloat() {
	ParamCurve<float, float, 4> curve;
	
	float inputs[4] = { 0.f, 1.f, 2.f, 3.f };
	float outputs[4] = { 1.f, 2.f, 2.f, 1.f };
	Interpolator<float, float>* interpolator = CatmullRomInterpolator<float, float>::getInstance();
	curve.initialize(interpolator, 4, inputs, outputs);
	
	bool first = true;
	for(float i = inputs[0] - .5f; i <= inputs[3] + .5f; i += .1f) {
		if (first) first = false;
		else printf(", ");
		printf (" %f", i);
	}

	printf("\n");
	first = true;
	for(float i = inputs[0] - .5f; i <= inputs[3] + .5f; i += .1f) {
		if (first) first = false;
		else printf(", ");
		printf (" %f", curve.getValue(i));
	}

	printf("\n");
	////check<float, float, 4>( 1.25, 2.171875, &curve );
	////check<float, float, 4>( 1.5, 2.125, &curve );
	////check<float, float, 4>( 1.75, 2.171875, &curve );
}

void testCatmullRom() {
	ParamCurve<float, CatmullRomClass, testsSize> curve;
	
	float inputs[testsSize] = { 0.f, 1.f, 2.f, 3.f, 4.f };
	CatmullRomClass outputs[testsSize] = { CatmullRomClass(0.f), CatmullRomClass(1.f), CatmullRomClass(4.f), CatmullRomClass(9.f), CatmullRomClass(16.f) };
	Interpolator<float, CatmullRomClass>* interpolator = CatmullRomInterpolator<float, CatmullRomClass>::getInstance();
	curve.initialize(interpolator, testsSize, inputs, outputs);
	
	bool first = true;
	for(float i = inputs[0]; i <= inputs[4]; i += .5f) {
		if (first) first = false;
		else printf(", ");
		printf (" %f", i);
	}

	printf("\n");
	first = true;
	for(float i = inputs[0]; i <= inputs[4]; i += .5f) {
		if (first) first = false;
		else printf(", ");
		printf (" %f", curve.getValue(i));
	}

	printf("\n");
}
