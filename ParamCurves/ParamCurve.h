///
/// @file ParamCurve.h Implementation of a parameterized response curve.
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
/// Stores a parameterized curve and returns the output values corresponding
/// to a specific input value.
/// @tparam TInput Input values type. Required operators.
/// TInput operator<=(TInput&)
/// TInput operator<(TInput&)
/// Other operators may be required, depending on chosen interpolator.
/// @tparam TOutput Output values type. Requires only operators needed for
/// the chosen interpolator.
///
template<typename TInput, typename TOutput, size_t maxSize>
class ParamCurve {
	Interpolator<TInput, TOutput>* interpolator;
	size_t length;
	TInput inputs[maxSize];
	TOutput outputs[maxSize];

public:
	///
	/// Creates a new instance of ParamCurve, with no elements.
	///
	ParamCurve() : length(0) {}

	///
	/// Initialize the curve with the desired input and output values, plus the interpolator.
	/// @param newInterpolator Interpolator desired to calculate values in subsequent calls.
	/// @param newLength Number of input and output elements to store in the curve.
	/// @param newInputs Values to use as source for value calculations.
	/// @param newOutputs Values to interpolate between when calculating results.
	///
	void initialize(Interpolator<TInput, TOutput>* newInterpolator, size_t newLength, TInput* newInputs, TOutput* newOutputs) {
		if (newLength >= maxSize) length = maxSize;
		else length = newLength;

		interpolator = newInterpolator;

		for(size_t i = 0; i < length; ++i) {
			inputs[i] = newInputs[i];
			outputs[i] = newOutputs[i];
		}
	}

	///
	/// Obtain the minimum input value in store.
	/// @return First input value, if any; 0 if no input values.
	///
	TInput getLeftBound() const {
		if (length == 0) return 0;
		return inputs[0];
	}

	///
	/// Obtain the maximum input value in store.
	/// @return Last input value, if any; 0 if no input values.
	///
	TInput getRightBound() const {
		if (length == 0) return 0;
		return inputs[maxSize - 1];
	}

	///
	/// Obtain the output value corresponding to the input received,
	/// calculated using the selected interpolator.
	/// @param input The value to calculate an output for.
	/// @return The output matching the requested input and selected interpolator.
	///
	TOutput getValue(TInput input) const {
		return interpolator->interpolate(input, inputs, outputs, length);
	}
};
