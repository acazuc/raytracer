#ifndef VEC2_H
# define VEC2_H

template <typename T> class TVec2
{

	public:
		union {T x, u;};
		union {T y, v;};
		inline TVec2<T>(T x, T y) : x(x), y(y) {};
		inline TVec2<T>(T xy) : x(xy), y(xy) {};
		inline TVec2<T>() {};
		T &operator [] (int idx);
		TVec2<T> operator - ();
		TVec2<T> operator += (T val);
		TVec2<T> operator -= (T val);
		TVec2<T> operator *= (T val);
		TVec2<T> operator /= (T val);
		TVec2<T> operator += (TVec2<T> vec);
		TVec2<T> operator -= (TVec2<T> vec);
		TVec2<T> operator *= (TVec2<T> vec);
		TVec2<T> operator /= (TVec2<T> vec);
		bool operator == (TVec2<T> vec);
		bool operator != (TVec2<T> vec);

};

template <typename T>
TVec2<T> operator + (TVec2<T> vec1, TVec2<T> vec2);
template <typename T>
TVec2<T> operator + (TVec2<T> vec, T val);
template <typename T>
TVec2<T> operator + (T val, TVec2<T> vec);
template <typename T>
TVec2<T> operator - (TVec2<T> vec1, TVec2<T> vec2);
template <typename T>
TVec2<T> operator - (TVec2<T> vec, T val);
template <typename T>
TVec2<T> operator - (T val, TVec2<T> vec);
template <typename T>
TVec2<T> operator * (TVec2<T> vec1, TVec2<T> vec2);
template <typename T>
TVec2<T> operator * (TVec2<T> vec, T val);
template <typename T>
TVec2<T> operator * (T val, TVec2<T> vec);
template <typename T>
TVec2<T> operator / (TVec2<T> vec1, TVec2<T> vec2);
template <typename T>
TVec2<T> operator / (TVec2<T> vec, T val);
template <typename T>
TVec2<T> operator / (T val, TVec2<T> vec);

template <typename T>
TVec2<T> min(TVec2<T> vec1, TVec2<T> vec2);
template <typename T>
TVec2<T> min(TVec2<T> vec, T val);
template <typename T>
TVec2<T> min(T val, TVec2<T> vec);

template <typename T>
TVec2<T> max(TVec2<T> vec1, TVec2<T> vec2);
template <typename T>
TVec2<T> max(TVec2<T> vec, T val);
template <typename T>
TVec2<T> max(T val, TVec2<T> vec);

template <typename T>
TVec2<T> clamp(TVec2<T> vec, T min, T max);
template <typename T>
TVec2<T> clamp(TVec2<T> vec, TVec2<T> min, TVec2<T> max);

template <typename T>
TVec2<T> mix(TVec2<T> vec1, TVec2<T> vec2, T per);
template <typename T>
TVec2<T> mod(TVec2<T> vec, T val);
template <typename T>
TVec2<T> floor(TVec2<T> vec);
template <typename T>
TVec2<T> round(TVec2<T> vec);
template <typename T>
TVec2<T> ceil(TVec2<T> vec);
template <typename T>
TVec2<T> fract(TVec2<T> vec);

template<typename T>
TVec2<T> normalize(TVec2<T> vec);
template<typename T>
TVec2<T> reflect(TVec2<T> vec1, TVec2<T> vec2);
template<typename T>
T dot(TVec2<T> vec1, TVec2<T> vec2);
template<typename T>
T angle(TVec2<T> vec1, TVec2<T> vec2);
template<typename T>
T length(TVec2<T> vec);

typedef TVec2<float> Vec2;

# include "Vec2.cpp"

#endif
