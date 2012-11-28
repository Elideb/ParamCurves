CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(curves_SRCS
    CMakeDummy.cpp
)

SET(curves_HDRS
	ParamCurve.h
	Interpolator.h
	ClampInterpolator.h
    ClampUpInterpolator.h
    LinearInterpolator.h
)

ADD_LIBRARY(ParamCurves ${curves_SRCS} ${curves_HDRS})
