#ifndef ALGEBRA_HPP
#define ALGEBRA_HPP

#include <iostream>
#include <cmath>
#include <cstdio> // Para sprintf em Mat4::operator<<

// =============================================================================
// Namespace & Forward Declarations
// =============================================================================

namespace alg {

// Forward declarations
struct Vec3;
struct Mat4;
struct Quat;

// --- Utility Constants & Functions ---

constexpr float PI = 3.14159265359f;

inline float degrees_to_radians(float degrees) noexcept {
    return degrees * PI / 180.0f;
}

inline float radians_to_degrees(float radians) noexcept {
    return radians * 180.0f / PI;
}


// =============================================================================
// Vec2 Struct
// =============================================================================

    struct Vec2 {
    float x = 0.0f, y = 0.0f;
};

// =============================================================================
// Vec3 Struct
// =============================================================================

struct Vec3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    // --- Constructors ---
    Vec3() = default;
    constexpr Vec3(float x_in, float y_in, float z_in) noexcept : x(x_in), y(y_in), z(z_in) {}

    // --- Member Operators ---
    Vec3& operator+=(const Vec3& other) noexcept { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3& operator-=(const Vec3& other) noexcept { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vec3& operator*=(float scalar) noexcept { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vec3& operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    Vec3 operator+(const Vec3& other) const noexcept { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const noexcept { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(float scalar) const noexcept { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3 operator/(float scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }
    Vec3 operator-() const noexcept { return Vec3(-x, -y, -z); }

    // --- Member Methods ---
    float magnitude() const noexcept { return std::sqrt(x*x + y*y + z*z); }
    float magnitude_squared() const noexcept { return x*x + y*y + z*z; }

    void normalize() {
        const float mag = magnitude();
        if (mag > 0.0f) { *this /= mag; }
    }

    Vec3 normalized() const {
        const float mag = magnitude();
        if (mag > 0.0f) { return *this / mag; }
        return Vec3(0.0f, 0.0f, 0.0f);
    }
};

// --- Vec3 Free Functions ---
inline Vec3 operator*(float scalar, const Vec3& v) noexcept { return v * scalar; }
inline float dot(const Vec3& a, const Vec3& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z; }

inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
    return Vec3( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// =============================================================================
// Mat4 Struct
// =============================================================================

struct Mat4 {
    // Stored in column-major order: m[column * 4 + row]
    float m[16];

    // --- Constructor (defaults to identity matrix) ---
    Mat4() noexcept {
        m[0]=1; m[4]=0; m[8]=0;  m[12]=0;
        m[1]=0; m[5]=1; m[9]=0;  m[13]=0;
        m[2]=0; m[6]=0; m[10]=1; m[14]=0;
        m[3]=0; m[7]=0; m[11]=0; m[15]=1;
    }

    // --- Static Factory Methods ---
    static Mat4 create_translation(const Vec3& translation) noexcept;
    static Mat4 create_scale(const Vec3& scale) noexcept;
    static Mat4 create_rotation_x(float angle_rad) noexcept;
    static Mat4 create_rotation_y(float angle_rad) noexcept;
    static Mat4 create_rotation_z(float angle_rad) noexcept;
    static Mat4 create_rotation_xyz(const Vec3& angles_rad) noexcept;

    static Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) noexcept;

    static Mat4 create_perspective(float fov_deg, float aspect_ratio, float near_plane, float far_plane) noexcept;
};

// --- Mat4 Free Functions ---

/**
 * @brief Multiplies two column-major Mat4 matrices (a * b).
 * @note The logic has been corrected. For C_ij = sum(A_ik * B_kj),
 * in column-major access this is C[j][i] = sum(A[k][i] * B[j][k]).
 */
inline Mat4 operator*(const Mat4& a, const Mat4& b) noexcept {
    Mat4 result;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            result.m[col * 4 + row] = sum;
        }
    }
    return result;
}

/**
 * @brief Transforms a Vec3 (point) by a Mat4 matrix.
 * @note Assumes the vector's w-component is 1.0.
 */
inline Vec3 operator*(const Mat4& m, const Vec3& v) noexcept {
    return Vec3(
        m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12],
        m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13],
        m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14]
    );
}

inline std::ostream& operator<<(std::ostream& os, const Mat4& mat) {
    char buffer[256];
    // Printing in row-major format for human readability
    snprintf(buffer, 256, "[ %7.2f %7.2f %7.2f %7.2f ]\n[ %7.2f %7.2f %7.2f %7.2f ]\n[ %7.2f %7.2f %7.2f %7.2f ]\n[ %7.2f %7.2f %7.2f %7.2f ]",
        mat.m[0], mat.m[4], mat.m[8], mat.m[12],
        mat.m[1], mat.m[5], mat.m[9], mat.m[13],
        mat.m[2], mat.m[6], mat.m[10], mat.m[14],
        mat.m[3], mat.m[7], mat.m[11], mat.m[15]);
    os << buffer;
    return os;
}

// --- Mat4 Static Method Definitions ---

inline Mat4 Mat4::create_translation(const Vec3& t) noexcept {
    Mat4 mat;
    mat.m[12] = t.x; mat.m[13] = t.y; mat.m[14] = t.z;
    return mat;
}

inline Mat4 Mat4::create_scale(const Vec3& s) noexcept {
    Mat4 mat;
    mat.m[0] = s.x; mat.m[5] = s.y; mat.m[10] = s.z;
    return mat;
}

inline Mat4 Mat4::create_rotation_x(float angle_rad) noexcept {
    Mat4 mat;
    const float c = std::cos(angle_rad), s = std::sin(angle_rad);
    mat.m[5] = c;  mat.m[9] = -s;
    mat.m[6] = s;  mat.m[10] = c;
    return mat;
}

inline Mat4 Mat4::create_rotation_y(float angle_rad) noexcept {
    Mat4 mat;
    const float c = std::cos(angle_rad), s = std::sin(angle_rad);
    mat.m[0] = c;  mat.m[8] = s;
    mat.m[2] = -s; mat.m[10] = c;
    return mat;
}

inline Mat4 Mat4::create_rotation_z(float angle_rad) noexcept {
    Mat4 mat;
    const float c = std::cos(angle_rad), s = std::sin(angle_rad);
    mat.m[0] = c;  mat.m[4] = -s;
    mat.m[1] = s;  mat.m[5] = c;
    return mat;
}

/**
 * @brief Creates a combined rotation matrix from Euler angles (in radians).
 * @note The order of application is Y (Yaw), then X (Pitch), then Z (Roll).
 * A vector v is transformed as (Yaw * Pitch * Roll) * v.
 */
inline Mat4 Mat4::create_rotation_xyz(const Vec3& angles_rad) noexcept {
    const Mat4 rot_y = Mat4::create_rotation_y(angles_rad.y); // Yaw
    const Mat4 rot_x = Mat4::create_rotation_x(angles_rad.x); // Pitch
    const Mat4 rot_z = Mat4::create_rotation_z(angles_rad.z); // Roll
    return rot_y * rot_x * rot_z;
}

    inline Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up) noexcept {
    const Vec3 z_axis = (eye - target).normalized();
    const Vec3 x_axis = cross(up, z_axis).normalized();
    const Vec3 y_axis = cross(z_axis, x_axis); // Não precisa normalizar, já é unitário

    Mat4 view_matrix;

    // Montagem correta em ordem de coluna-maior
    // Coluna 0
    view_matrix.m[0] = x_axis.x;
    view_matrix.m[1] = x_axis.y;
    view_matrix.m[2] = x_axis.z;
    view_matrix.m[3] = 0.0f;

    // Coluna 1
    view_matrix.m[4] = y_axis.x;
    view_matrix.m[5] = y_axis.y;
    view_matrix.m[6] = y_axis.z;
    view_matrix.m[7] = 0.0f;

    // Coluna 2
    view_matrix.m[8] = z_axis.x;
    view_matrix.m[9] = z_axis.y;
    view_matrix.m[10] = z_axis.z;
    view_matrix.m[11] = 0.0f;

    // Coluna 3
    view_matrix.m[12] = -dot(x_axis, eye);
    view_matrix.m[13] = -dot(y_axis, eye);
    view_matrix.m[14] = -dot(z_axis, eye);
    view_matrix.m[15] = 1.0f;

    return view_matrix;
}

    inline Mat4 Mat4::create_perspective(float fov_rad, float aspect_ratio, float near_plane, float far_plane) noexcept {
    Mat4 mat{}; // Zero initialize
    const float tan_half_fov = std::tan(fov_rad / 2.0f);

    mat.m[0] = 1.0f / (aspect_ratio * tan_half_fov);
    mat.m[5] = 1.0f / (tan_half_fov);
    mat.m[10] = -(far_plane + near_plane) / (far_plane - near_plane);
    mat.m[11] = -1.0f;
    mat.m[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);

    return mat;
}



// =============================================================================
// Quat Struct
// =============================================================================

struct Quat {
    float w = 1.0f, x = 0.0f, y = 0.0f, z = 0.0f;

    // --- Constructors ---
    Quat() = default;
    constexpr Quat(float w_in, float x_in, float y_in, float z_in) noexcept
        : w(w_in), x(x_in), y(y_in), z(z_in) {}

    // --- Static Factory ---
    static Quat from_axis_angle(const Vec3& axis, float angle_rad) noexcept {
        const float half_angle = angle_rad / 2.0f;
        const float s = std::sin(half_angle);
        return Quat(std::cos(half_angle), axis.x * s, axis.y * s, axis.z * s);
    }

    // --- Operators ---
    Quat& operator+=(const Quat& other) noexcept { w += other.w; x += other.x; y += other.y; z += other.z; return *this; }
    Quat& operator-=(const Quat& other) noexcept { w -= other.w; x -= other.x; y -= other.y; z -= other.z; return *this; }
    Quat& operator*=(float scalar) noexcept { w *= scalar; x *= scalar; y *= scalar; z *= scalar; return *this; }

    Quat operator+(const Quat& other) const noexcept { return Quat(w + other.w, x + other.x, y + other.y, z + other.z); }
    Quat operator-(const Quat& other) const noexcept { return Quat(w - other.w, x - other.x, y - other.y, z - other.z); }
    Quat operator*(float scalar) const noexcept { return Quat(w * scalar, x * scalar, y * scalar, z * scalar); }
    Quat operator-() const noexcept { return Quat(-w, -x, -y, -z); }

    // --- Member Methods ---
    float magnitude() const noexcept { return std::sqrt(w*w + x*x + y*y + z*z); }

    void normalize() noexcept {
        const float mag = magnitude();
        if (mag > 0.0001f) { *this *= (1.0f / mag); }
    }

    Quat normalized() const noexcept {
        Quat q = *this;
        q.normalize();
        return q;
    }

    Quat conjugate() const noexcept { return Quat(w, -x, -y, -z); }
    Quat inverse() const noexcept { return conjugate().normalized(); /* For unit quat, just conjugate */ }

    Mat4 to_rotation_matrix() const noexcept {
        // For a normalized quaternion
        Mat4 mat;
        const float xx = x * x, yy = y * y, zz = z * z;
        const float xy = x * y, xz = x * z, yz = y * z;
        const float wx = w * x, wy = w * y, wz = w * z;

        mat.m[0] = 1.0f - 2.0f * (yy + zz);
        mat.m[1] = 2.0f * (xy + wz);
        mat.m[2] = 2.0f * (xz - wy);

        mat.m[4] = 2.0f * (xy - wz);
        mat.m[5] = 1.0f - 2.0f * (xx + zz);
        mat.m[6] = 2.0f * (yz + wx);

        mat.m[8] = 2.0f * (xz + wy);
        mat.m[9] = 2.0f * (yz - wx);
        mat.m[10]= 1.0f - 2.0f * (xx + yy);
        return mat;
    }
};

// --- Quat Free Functions ---

inline Quat operator*(float scalar, const Quat& q) noexcept { return q * scalar; }

// Hamilton Product
inline Quat operator*(const Quat& q1, const Quat& q2) noexcept {
    return Quat(
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z, // w
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, // x
        q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x, // y
        q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w  // z
    );
}

inline std::ostream& operator<<(std::ostream& os, const Quat& q) {
    os << "Quat(w:" << q.w << ", x:" << q.x << ", y:" << q.y << ", z:" << q.z << ")";
    return os;
}

/**
 * @brief Spherical Linear Interpolation between two quaternions.
 * @param q0 Start quaternion (should be normalized).
 * @param q1 End quaternion (should be normalized).
 * @param t Interpolation factor [0, 1].
 * @return The interpolated quaternion.
 */
inline Quat slerp(Quat q0, Quat q1, float t) noexcept {
    float dot_product = q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;

    // If dot is negative, the quaternions are more than 90 degrees apart.
    // We can take the shorter path by reversing one of them.
    if (dot_product < 0.0f) {
        q1 = -q1;
        dot_product = -dot_product;
    }

    // If the quaternions are very close, use linear interpolation to avoid
    // division by zero and precision issues.
    const float DOT_THRESHOLD = 0.9995f;
    if (dot_product > DOT_THRESHOLD) {
        Quat result = q0 + t * (q1 - q0);
        result.normalize();
        return result;
    }

    // Standard slerp formula
    const float theta_0 = std::acos(dot_product);        // angle between quaternions
    const float theta = theta_0 * t;                     // angle for the interpolated point
    const float sin_theta = std::sin(theta);
    const float sin_theta_0 = std::sin(theta_0);

    const float s0 = std::cos(theta) - dot_product * sin_theta / sin_theta_0;
    const float s1 = sin_theta / sin_theta_0;

    return (s0 * q0) + (s1 * q1);
}

} // namespace alg

#endif // ALGEBRA_HPP