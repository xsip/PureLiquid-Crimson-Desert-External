#pragma once
// used: [crt] isfinite, fmodf, remainderf
#include <cmath>
#include <Math/Definitions.h>
#include <Math/Vector.h>
#include <Math/Core.h>

// forward declarations
struct Matrix3x4_t;

struct MATH_API QAngle_t
{
	constexpr QAngle_t(float x = 0.f, float y = 0.f, float z = 0.f);
	constexpr QAngle_t(const float* arrAngles);

#pragma region qangle_array_operators
	[[nodiscard]] float& operator[](const int nIndex);
	[[nodiscard]] const float& operator[](const int nIndex) const;
#pragma endregion

#pragma region qangle_relational_operators
	bool operator==(const QAngle_t& angBase) const;
	bool operator!=(const QAngle_t& angBase) const;
#pragma endregion

#pragma region qangle_assignment_operators
	constexpr QAngle_t& operator=(const QAngle_t& angBase);
#pragma endregion

#pragma region qangle_arithmetic_assignment_operators
	constexpr QAngle_t& operator+=(const QAngle_t& angBase);
	constexpr QAngle_t& operator-=(const QAngle_t& angBase);
	constexpr QAngle_t& operator*=(const QAngle_t& angBase);
	constexpr QAngle_t& operator/=(const QAngle_t& angBase);
	constexpr QAngle_t& operator+=(const float flAdd);
	constexpr QAngle_t& operator-=(const float flSubtract);
	constexpr QAngle_t& operator*=(const float flMultiply);
	constexpr QAngle_t& operator/=(const float flDivide);
#pragma endregion

#pragma region qangle_arithmetic_unary_operators
	constexpr QAngle_t& operator-();
	constexpr QAngle_t operator-() const;
#pragma endregion

#pragma region qangle_arithmetic_ternary_operators
	constexpr QAngle_t operator+(const QAngle_t& angAdd) const;
	constexpr QAngle_t operator-(const QAngle_t& angSubtract) const;
	constexpr QAngle_t operator*(const QAngle_t& angMultiply) const;
	constexpr QAngle_t operator/(const QAngle_t& angDivide) const;
	constexpr QAngle_t operator+(const float flAdd) const;
	constexpr QAngle_t operator-(const float flSubtract) const;
	constexpr QAngle_t operator*(const float flMultiply) const;
	constexpr QAngle_t operator/(const float flDivide) const;
#pragma endregion

	// @returns : true if each component of angle is finite, false otherwise
	[[nodiscard]] bool IsValid() const;

	/// @returns: true if each component of angle equals to another, false otherwise
	[[nodiscard]] bool IsEqual(const QAngle_t& angEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon()) const;

	/// @returns: true if each component of angle equals zero, false otherwise
	[[nodiscard]] bool IsZero() const;

	/// @returns: length of hypotenuse
	[[nodiscard]] float Length2D() const;

	/// clamp each angle component by minimal/maximal allowed value for source sdk games
	/// @returns: clamped angle
	constexpr QAngle_t& ClampAngle();

	/// map polar angles to the range of [-180, 180] degrees
	/// @returns: normalized angle
	QAngle_t& Normalize();

	/// convert angle to direction vectors
	/// @param[out] pvecForward [optional] output for converted forward vector
	/// @param[out] pvecRight [optional] output for converted right vector
	/// @param[out] pvecUp [optional] output for converted up vector
	void ToDirections(Vector* pvecForward, Vector* pvecRight = nullptr, Vector* pvecUp = nullptr) const;

	float DistanceTo(const QAngle_t& other) const;
	Vector AngleToDirection();

	/// @param[in] vecOrigin [optional] origin for converted matrix
	/// @returns: matrix converted from angle
	[[nodiscard]] Matrix3x4_t ToMatrix(const Vector& vecOrigin = {}) const;

#pragma region qangle_conversion_helpers
	/// Convert radians to degrees (static helper)
	/// @param[in] radians value in radians
	/// @returns: value in degrees
	[[nodiscard]] static constexpr float RadiansToDegrees(float radians)
	{
		return radians * 57.295779513082320876f; // 180/π - inline constant
	}

	/// Convert degrees to radians (static helper)
	/// @param[in] degrees value in degrees
	/// @returns: value in radians
	[[nodiscard]] static constexpr float DegreesToRadians(float degrees)
	{
		return degrees * 0.017453292519943295769f; // π/180 - inline constant
	}

	/// Convert this angle from radians to degrees (modifies in place)
	/// @returns: reference to this angle (now in degrees)
	constexpr QAngle_t& FromRadians()
	{
		this->x *= 57.295779513082320876f;
		this->y *= 57.295779513082320876f;
		this->z *= 57.295779513082320876f;
		return *this;
	}

	/// Convert this angle from degrees to radians (modifies in place)
	/// @returns: reference to this angle (now in radians)
	constexpr QAngle_t& ToRadians()
	{
		this->x *= 0.017453292519943295769f;
		this->y *= 0.017453292519943295769f;
		this->z *= 0.017453292519943295769f;
		return *this;
	}

	/// Get a copy of this angle converted from radians to degrees
	/// @returns: new angle in degrees
	[[nodiscard]] constexpr QAngle_t AsDegrees() const
	{
		return QAngle_t(
			this->x * 57.295779513082320876f,
			this->y * 57.295779513082320876f,
			this->z * 57.295779513082320876f
		);
	}

	/// Get a copy of this angle converted from degrees to radians
	/// @returns: new angle in radians
	[[nodiscard]] constexpr QAngle_t AsRadians() const
	{
		return QAngle_t(
			this->x * 0.017453292519943295769f,
			this->y * 0.017453292519943295769f,
			this->z * 0.017453292519943295769f
		);
	}
#pragma endregion

public:
	float x = 0.0f, y = 0.0f, z = 0.0f;
};

using QAngle = QAngle_t;