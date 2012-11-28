///
/// @file LinearInterpolator.h Implementation of a linear interpolator.
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

#include "Interpolator.h"

///
/// Interpolates linearly between two values, using as first value inputs[index] received.
/// @tparam TInput Input values type. Required operators:
/// TInput operator-(TInput&)
/// TInput operator/(TInput&)
/// @tparam TOutput Output values type. Required operators:
/// TOuput operator+(TOutput&)
/// TOuput operator-(TOutput&)
/// TOutput operator*(float&).
///
template<typename TInput, typename TOutput>
class LinearInterpolator : public Interpolator<TInput, TOutput> {
	LinearInterpolator() : Interpolator<TInput, TOutput>() { this->interpolation = interpolationLinear; }

public:
	static Interpolator<TInput, TOutput>* getInstance() {
		static LinearInterpolator<TInput, TOutput> instance;
		return &instance;
	}

	TOutput interpolate(TInput input, TInput const *inputs, TOutput const *outputs, size_t size, size_t index) {
		if (index < 0) return outputs[0];
		if (index + 1 >= size) return outputs[size-1];

		float ratio = (input - inputs[index]) / (inputs[index+1] - inputs[index]);
		return outputs[index] + ((outputs[index+1] - outputs[index]) * ratio);
	}
};
