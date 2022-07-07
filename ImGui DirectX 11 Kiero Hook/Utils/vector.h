#pragma once

#define M_PI	3.14159265358979323846264338327950288419716939937510
typedef struct _D3DMATRIX2 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX2;


class Vector2
{
public:
	float x;
	float y;
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	~Vector2() {}
	inline float Distance(Vector2 v)
	{
		return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
	}
	__forceinline Vector2 operator-(const Vector2& V) {
		return Vector2(x - V.x, y - V.y);
	}

	__forceinline Vector2 operator+(const Vector2& V) {
		return Vector2(x + V.x, y + V.y);
	}

	__forceinline Vector2 operator*(float Scale) const {
		return Vector2(x * Scale, y * Scale);
	}

	__forceinline Vector2 operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return Vector2(x * RScale, y * RScale);
	}

	__forceinline Vector2 operator+(float A) const {
		return Vector2(x + A, y + A);
	}

	__forceinline Vector2 operator-(float A) const {
		return Vector2(x - A, y - A);
	}

	__forceinline Vector2 operator*(const Vector2& V) const {
		return Vector2(x * V.x, y * V.y);
	}


	__forceinline Vector2 operator/(const Vector2& V) const {
		return Vector2(x / V.x, y / V.y);
	}

	__forceinline float operator|(const Vector2& V) const {
		return x * V.x + y * V.y;
	}

	__forceinline float operator^(const Vector2& V) const {
		return x * V.y - y * V.x;
	}

	__forceinline Vector2& operator+=(const Vector2& v) {
		x += v.x; y += v.x; return *this;
	}

	__forceinline Vector2& operator-=(const Vector2& v) {
		x -= v.x; y -= v.y; return *this;
	}

	__forceinline Vector2& operator*=(const Vector2& v) {
		x *= v.x; y *= v.y; return *this;
	}

	__forceinline Vector2& operator/=(const Vector2& v) {
		x /= v.x; y /= v.y; return *this;
	}

	__forceinline bool operator==(const Vector2& src) const {
		return (src.x == x) && (src.y == y);
	}

	__forceinline bool operator!=(const Vector2& src) const {
		return (src.x != x || (src.y != y));
	}
	__forceinline bool operator>(const Vector2& src) const {
		return (x > src.x && (y > src.y));
	}
	__forceinline bool operator<(const Vector2& src) const {
		return (x < src.x && (y < src.y));
	}
};
class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v, float Div)
	{
		return float(sqrtf(pow(x - v.x, 2.0) + pow(y - v.y, 2.0) + pow(z - v.z, 2.0)) / Div);
	}
	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}
	inline float Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	inline Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	inline Vector3& operator*=(const Vector3& v) {
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}

	inline Vector3& operator/=(const Vector3& v) {
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}

	inline Vector3& operator+=(float v) {
		x += v; y += v; z += v; return *this;
	}

	inline Vector3& operator-=(float v) {
		x -= v; y -= v; z -= v; return *this;
	}

	inline Vector3& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}

	inline Vector3& operator/=(float v) {
		x /= v; y /= v; z /= v; return *this;
	}

	inline Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	inline Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 operator*(const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	inline Vector3 operator/(const Vector3& v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	inline Vector3 operator+(float v) const {
		return Vector3(x + v, y + v, z + v);
	}

	inline Vector3 operator-(float v) const {
		return Vector3(x - v, y - v, z - v);
	}

	inline Vector3 operator*(float v) const {
		return Vector3(x * v, y * v, z * v);
	}

	inline Vector3 operator/(float v) const {
		return Vector3(x / v, y / v, z / v);
	}
	__forceinline bool operator==(const Vector3& src) const {
		return (src.x == x) && (src.y == y) && (src.z == z);
	}
};

class Vector4
{
public:
	Vector4() : x(0.f), y(0.f), w(0.f), h(0.f)
	{

	}

	Vector4(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h)
	{

	}
	~Vector4()
	{

	}

	float x;
	float y;
	float w;
	float h;

	inline float Dot(Vector4 v)
	{
		return x * v.x + y * v.y + w * v.w + h * v.h;
	}

	inline float Distance(Vector4 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.w - w, 2.0) + powf(v.h - h, 2.0)));
	}
	inline float Length()
	{
		return sqrtf(x * x + y * y + w * w + h * h);
	}
	Vector4 operator+(Vector4 v)
	{
		return Vector4(x + v.x, y + v.y, w + v.w, h + v.h);
	}

	Vector4 operator-(Vector4 v)
	{
		return Vector4(x - v.x, y - v.y, w - v.w, h - v.h);
	}
};
struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};

struct FTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];
	D3DMATRIX2 ToMatrixWithScale()
	{
		D3DMATRIX2 m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
	static D3DMATRIX2 MatrixMultiplication(D3DMATRIX2 pM1, D3DMATRIX2 pM2) {

		D3DMATRIX2 pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}
	D3DMATRIX2 Matrix2(Vector3 rot, Vector3 origin = Vector3(0, 0, 0)) {

		float radPitch = (rot.x * float(M_PI) / 180.f);
		float radYaw = (rot.y * float(M_PI) / 180.f);
		float radRoll = (rot.z * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX2 matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

};
enum ETraceTypeQuery
{
	TraceTypeQuery1,
	TraceTypeQuery2,
	TraceTypeQuery3,
	TraceTypeQuery4,
	TraceTypeQuery5,
	TraceTypeQuery6,
	TraceTypeQuery7,
	TraceTypeQuery8,
	TraceTypeQuery9,
	TraceTypeQuery10,
	TraceTypeQuery11,
	TraceTypeQuery12,
	TraceTypeQuery13,
	TraceTypeQuery14,
	TraceTypeQuery15,
	TraceTypeQuery16,
	TraceTypeQuery17,
	TraceTypeQuery18,
	TraceTypeQuery19,
	TraceTypeQuery20,
	TraceTypeQuery21,
	TraceTypeQuery22,
	TraceTypeQuery23,
	TraceTypeQuery24,
	TraceTypeQuery25,
	TraceTypeQuery26,
	TraceTypeQuery27,
	TraceTypeQuery28,
	TraceTypeQuery29,
	TraceTypeQuery30,
	TraceTypeQuery31,
	TraceTypeQuery32,

	TraceTypeQuery_MAX
};
enum EDrawDebugTrace
{
	None,
	ForOneFrame,
	ForDuration,
	Persistent

};
struct FHitResult
{

	char pad_88[0x88];


};
struct FLinearColor
{
	float	R,
		G,
		B,
		A;
	inline FLinearColor()
		: R(0), G(0), B(0), A(0)
	{ }

	inline FLinearColor(float r, float g, float b, float a)
		: R(r),
		G(g),
		B(b),
		A(a)
	{ }


};