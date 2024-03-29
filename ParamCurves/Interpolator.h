///
/// @file Interpolator.h Abstract definition of an interpolator.
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

#pragma once

enum t_interpolationMode {
	interpolationClamp
	, interpolationClampUp
	, interpolationLinear
	, interpolationCatmullRom
	, interpolationSmooth = interpolationCatmullRom
};

///
/// Abstract implementation of an interpolator.
/// @tparam TInput Input values type.
/// @tparam TOutput Output values type.
///
template<typename TInput, typename TOutput>
class Interpolator {

protected:
	t_interpolationMode interpolation;

protected:
	Interpolator() {}

public:
	t_interpolationMode getInterpolationMode() { return interpolation; }

	virtual TOutput interpolate(TInput input, TInput const *inputs, TOutput const *outputs, size_t size) = 0;
};
