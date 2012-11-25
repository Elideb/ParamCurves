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

template<typename TInput, typename TOutput, size_t maxSize>
class ParamCurve {
	Interpolator<TInput, TOutput>* interpolator;
	size_t length;
	TInput inputs[maxSize];
	TOutput outputs[maxSize];

public:
	// Creates a new instance of ParamCurve, with no elements.
	ParamCurve() : length(0) {}

	void initialize(Interpolator<TInput, TOutput>* newInterpolator, size_t newLength, TInput* newInputs, TOutput* newOutputs) {
		if (newLength >= maxSize) length = maxSize;
		else length = newLength;

		interpolator = newInterpolator;

		for(size_t i = 0; i < length; ++i) {
			inputs[i] = newInputs[i];
			outputs[i] = newOutputs[i];
		}
	}

	TOutput getValue(TInput input) const {
		if(length == 0) return 0;
		if (input <= inputs[0]) return outputs[0];
		if (input >= inputs[length-1]) return outputs[length-1];

		for(size_t i = 0; i < length; ++i) {
			if (inputs[i] <= input && input < inputs[i+1]) {
				return interpolator->interpolate(input, inputs, outputs, i);
			}
		}

		return outputs[length-1];
	}
};
