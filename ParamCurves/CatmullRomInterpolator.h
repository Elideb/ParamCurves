///
/// @file CatmullRomInterpolator.h Implementation of a linear interpolator.
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
/// Interpolates smoothly between two values, using as first value inputs[index] received.
/// It requires two points to interpolate between, plus two control points to ensure smoothness.
/// @tparam TInput Input values type. Required operators:
/// TInput operator<=(TInput&)
/// TInput operator<(TInput&)
/// TInput operator-(TInput&)
/// TInput operator/(TInput&)
/// @tparam TOutput Output values type. Required operators:
/// TOutput operator*(float&)
///
template<typename TInput, typename TOutput>
class CatmullRomInterpolator : public Interpolator<TInput, TOutput> {
	CatmullRomInterpolator() : Interpolator<TInput, TOutput>() { this->interpolation = interpolationCatmullRom; }

public:
	static Interpolator<TInput, TOutput>* getInstance() {
		static CatmullRomInterpolator<TInput, TOutput> instance;
		return &instance;
	}

	TOutput interpolate(TInput input, TInput const *inputs, TOutput const *outputs, size_t size) {
		if (size == 0) return 0;
		if (input <= inputs[0]) return outputs[0];
		if (inputs[size-1] <= input) return outputs[size-1];

		for(size_t i = 0; i < size; ++i) {
			if (inputs[i] <= input && input < inputs[i+1]) {
				// First control point
				TOutput c1 = (i > 0) ? outputs[i-1] : outputs[i];
				// First point
				TOutput v1 = outputs[i];
				// Second point
				TOutput v2 = (i < size - 1) ? outputs[i+1] : outputs[size-1];
				// Second control point
				TOutput c2 = (i < size - 2) ? outputs[i+2] : outputs[size-1];

				float ratio = (input - inputs[i]) / (inputs[i+1] - inputs[i]);
				return .5f * ((2.f * v1)
					+ (v2 - c1) * ratio
					+ (2.f * c1 - 5.f * v1 + 4.f * v2 - c2) * ratio * ratio
					+ (3.f * v1 - c1 - 3.f * v2 + c2) * ratio * ratio * ratio);


				////return c1 * ((-ratio + 2.f) * ratio - 1.f) * ratio * .5f
				////	+ v1 * (((3.f * ratio - 5.f) * ratio) * ratio + 2.f) * .5f
				////	+ v2 * ((-3.f * ratio + 4.f) * ratio + 1.f) * ratio * .5f
				////	+ c2 * ((ratio - 1.f) * ratio * ratio) * .5f;
			}
		}

		// Only reachable with malformed inputs
		return outputs[size-1];
	}
};
