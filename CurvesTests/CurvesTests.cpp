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

void testLinear();
void testClamp();
void testCompNonDivClamp();
void testCompNonDivLinear();
void testCompClassClamp();
void testCompClassLinear();

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
	printf("\nTesting div linear interpolation:\n");
	testCompClassLinear();

	return 0;
}

template<typename T>
bool almostEqual(T v1, T v2) {
	if (v2 > v1)
		return v2 - v1 < .0001;
	else
		return v1 - v2 < .0001;
}

template<typename TInput, typename TOutput>
bool check(TInput input, TOutput expectedOutput, ParamCurve<TInput, TOutput, testsSize>* const curve) {
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
	float inputs[5] = { 0.f, .5f, 1.5f, 2.f, 5.f };
	float outputs[5] = { 4.f, 3.f, 4.f, 5.f, 5.f };
	Interpolator<float, float>* interpolator = new LinearInterpolator<float, float>();
	curve.initialize(interpolator, 5, inputs, outputs);

	check<float, float>(-1.f,	4.f,	&curve);
	check<float, float>(0.f,	4.f,	&curve);
	check<float, float>(0.000001f,	4.f,	&curve);
	check<float, float>(.125f,	3.75f,	&curve);
	check<float, float>(.25f,	3.5f,	&curve);
	check<float, float>(.5f,	3.f,	&curve);
	check<float, float>(1.f,	3.5f,	&curve);
	check<float, float>(1.6f,	4.2f,	&curve);
	check<float, float>(2.f,	5.f,	&curve);
	check<float, float>(3.f,	5.f,	&curve);
	check<float, float>(4.f,	5.f,	&curve);
	check<float, float>(5.f,	5.f,	&curve);
	check<float, float>(20.f,	5.f,	&curve);
	check<float, float>(5.01f,	5.f,	&curve);

	delete(interpolator);
}

void testClamp() {
	ParamCurve<float, float, testsSize> curve;
	float inputs[5] = { 0.f, .5f, 1.5f, 2.f, 5.f };
	float outputs[5] = { 4.f, 3.f, 4.f, 5.f, 5.f };
	Interpolator<float, float>* interpolator = new ClampInterpolator<float, float>();
	curve.initialize( interpolator, 5, inputs, outputs );

	check<float, float>(-1.f,	4.f,	&curve);
	check<float, float>(0.f,	4.f,	&curve);
	check<float, float>(.125f,	4.f,	&curve);
	check<float, float>(.25f,	4.f,	&curve);
	check<float, float>(.5f,	3.f,	&curve);
	check<float, float>(1.f,	3.f,	&curve);
	check<float, float>(1.6f,	4.f,	&curve);
	check<float, float>(2.f,	5.f,	&curve);
	check<float, float>(3.f,	5.f,	&curve);
	check<float, float>(4.f,	5.f,	&curve);
	check<float, float>(5.f,	5.f,	&curve);
	check<float, float>(20.f,	5.f,	&curve);
	check<float, float>(5.01f,	5.f,	&curve);

	delete(interpolator);
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
	CompNonDivClass inputs[5] = { CompNonDivClass(0.f), CompNonDivClass(1.f), CompNonDivClass(2.f), CompNonDivClass(3.f), CompNonDivClass(4.f) };
	float outputs[5] = { 4.f, 3.f, 4.f, 5.f, 5.f };
	Interpolator<CompNonDivClass, float>* interpolator = new ClampInterpolator<CompNonDivClass, float>();
	curve.initialize(interpolator, 5, inputs, outputs);
	
	check<CompNonDivClass, float>(-1.f,			4.f,	&curve);
	check<CompNonDivClass, float>(0.f,			4.f,	&curve);
	check<CompNonDivClass, float>(0.000001f,	4.f,	&curve);
	check<CompNonDivClass, float>(.125f,		4.f,	&curve);
	check<CompNonDivClass, float>(.25f,			4.f,	&curve);
	check<CompNonDivClass, float>(.5f,			4.f,	&curve);
	check<CompNonDivClass, float>(1.f,			3.f,	&curve);
	check<CompNonDivClass, float>(1.6f,			3.f,	&curve);
	check<CompNonDivClass, float>(2.f,			4.f,	&curve);
	check<CompNonDivClass, float>(3.f,			5.f,	&curve);
	check<CompNonDivClass, float>(4.f,			5.f,	&curve);
	check<CompNonDivClass, float>(5.f,			5.f,	&curve);
	check<CompNonDivClass, float>(20.f,			5.f,	&curve);
	check<CompNonDivClass, float>(5.01f,		5.f,	&curve);

	delete(interpolator);
}

void testCompNonDivLinear() {
	// This should fail to compile because linear interpolator requires Output to support operators +, -, *, / and float
	////ParamCurve<float, CompNonDivClass> curve;
	////
	////float inputs[5] = { 0.f, 1.f, 2.f, 3.f, 4.f };
	////CompNonDivClass outputs[5] = { CompNonDivClass(0.f), CompNonDivClass(1.f), CompNonDivClass(2.f), CompNonDivClass(3.f), CompNonDivClass(4.f) };
	////Interpolator<float, CompNonDivClass>* interpolator = new LinearInterpolator<float, CompNonDivClass>();
	////curve.initialize(interpolator, 5, inputs, outputs);
	////
	////check<float, CompNonDivClass>(.5f, .5f, &curve);
	////
	////delete(interpolator);
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
	
	CompClass inputs[5] = { CompClass(0.f), CompClass(1.f), CompClass(2.f), CompClass(3.f), CompClass(4.f) };
	float outputs[5] = { 0.f, 1.f, 4.f, 9.f, 16.f };
	Interpolator<CompClass, float>* interpolator = new ClampInterpolator<CompClass, float>();
	curve.initialize(interpolator, 5, inputs, outputs);
	
	check<CompClass, float>(-1.f,	0.f,	&curve);
	check<CompClass, float>(0.f,	0.f,	&curve);
	check<CompClass, float>(.125f,	0.f,	&curve);
	check<CompClass, float>(.25f,	0.f,	&curve);
	check<CompClass, float>(.5f,	0.f,	&curve);
	check<CompClass, float>(1.f,	1.f,	&curve);
	check<CompClass, float>(1.6f,	1.f,	&curve);
	check<CompClass, float>(2.f,	4.f,	&curve);
	check<CompClass, float>(3.f,	9.f,	&curve);
	check<CompClass, float>(3.9f,	9.f,	&curve);
	check<CompClass, float>(5.f,	16.f,	&curve);
	check<CompClass, float>(20.f,	16.f,	&curve);
	check<CompClass, float>(4.01f,	16.f,	&curve);
	
	delete(interpolator);
}

void testCompClassLinear() {
	ParamCurve<CompClass, float, testsSize> curve;
	
	CompClass inputs[5] = { CompClass(0.f), CompClass(1.f), CompClass(2.f), CompClass(3.f), CompClass(4.f) };
	float outputs[5] = { 0.f, 1.f, 4.f, 9.f, 16.f };
	Interpolator<CompClass, float>* interpolator = new LinearInterpolator<CompClass, float>();
	curve.initialize(interpolator, 5, inputs, outputs);
	
	check<CompClass, float>(-1.f,	0.f,	&curve);
	check<CompClass, float>(0.f,	0.f,	&curve);
	check<CompClass, float>(.125f,	.125f,	&curve);
	check<CompClass, float>(.25f,	.25f,	&curve);
	check<CompClass, float>(.5f,	.5f,	&curve);
	check<CompClass, float>(1.f,	1.f,	&curve);
	check<CompClass, float>(1.6f,	2.8f,	&curve);
	check<CompClass, float>(2.f,	4.f,	&curve);
	check<CompClass, float>(3.f,	9.f,	&curve);
	check<CompClass, float>(3.9f,	15.3f,	&curve);
	check<CompClass, float>(5.f,	16.f,	&curve);
	check<CompClass, float>(20.f,	16.f,	&curve);
	check<CompClass, float>(4.01f,	16.f,	&curve);
	
	delete(interpolator);
}
