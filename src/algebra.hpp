#ifndef ALGEBRA_HPP
#define ALGEBRA_HPP

#include <iostream>
#include <cmath>
#include <cstdio> // For sprintf in Mat4::operator<<

/**
 * @namespace alg
 * @brief Linear algebra library for 3D graphics and physics
 *
 * Provides vector, matrix, and quaternion implementations with essential operations
 * required for 3D transformations, camera systems, and rotation handling. Designed
 * for real-time graphics applications with focus on performance and correctness.
 *
 * Key Principles:
 * 1. Right-handed coordinate system (consistent with OpenGL/Vulkan)
 * 2. Column-major matrix storage (GPU compatibility)
 * 3. Unit quaternions for robust rotations
 *
 * @author Hertz
 * @date 17/07/2025
 */
namespace alg {

// Forward declarations
struct Vec3;
struct Mat4;
struct Quat;

// =============================================================================
// Constants & Utility Functions
// =============================================================================

/**
 * @brief Mathematical constant π (pi)
 *
 * Fundamental constant for circle geometry and trigonometric functions.
 * Required for accurate angle conversions and rotation calculations.
 */
constexpr float PI = 3.14159265359f;

/**
 * @brief Convert degrees to radians
 * @param degrees Angle in degrees
 * @return Equivalent angle in radians
 *
 * Most trigonometric functions in C++ standard library use radians.
 * This conversion enables intuitive angle specification in degrees
 * while maintaining computational accuracy.
 */
inline float degrees_to_radians(float degrees) noexcept {
    return degrees * PI / 180.0f;
}

/**
 * @brief Convert radians to degrees
 * @param radians Angle in radians
 * @return Equivalent angle in degrees
 *
 * Useful for human-readable angle representations and UI display
 * after mathematical operations in radians.
 */
inline float radians_to_degrees(float radians) noexcept {
    return radians * 180.0f / PI;
}

// =============================================================================
// Vec2 Struct (2D Vector)
// =============================================================================

/**
 * @struct Vec2
 * @brief 2-dimensional vector for texture coordinates and 2D operations
 *
 * Primarily used for UV mapping and 2D graphics where z-component is unnecessary.
 * Provides storage efficiency for 2D-specific operations.
 */
struct Vec2 {
    float x = 0.0f, y = 0.0f;

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }


};

// =============================================================================
// Vec3 Struct (3D Vector)
// ============================f=================================================

/**
 * @struct Vec3
 * @brief 3-dimensional vector for positions, directions, and scaling
 *
 * Core building block for 3D graphics operations. Used for:
 * - World-space coordinates
 * - Surface normals
 * - Velocity vectors
 * - RGB color representations
 *
 * Implements essential vector operations required for linear algebra.
 */
struct Vec3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    // --- Constructors ---
    Vec3() = default;

    /**
     * @brief Construct vector from components
     * @param x_in X-component
     * @param y_in Y-component
     * @param z_in Z-component
     */
    constexpr Vec3(float x_in, float y_in, float z_in) noexcept
        : x(x_in), y(y_in), z(z_in) {}

    // --- Vector Arithmetic Operations ---

    /**
     * @brief Vector addition (modifies current vector)
     * @param other Vector to add
     * @return Reference to modified vector
     */
    Vec3& operator+=(const Vec3& other) noexcept {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    /**
     * @brief Vector subtraction (modifies current vector)
     * @param other Vector to subtract
     * @return Reference to modified vector
     */
    Vec3& operator-=(const Vec3& other) noexcept {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    /**
     * @brief Scalar multiplication (modifies current vector)
     * @param scalar Scaling factor
     * @return Reference to modified vector
     */
    Vec3& operator*=(float scalar) noexcept {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    /**
     * @brief Scalar division (modifies current vector)
     * @param scalar Division factor
     * @return Reference to modified vector
     * @warning Undefined behavior when scalar == 0
     */
    Vec3& operator/=(float scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    // --- Vector Arithmetic Operations (Immutable) ---
    Vec3 operator+(const Vec3& other) const noexcept {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    Vec3 operator-(const Vec3& other) const noexcept {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    Vec3 operator*(float scalar) const noexcept {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }
    Vec3 operator-() const noexcept {
        return Vec3(-x, -y, -z);
    }

    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // --- Vector Operations ---

    /**
     * @brief Compute vector magnitude (length)
     * @return Euclidean norm ||v||
     *
     * Fundamental for distance calculations and physics simulations.
     * Computes √(x² + y² + z²) using Pythagoras theorem in 3D space.
     */
    float magnitude() const noexcept {
        return std::sqrt(x*x + y*y + z*z);
    }

    /**
     * @brief Compute squared vector magnitude
     * @return Squared Euclidean norm ||v||²
     *
     * Optimization for distance comparisons avoiding sqrt computation.
     * Essential for performance-critical collision detection.
     */
    float magnitude_squared() const noexcept {
        return x*x + y*y + z*z;
    }

    /**
     * @brief Normalize vector in-place
     *
     * Converts vector to unit vector while preserving direction.
     * Required for direction vectors in lighting calculations.
     * Handles zero-vector case gracefully.
     */
    void normalize() {
        const float mag = magnitude();
        if (mag > 0.0f) {
            *this /= mag;
        }
    }

    /**
     * @brief Get normalized copy of vector
     * @return Unit vector in same direction
     *
     * Non-destructive version of normalize(). Returns zero vector
     * when magnitude is zero to prevent NaN propagation.
     */
    Vec3 normalized() const {
        const float mag = magnitude();
        if (mag > 0.0f) {
            return *this / mag;
        }
        return Vec3(0.0f, 0.0f, 0.0f);
    }
};

// --- Vec3 Free Functions ---

/**
 * @brief Scalar-vector multiplication (commutative)
 * @param scalar Scaling factor
 * @param v Vector to scale
 * @return Scaled vector
 */
inline Vec3 operator*(float scalar, const Vec3& v) noexcept {
    return v * scalar;
}

/**
 * @brief Vector dot product
 * @param a First vector
 * @param b Second vector
 * @return a · b = Σ(aᵢbᵢ)
 *
 * Measures cosine of angle between vectors when both are unit length.
 * Fundamental for:
 * - Projection calculations
 * - Lighting equations (Lambertian reflectance)
 * - Orthogonality testing
 */
inline float dot(const Vec3& a, const Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Vector cross product
 * @param a First vector
 * @param b Second vector
 * @return a × b (right-handed system)
 *
 * Computes vector orthogonal to both inputs. Magnitude equals
 * area of parallelogram spanned by input vectors.
 * Critical for:
 * - Surface normal calculation
 * - Torque computation in physics
 * - Building orthonormal bases
 */
inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

/**
 * @brief Output stream operator for vector
 * @param os Output stream
 * @param v Vector to display
 * @return Reference to output stream
 *
 * Formats vector as (x, y, z) for debugging and logging.
 */
inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// =============================================================================
// Mat4 Struct (4x4 Matrix)
// =============================================================================

/**
 * @struct Mat4
 * @brief 4x4 transformation matrix (column-major storage)
 *
 * Represents affine transformations in homogeneous coordinates.
 * Storage order: m[column * 4 + row] (OpenGL-compatible)
 *
 * Supports:
 * - Rigid transformations (translation + rotation)
 * - Projections (perspective/orthographic)
 * - Compound transformations via matrix multiplication
 *
 * Column-major storage enables direct memory compatibility with:
 * - OpenGL (glUniformMatrix4fv)
 * - Vulkan (with appropriate transpose flag)
 */
struct Mat4 {
    // Column-major storage: m[column * 4 + row]
    float m[16];

    /**
     * @brief Construct identity matrix
     *
     * Initializes to identity transformation (no-op).
     * Identity matrix serves as multiplicative identity
     * in transformation chains.
     */
    Mat4() noexcept {
        // clang-format off
        m[0]=1; m[4]=0; m[8]=0;  m[12]=0;
        m[1]=0; m[5]=1; m[9]=0;  m[13]=0;
        m[2]=0; m[6]=0; m[10]=1; m[14]=0;
        m[3]=0; m[7]=0; m[11]=0; m[15]=1;
        // clang-format on
    }

    // --- Transformation Factory Methods ---

    /**
     * @brief Create translation matrix
     * @param translation Displacement vector
     * @return Homogeneous translation matrix
     *
     * Constructs matrix of form:
     * [1 0 0 Tx]
     * [0 1 0 Ty]
     * [0 0 1 Tz]
     * [0 0 0 1 ]
     */
    static Mat4 create_translation(const Vec3& translation) noexcept;

    /**
     * @brief Create scaling matrix
     * @param scale Scaling factors per axis
     * @return Homogeneous scaling matrix
     *
     * Constructs matrix of form:
     * [Sx 0  0  0]
     * [0  Sy 0  0]
     * [0  0  Sz 0]
     * [0  0  0  1]
     */
    static Mat4 create_scale(const Vec3& scale) noexcept;

    /**
     * @brief Create rotation matrix around X-axis
     * @param angle_rad Rotation angle (radians)
     * @return Rotation matrix (right-hand rule)
     *
     * Right-hand rotation convention:
     * Positive angle rotates counter-clockwise when looking
     * along axis from positive direction.
     */
    static Mat4 create_rotation_x(float angle_rad) noexcept;

    /**
     * @brief Create rotation matrix around Y-axis
     * @param angle_rad Rotation angle (radians)
     */
    static Mat4 create_rotation_y(float angle_rad) noexcept;

    /**
     * @brief Create rotation matrix around Z-axis
     * @param angle_rad Rotation angle (radians)
     */
    static Mat4 create_rotation_z(float angle_rad) noexcept;

    /**
     * @brief Create combined rotation from Euler angles
     * @param angles_rad Rotation angles (radians) for X,Y,Z axes
     * @return Combined rotation matrix
     *
     * Rotation order: Y (yaw) -> X (pitch) -> Z (roll)
     *
     * This order avoids gimbal lock for common camera systems
     * where yaw is applied first, then pitch, then roll.
     */
    static Mat4 create_rotation_xyz(const Vec3& angles_rad) noexcept;

    /**
     * @brief Create view matrix for camera
     * @param eye Camera position in world space
     * @param target Point camera is looking at
     * @param up Approximate up direction
     * @return View transformation matrix
     *
     * Constructs matrix that transforms world coordinates to
     * camera space. Uses right-handed coordinate system:
     * - Camera looks along negative Z-axis
     * - Y-axis points upward in camera space
     *
     * Computes orthonormal basis from:
     * 1. Forward vector (eye->target)
     * 2. Right vector (up × forward)
     * 3. Real up vector (forward × right)
     */
    static Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) noexcept;

    /**
     * @brief Create perspective projection matrix
     * @param fov_rad Vertical field-of-view (radians)
     * @param aspect_ratio Viewport width/height
     * @param near_plane Near clipping plane (>0)
     * @param far_plane Far clipping plane
     * @return Perspective projection matrix
     *
     * Projects 3D points to 2D viewport with perspective divide.
     * Depth range: [-1, 1] (OpenGL standard)
     *
     * Matrix form:
     * [f/aspect 0  0              0            ]
     * [0        f  0              0            ]
     * [0        0  -(f+n)/(f-n)  -2fn/(f-n)   ]
     * [0        0  -1             0            ]
     *
     * Where f = cot(fov/2)
     */
    static Mat4 create_perspective(
        float fov_rad,
        float aspect_ratio,
        float near_plane,
        float far_plane
    ) noexcept;
};

// --- Mat4 Operations ---

/**
 * @brief Matrix multiplication
 * @param a Left matrix
 * @param b Right matrix
 * @return Matrix product a * b
 *
 * Implements standard matrix multiplication for 4x4 matrices.
 * Order matters: Applies transformation b first, then a.
 *
 * Implementation notes:
 * - Column-major storage requires careful index calculation
 * - Optimized for clarity over absolute performance
 */
inline Mat4 operator*(const Mat4& a, const Mat4& b) noexcept {
    Mat4 result;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            // Compute dot product of a's row and b's column
            for (int k = 0; k < 4; ++k) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            result.m[col * 4 + row] = sum;
        }
    }
    return result;
}

/**
 * @brief Transform 3D point with matrix
 * @param m Transformation matrix
 * @param v 3D point (implicit w=1)
 * @return Transformed point (w-division not performed)
 *
 * Applies affine transformation to point:
 * [x']   [m00 m01 m02 m03] [x]
 * [y'] = [m10 m11 m12 m13] [y]
 * [z']   [m20 m21 m22 m23] [z]
 * [ 1]   [m30 m31 m32 m33] [1]
 *
 * Returns only (x', y', z') components. For perspective
 * projections, divide by w-component is needed separately.
 */
inline Vec3 operator*(const Mat4& m, const Vec3& v) noexcept {
    return Vec3(
        m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12],
        m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13],
        m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14]
    );
}

/**
 * @brief Output stream operator for matrix
 * @param os Output stream
 * @param mat Matrix to display
 * @return Reference to output stream
 *
 * Formats matrix in row-major layout for readability:
 * [ row0 ]
 * [ row1 ]
 * [ row2 ]
 * [ row3 ]
 *
 * Note: Matrix memory is column-major, but display shows
 * logical rows for easier interpretation.
 */
inline std::ostream& operator<<(std::ostream& os, const Mat4& mat) {
    char buffer[256];
    // Print in row-major format for readability
    std::snprintf(buffer, 256,
        "[ %7.2f %7.2f %7.2f %7.2f ]\n"
        "[ %7.2f %7.2f %7.2f %7.2f ]\n"
        "[ %7.2f %7.2f %7.2f %7.2f ]\n"
        "[ %7.2f %7.2f %7.2f %7.2f ]",
        mat.m[0], mat.m[4], mat.m[8], mat.m[12],
        mat.m[1], mat.m[5], mat.m[9], mat.m[13],
        mat.m[2], mat.m[6], mat.m[10], mat.m[14],
        mat.m[3], mat.m[7], mat.m[11], mat.m[15]
    );
    os << buffer;
    return os;
}

// --- Mat4 Method Implementations ---

inline Mat4 Mat4::create_translation(const Vec3& t) noexcept {
    Mat4 mat;
    mat.m[12] = t.x;
    mat.m[13] = t.y;
    mat.m[14] = t.z;
    return mat;
}

inline Mat4 Mat4::create_scale(const Vec3& s) noexcept {
    Mat4 mat;
    mat.m[0] = s.x;
    mat.m[5] = s.y;
    mat.m[10] = s.z;
    return mat;
}

inline Mat4 Mat4::create_rotation_x(float angle_rad) noexcept {
    Mat4 mat;
    const float c = std::cos(angle_rad), s = std::sin(angle_rad);
    // Column 1 (affects y and z)
    mat.m[5] = c;  mat.m[9] = -s;
    mat.m[6] = s;  mat.m[10] = c;
    return mat;
}

inline Mat4 Mat4::create_rotation_y(float angle_rad) noexcept {
    Mat4 mat;
    const float c = std::cos(angle_rad), s = std::sin(angle_rad);
    // Column 0 (affects x and z)
    mat.m[0] = c;  mat.m[8] = s;
    mat.m[2] = -s; mat.m[10] = c;
    return mat;
}

inline Mat4 Mat4::create_rotation_z(float angle_rad) noexcept {
    Mat4 mat;
    const float c = std::cos(angle_rad), s = std::sin(angle_rad);
    // Column 0 (affects x and y)
    mat.m[0] = c;  mat.m[4] = -s;
    mat.m[1] = s;  mat.m[5] = c;
    return mat;
}

inline Mat4 Mat4::create_rotation_xyz(const Vec3& angles_rad) noexcept {
    const Mat4 rot_y = Mat4::create_rotation_y(angles_rad.y); // Yaw
    const Mat4 rot_x = Mat4::create_rotation_x(angles_rad.x); // Pitch
    const Mat4 rot_z = Mat4::create_rotation_z(angles_rad.z); // Roll
    // Apply rotations in Y->X->Z order (Tait-Bryan angles)
    return rot_y * rot_x * rot_z;
}

inline Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up) noexcept {
    const Vec3 z_axis = (eye - target).normalized(); // Forward direction
    const Vec3 x_axis = cross(up, z_axis).normalized(); // Right direction
    const Vec3 y_axis = cross(z_axis, x_axis); // Orthogonal up

    Mat4 view_matrix;
    // Column 0: Right vector
    view_matrix.m[0] = x_axis.x;
    view_matrix.m[1] = x_axis.y;
    view_matrix.m[2] = x_axis.z;
    view_matrix.m[3] = 0.0f;

    // Column 1: Up vector
    view_matrix.m[4] = y_axis.x;
    view_matrix.m[5] = y_axis.y;
    view_matrix.m[6] = y_axis.z;
    view_matrix.m[7] = 0.0f;

    // Column 2: Forward vector
    view_matrix.m[8] = z_axis.x;
    view_matrix.m[9] = z_axis.y;
    view_matrix.m[10] = z_axis.z;
    view_matrix.m[11] = 0.0f;

    // Column 3: Translation (eye position)
    view_matrix.m[12] = -dot(x_axis, eye);
    view_matrix.m[13] = -dot(y_axis, eye);
    view_matrix.m[14] = -dot(z_axis, eye);
    view_matrix.m[15] = 1.0f;

    return view_matrix;
}

inline Mat4 Mat4::create_perspective(float fov_rad, float aspect_ratio, float near_plane, float far_plane) noexcept {
    Mat4 mat{}; // Zero initialization
    const float tan_half_fov = std::tan(fov_rad / 2.0f);

    // Perspective projection parameters
    mat.m[0] = 1.0f / (aspect_ratio * tan_half_fov);
    mat.m[5] = 1.0f / tan_half_fov;
    mat.m[10] = -(far_plane + near_plane) / (far_plane - near_plane);
    mat.m[11] = -1.0f; // W-component sign flip
    mat.m[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);

    return mat;
}

// =============================================================================
// Quat Struct (Quaternion)
// =============================================================================

/**
 * @struct Quat
 * @brief Unit quaternion for 3D rotations
 *
 * Advantages over Euler angles:
 * - Avoids gimbal lock
 * - Smooth interpolation (SLERP)
 * - Efficient composition of rotations
 *
 * Storage: [w, (x, y, z)] where w is real part and (x,y,z) imaginary.
 * Unit quaternions satisfy: w² + x² + y² + z² = 1
 */
struct Quat {
    float w = 1.0f, x = 0.0f, y = 0.0f, z = 0.0f;

    // --- Constructors ---
    Quat() = default;

    /**
     * @brief Construct from components
     * @param w_in Real component
     * @param x_in Imaginary i-component
     * @param y_in Imaginary j-component
     * @param z_in Imaginary k-component
     */
    constexpr Quat(float w_in, float x_in, float y_in, float z_in) noexcept
        : w(w_in), x(x_in), y(y_in), z(z_in) {}

    // --- Static Factory ---

    /**
     * @brief Create from axis-angle representation
     * @param axis Rotation axis (must be normalized)
     * @param angle_rad Rotation angle in radians
     * @return Unit quaternion representing rotation
     *
     * Formula: q = [cos(θ/2), axis * sin(θ/2)]
     *
     * More numerically stable than Euler angles for rotation
     * composition and avoids singularities.
     */
    static Quat from_axis_angle(const Vec3& axis, float angle_rad) noexcept {
        const float half_angle = angle_rad / 2.0f;
        const float s = std::sin(half_angle);
        return Quat(std::cos(half_angle), axis.x * s, axis.y * s, axis.z * s);
    }

    // --- Quaternion Arithmetic ---
    Quat& operator+=(const Quat& other) noexcept {
        w += other.w; x += other.x; y += other.y; z += other.z;
        return *this;
    }
    Quat& operator-=(const Quat& other) noexcept {
        w -= other.w; x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
    Quat& operator*=(float scalar) noexcept {
        w *= scalar; x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Quat operator+(const Quat& other) const noexcept {
        return Quat(w + other.w, x + other.x, y + other.y, z + other.z);
    }
    Quat operator-(const Quat& other) const noexcept {
        return Quat(w - other.w, x - other.x, y - other.y, z - other.z);
    }
    Quat operator*(float scalar) const noexcept {
        return Quat(w * scalar, x * scalar, y * scalar, z * scalar);
    }
    Quat operator-() const noexcept {
        return Quat(-w, -x, -y, -z);
    }

    // --- Quaternion Operations ---

    /**
     * @brief Compute magnitude of quaternion
     * @return √(w² + x² + y² + z²)
     */
    float magnitude() const noexcept {
        return std::sqrt(w*w + x*x + y*y + z*z);
    }

    /**
     * @brief Normalize to unit quaternion in-place
     *
     * Maintains rotation representation validity.
     * Only unit quaternions represent rotations.
     */
    void normalize() noexcept {
        const float mag = magnitude();
        if (mag > 0.0001f) {
            *this *= (1.0f / mag);
        }
    }

    /**
     * @brief Get normalized copy of quaternion
     * @return Unit quaternion
     */
    Quat normalized() const noexcept {
        Quat q = *this;
        q.normalize();
        return q;
    }

    /**
     * @brief Compute quaternion conjugate
     * @return [w, -x, -y, -z]
     *
     * For unit quaternions, conjugate is equivalent to inverse.
     * Represents reverse rotation.
     */
    Quat conjugate() const noexcept {
        return Quat(w, -x, -y, -z);
    }

    /**
     * @brief Compute inverse quaternion
     * @return Inverse rotation
     *
     * For unit quaternions: inverse = conjugate.
     * Satisfies: q * q⁻¹ = identity
     */
    Quat inverse() const noexcept {
        return conjugate().normalized();
    }

    /**
     * @brief Convert to rotation matrix
     * @return 3x3 rotation matrix (embedded in Mat4)
     *
     * Matrix form derived from quaternion components.
     * Assumes quaternion is normalized.
     */
    Mat4 to_rotation_matrix() const noexcept {
        Mat4 mat;
        const float xx = x * x, yy = y * y, zz = z * z;
        const float xy = x * y, xz = x * z, yz = y * z;
        const float wx = w * x, wy = w * y, wz = w * z;

        // Diagonal elements (scaled by 1 - 2(y² + z²) etc.)
        mat.m[0] = 1.0f - 2.0f * (yy + zz);
        mat.m[5] = 1.0f - 2.0f * (xx + zz);
        mat.m[10] = 1.0f - 2.0f * (xx + yy);

        // Off-diagonal elements
        mat.m[1] = 2.0f * (xy + wz);
        mat.m[4] = 2.0f * (xy - wz);

        mat.m[2] = 2.0f * (xz - wy);
        mat.m[8] = 2.0f * (xz + wy);

        mat.m[6] = 2.0f * (yz + wx);
        mat.m[9] = 2.0f * (yz - wx);

        // Bottom row remains identity
        mat.m[15] = 1.0f;
        return mat;
    }
};

// --- Quat Free Functions ---

/**
 * @brief Scalar-quaternion multiplication
 * @param scalar Scaling factor
 * @param q Quaternion to scale
 * @return Scaled quaternion
 */
inline Quat operator*(float scalar, const Quat& q) noexcept {
    return q * scalar;
}

/**
 * @brief Hamilton product (quaternion multiplication)
 * @param q1 First quaternion
 * @param q2 Second quaternion
 * @return Composition q1 ⊗ q2
 *
 * Combines two rotations: q1 followed by q2.
 * Non-commutative: q1 ⊗ q2 ≠ q2 ⊗ q1
 *
 * Follows right-hand rule for rotation composition.
 */
inline Quat operator*(const Quat& q1, const Quat& q2) noexcept {
    return Quat(
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z, // w
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, // x
        q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x, // y
        q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w  // z
    );
}

/**
 * @brief Output stream operator for quaternion
 * @param os Output stream
 * @param q Quaternion to display
 * @return Reference to output stream
 */
inline std::ostream& operator<<(std::ostream& os, const Quat& q) {
    os << "Quat(w:" << q.w << ", x:" << q.x << ", y:" << q.y << ", z:" << q.z << ")";
    return os;
}

/**
 * @brief Spherical Linear Interpolation (SLERP)
 * @param q0 Start rotation (normalized)
 * @param q1 End rotation (normalized)
 * @param t Interpolation factor [0,1]
 * @return Interpolated quaternion
 *
 * Maintains constant angular velocity along the shortest
 * great-circle path on the unit hypersphere.
 *
 * Falls back to linear interpolation when angles are small
 * to avoid division by zero and improve performance.
 */
inline Quat slerp(Quat q0, Quat q1, float t) noexcept {
    float dot_product = q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;

    // Handle negative dot product (choose shorter path)
    if (dot_product < 0.0f) {
        q1 = -q1;
        dot_product = -dot_product;
    }

    // Threshold for linear interpolation fallback
    const float DOT_THRESHOLD = 0.9995f;
    if (dot_product > DOT_THRESHOLD) {
        // Lerp approximation for small angles
        Quat result = q0 + t * (q1 - q0);
        result.normalize();
        return result;
    }

    // Standard SLERP formula
    const float theta_0 = std::acos(dot_product);
    const float theta = theta_0 * t;
    const float sin_theta = std::sin(theta);
    const float sin_theta_0 = std::sin(theta_0);

    const float s0 = std::cos(theta) - dot_product * sin_theta / sin_theta_0;
    const float s1 = sin_theta / sin_theta_0;

    return (s0 * q0) + (s1 * q1);
}

} // namespace alg
#endif // ALGEBRA_HPP