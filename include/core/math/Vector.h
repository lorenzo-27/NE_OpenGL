#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cfloat>
#include <memory>
#include <iostream>
#include <cassert>

class Vector3 {
public:
    // Constructors
    Vector3() = default;

    explicit constexpr Vector3(float b) noexcept : x(b), y(b), z(b) {
    }

    explicit Vector3(const float *b) : x(b[0]), y(b[1]), z(b[2]) {
    }

    constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {
    }

    // Static factories
    [[nodiscard]] static constexpr Vector3 Zero() noexcept { return Vector3(0.0f); }
    [[nodiscard]] static constexpr Vector3 Ones() noexcept { return Vector3(1.0f); }
    [[nodiscard]] static constexpr Vector3 UnitX() noexcept { return {1.0f, 0.0f, 0.0f}; }
    [[nodiscard]] static constexpr Vector3 UnitY() noexcept { return {0.0f, 1.0f, 0.0f}; }
    [[nodiscard]] static constexpr Vector3 UnitZ() noexcept { return {0.0f, 0.0f, 1.0f}; }

    // Setters
    void Set(float _x, float _y, float _z) noexcept {
        x = _x;
        y = _y;
        z = _z;
    }

    void SetZero() noexcept { Set(0.0f, 0.0f, 0.0f); }
    void SetOnes() noexcept { Set(1.0f, 1.0f, 1.0f); }
    void SetUnitX() noexcept { Set(1.0f, 0.0f, 0.0f); }
    void SetUnitY() noexcept { Set(0.0f, 1.0f, 0.0f); }
    void SetUnitZ() noexcept { Set(0.0f, 0.0f, 1.0f); }

    // Basic operations
    [[nodiscard]] Vector3 operator+(float b) const noexcept {
        return {x + b, y + b, z + b};
    }

    [[nodiscard]] Vector3 operator-(float b) const noexcept {
        return {x - b, y - b, z - b};
    }

    [[nodiscard]] Vector3 operator*(float b) const noexcept {
        return {x * b, y * b, z * b};
    }

    [[nodiscard]] Vector3 operator/(float b) const noexcept {
        return {x / b, y / b, z / b};
    }

    [[nodiscard]] Vector3 operator+(const Vector3 &b) const noexcept {
        return {x + b.x, y + b.y, z + b.z};
    }

    [[nodiscard]] Vector3 operator-(const Vector3 &b) const noexcept {
        return {x - b.x, y - b.y, z - b.z};
    }

    [[nodiscard]] Vector3 operator*(const Vector3 &b) const noexcept {
        return {x * b.x, y * b.y, z * b.z};
    }

    [[nodiscard]] Vector3 operator/(const Vector3 &b) const noexcept {
        return {x / b.x, y / b.y, z / b.z};
    }

    Vector3 &operator+=(float b) noexcept {
        x += b;
        y += b;
        z += b;
        return *this;
    }

    Vector3 &operator-=(float b) noexcept {
        x -= b;
        y -= b;
        z -= b;
        return *this;
    }

    Vector3 &operator*=(float b) noexcept {
        x *= b;
        y *= b;
        z *= b;
        return *this;
    }

    Vector3 &operator/=(float b) noexcept {
        x /= b;
        y /= b;
        z /= b;
        return *this;
    }

    Vector3 &operator+=(const Vector3 &b) noexcept {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }

    Vector3 &operator-=(const Vector3 &b) noexcept {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        return *this;
    }

    Vector3 &operator*=(const Vector3 &b) noexcept {
        x *= b.x;
        y *= b.y;
        z *= b.z;
        return *this;
    }

    Vector3 &operator/=(const Vector3 &b) noexcept {
        x /= b.x;
        y /= b.y;
        z /= b.z;
        return *this;
    }

    [[nodiscard]] Vector3 operator-() const noexcept {
        return {-x, -y, -z};
    }

    // Vector algebra
    [[nodiscard]] float Dot(const Vector3 &b) const noexcept {
        return x * b.x + y * b.y + z * b.z;
    }

    [[nodiscard]] Vector3 Cross(const Vector3 &b) const noexcept {
        return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x};
    }

    [[nodiscard]] constexpr float MagSq() const noexcept {
        return x * x + y * y + z * z;
    }

    [[nodiscard]] float Mag() const noexcept {
        return std::sqrt(MagSq());
    }

    // Normalization
    void Normalize() {
        *this /= Mag();
    }

    void NormalizeSafe() {
        *this /= (Mag() + FLT_EPSILON);
    }

    [[nodiscard]] Vector3 Normalized() const {
        return *this / Mag();
    }

    [[nodiscard]] Vector3 NormalizedSafe() const {
        return *this / (Mag() + FLT_EPSILON);
    }

    [[nodiscard]] float Angle(const Vector3 &b) const {
        return std::acos(Normalized().Dot(b.Normalized()));
    }

    [[nodiscard]] float AngleSafe(const Vector3 &b) const {
        return std::acos(NormalizedSafe().Dot(b.NormalizedSafe()));
    }

    void ClipMag(float m) {
        assert(m > 0.0f);
        const float r = MagSq() / (m * m);
        if (r > 1.0f) {
            *this /= std::sqrt(r);
        }
    }

    [[nodiscard]] constexpr bool IsNDC() const noexcept {
        return (x > -1.0f && x < 1.0f && y > -1.0f && y < 1.0f && z > -1.0f && z < 1.0f);
    }

    // Components
    float x, y, z;
};

[[nodiscard]] inline Vector3 operator/(float b, const Vector3 &v) noexcept {
    return {b / v.x, b / v.y, b / v.z};
}

inline void operator/=(float b, Vector3 &v) noexcept {
    v = b / v;
}

class Vector4 {
public:
    Vector4() = default;

    explicit constexpr Vector4(float b) noexcept : x(b), y(b), z(b), w(b) {
    }

    constexpr Vector4(const Vector3 &xyz, float w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {
    }

    constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {
    }

    [[nodiscard]] Vector3 XYZ() const noexcept { return {x, y, z}; }
    [[nodiscard]] Vector3 XYZNormalized() const { return XYZ().Normalized(); }
    [[nodiscard]] Vector3 Homogenized() const noexcept { return {x / w, y / w, z / w}; }

    [[nodiscard]] Vector4 operator*(float b) const noexcept {
        return {x * b, y * b, z * b, w * b};
    }

    [[nodiscard]] Vector4 operator/(float b) const noexcept {
        return {x / b, y / b, z / b, w / b};
    }

    Vector4 &operator*=(float b) noexcept {
        x *= b;
        y *= b;
        z *= b;
        w *= b;
        return *this;
    }

    Vector4 &operator/=(float b) noexcept {
        return *this *= 1.0f / b;
    }

    [[nodiscard]] float Dot(const Vector4 &b) const noexcept { return x * b.x + y * b.y + z * b.z + w * b.w; }

    // Components
    float x, y, z, w;
};

class Matrix4 {
public:
    // Constructors
    constexpr Matrix4(std::initializer_list<float> values) noexcept {
        std::ranges::copy(values, m);
    }
    Matrix4() = default;

    explicit Matrix4(float b) noexcept { Fill(b); }

    // General
    void Fill(float b) noexcept {
        std::fill_n(m, 16, b);
    }
    void MakeZero() noexcept {
        Fill(0.0f);
    }

    void MakeIdentity() noexcept {
        m[0] = 1.0f;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = 1.0f;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }

    void MakeRotX(float a) noexcept {
        const float ca = std::cos(a);
        const float sa = std::sin(a);
        m[0] = 1.0f;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = ca;
        m[6] = -sa;
        m[7] = 0.0f;
        m[8] = 0.0f;
        m[9] = sa;
        m[10] = ca;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }

    void MakeRotY(float a) noexcept {
        const float ca = std::cos(a);
        const float sa = std::sin(a);
        m[0] = ca;
        m[1] = 0.0f;
        m[2] = sa;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = -sa;
        m[9] = 0.0f;
        m[10] = ca;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }

    void MakeRotZ(float a) noexcept {
        const float ca = std::cos(a);
        const float sa = std::sin(a);
        m[0] = ca;
        m[1] = -sa;
        m[2] = 0.0f;
        m[3] = 0.0f;
        m[4] = sa;
        m[5] = ca;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = 1.0f;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }

    void MakeTrans(const Vector3 &t) noexcept {
        m[0] = 1.0f;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[3] = t.x;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[7] = t.y;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = 1.0f;
        m[11] = t.z;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }

    void MakeScale(const Vector3 &s) noexcept {
        m[0] = s.x;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = s.y;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = s.z;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }

    // Static factories
    [[nodiscard]] static Matrix4 Zero() noexcept {
        Matrix4 m;
        m.MakeZero();
        return m;
    }

    [[nodiscard]] static Matrix4 Identity() noexcept {
        Matrix4 m;
        m.MakeIdentity();
        return m;
    }

    [[nodiscard]] static Matrix4 RotX(float a) noexcept {
        Matrix4 m;
        m.MakeRotX(a);
        return m;
    }

    [[nodiscard]] static Matrix4 RotY(float a) noexcept {
        Matrix4 m;
        m.MakeRotY(a);
        return m;
    }

    [[nodiscard]] static Matrix4 RotZ(float a) noexcept {
        Matrix4 m;
        m.MakeRotZ(a);
        return m;
    }

    [[nodiscard]] static Matrix4 Trans(const Vector3 &t) noexcept {
        Matrix4 m;
        m.MakeTrans(t);
        return m;
    }

    [[nodiscard]] static Matrix4 Scale(float s) noexcept {
        Matrix4 m;
        m.MakeScale(Vector3(s));
        return m;
    }

    [[nodiscard]] static Matrix4 Scale(const Vector3 &s) noexcept {
        Matrix4 m;
        m.MakeScale(s);
        return m;
    }

    // Getters
    [[nodiscard]] Vector3 XAxis() const noexcept {
        return {m[0], m[4], m[8]};
    }

    [[nodiscard]] Vector3 YAxis() const noexcept {
        return {m[1], m[5], m[9]};
    }

    [[nodiscard]] Vector3 ZAxis() const noexcept {
        return {m[2], m[6], m[10]};
    }

    [[nodiscard]] Vector3 Translation() const noexcept {
        return {m[3], m[7], m[11]};
    }

    [[nodiscard]] Vector3 Scale() const noexcept {
        return {m[0], m[5], m[10]};
    }

    // Setters
    void SetTranslation(const Vector3 &t) noexcept {
        m[3] = t.x;
        m[7] = t.y;
        m[11] = t.z;
    }

    void SetXAxis(const Vector3 &t) noexcept {
        m[0] = t.x;
        m[4] = t.y;
        m[8] = t.z;
    }

    void SetYAxis(const Vector3 &t) noexcept {
        m[1] = t.x;
        m[5] = t.y;
        m[9] = t.z;
    }

    void SetZAxis(const Vector3 &t) noexcept {
        m[2] = t.x;
        m[6] = t.y;
        m[10] = t.z;
    }

    void SetScale(const Vector3 &s) noexcept {
        m[0] = s.x;
        m[5] = s.y;
        m[10] = s.z;
    }

    // Transformations
    [[nodiscard]] constexpr Matrix4 Transposed() const noexcept {
        return Matrix4{
            m[0],  m[4],  m[8],   m[12],
            m[1],  m[5],  m[9],   m[13],
            m[2],  m[6],  m[10],  m[14],
            m[3],  m[7],  m[11],  m[15]
        };
    }

    void Translate(const Vector3 &t) noexcept {
        m[3] += t.x;
        m[7] += t.y;
        m[11] += t.z;
    }

    void Stretch(const Vector3 &s) noexcept {
        m[0] *= s.x;
        m[5] *= s.y;
        m[10] *= s.z;
    }

    // Arithmetic operators
    [[nodiscard]] Matrix4 operator+(const Matrix4 &b) const noexcept {
        Matrix4 out;
        for (int i = 0; i < 16; ++i) { out.m[i] = m[i] + b.m[i]; }
        return out;
    }

    [[nodiscard]] Matrix4 operator-(const Matrix4 &b) const noexcept {
        Matrix4 out;
        for (int i = 0; i < 16; ++i) { out.m[i] = m[i] - b.m[i]; }
        return out;
    }

    Matrix4 &operator+=(const Matrix4 &b) noexcept {
        for (int i = 0; i < 16; ++i) { m[i] += b.m[i]; }
        return *this;
    }

    Matrix4 &operator-=(const Matrix4 &b) noexcept {
        for (int i = 0; i < 16; ++i) { m[i] -= b.m[i]; }
        return *this;
    }

    Matrix4 &operator*=(float scalar) noexcept {
        for (float &val: m) val *= scalar;
        return *this;
    }

    Matrix4 &operator/=(float scalar) noexcept {
        return *this *= (1.0f / scalar);
    }

    // Matrix-matrix multiplication
    [[nodiscard]] Matrix4 operator*(const Matrix4 &b) const noexcept {
        Matrix4 out;
        out.m[0] = b.m[0] * m[0] + b.m[4] * m[1] + b.m[8] * m[2] + b.m[12] * m[3];
        out.m[1] = b.m[1] * m[0] + b.m[5] * m[1] + b.m[9] * m[2] + b.m[13] * m[3];
        out.m[2] = b.m[2] * m[0] + b.m[6] * m[1] + b.m[10] * m[2] + b.m[14] * m[3];
        out.m[3] = b.m[3] * m[0] + b.m[7] * m[1] + b.m[11] * m[2] + b.m[15] * m[3];

        out.m[4] = b.m[0] * m[4] + b.m[4] * m[5] + b.m[8] * m[6] + b.m[12] * m[7];
        out.m[5] = b.m[1] * m[4] + b.m[5] * m[5] + b.m[9] * m[6] + b.m[13] * m[7];
        out.m[6] = b.m[2] * m[4] + b.m[6] * m[5] + b.m[10] * m[6] + b.m[14] * m[7];
        out.m[7] = b.m[3] * m[4] + b.m[7] * m[5] + b.m[11] * m[6] + b.m[15] * m[7];

        out.m[8] = b.m[0] * m[8] + b.m[4] * m[9] + b.m[8] * m[10] + b.m[12] * m[11];
        out.m[9] = b.m[1] * m[8] + b.m[5] * m[9] + b.m[9] * m[10] + b.m[13] * m[11];
        out.m[10] = b.m[2] * m[8] + b.m[6] * m[9] + b.m[10] * m[10] + b.m[14] * m[11];
        out.m[11] = b.m[3] * m[8] + b.m[7] * m[9] + b.m[11] * m[10] + b.m[15] * m[11];

        out.m[12] = b.m[0] * m[12] + b.m[4] * m[13] + b.m[8] * m[14] + b.m[12] * m[15];
        out.m[13] = b.m[1] * m[12] + b.m[5] * m[13] + b.m[9] * m[14] + b.m[13] * m[15];
        out.m[14] = b.m[2] * m[12] + b.m[6] * m[13] + b.m[10] * m[14] + b.m[14] * m[15];
        out.m[15] = b.m[3] * m[12] + b.m[7] * m[13] + b.m[11] * m[14] + b.m[15] * m[15];
        return out;
    }

    Matrix4 &operator*=(const Matrix4 &b) noexcept {
        return *this = *this * b;
    }

    // Matrix-vector multiplication
    [[nodiscard]] Vector4 operator*(const Vector4 &vec) const noexcept {
        return {
            m[0] * vec.x + m[1] * vec.y + m[2] * vec.z + m[3] * vec.w,
            m[4] * vec.x + m[5] * vec.y + m[6] * vec.z + m[7] * vec.w,
            m[8] * vec.x + m[9] * vec.y + m[10] * vec.z + m[11] * vec.w,
            m[12] * vec.x + m[13] * vec.y + m[14] * vec.z + m[15] * vec.w
        };
    }

    [[nodiscard]] Vector3 MulPoint(const Vector3 &p) const noexcept {
        const float w = m[12] * p.x + m[13] * p.y + m[14] * p.z + m[15];
        return  {
            (m[0] * p.x + m[1] * p.y + m[2] * p.z + m[3]) / w,
            (m[4] * p.x + m[5] * p.y + m[6] * p.z + m[7]) / w,
            (m[8] * p.x + m[9] * p.y + m[10] * p.z + m[11]) / w
        };
    }

    [[nodiscard]] Vector3 MulDirection(const Vector3 &dir) const noexcept {
        return {
            m[0] * dir.x + m[1] * dir.y + m[2] * dir.z,
            m[4] * dir.x + m[5] * dir.y + m[6] * dir.z,
            m[8] * dir.x + m[9] * dir.y + m[10] * dir.z
        };
    }

    // Inverse matrix
    [[nodiscard]] Matrix4 Inverse() const {
        // Using std::array for compile-time bounds checking and better semantics
        std::array<float, 16> cofactors{};

        // Calculate cofactors using helper lambda for better readability
        const auto cofactor = [this](size_t row, size_t col) -> float {
            // Indices for the 3x3 minor matrix
            const size_t i1 = (row + 1) % 4, i2 = (row + 2) % 4, i3 = (row + 3) % 4;
            const size_t j1 = (col + 1) % 4, j2 = (col + 2) % 4, j3 = (col + 3) % 4;

            // Calculate 3x3 determinant using Sarrus' rule
            return m[i1 * 4 + j1] * m[i2 * 4 + j2] * m[i3 * 4 + j3] +
                   m[i1 * 4 + j2] * m[i2 * 4 + j3] * m[i3 * 4 + j1] +
                   m[i1 * 4 + j3] * m[i2 * 4 + j1] * m[i3 * 4 + j2] -
                   m[i1 * 4 + j3] * m[i2 * 4 + j2] * m[i3 * 4 + j1] -
                   m[i1 * 4 + j2] * m[i2 * 4 + j1] * m[i3 * 4 + j3] -
                   m[i1 * 4 + j1] * m[i2 * 4 + j3] * m[i3 * 4 + j2];
        };

        // Calculate all cofactors with proper sign
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                cofactors[j * 4 + i] = ((i + j) % 2 == 0 ? 1.0f : -1.0f) * cofactor(i, j);
            }
        }

        // Calculate determinant using first row of cofactors
        const float det = m[0] * cofactors[0] + m[1] * cofactors[4] +
                         m[2] * cofactors[8] + m[3] * cofactors[12];

        // Check for singular matrix
        if (std::abs(det) < std::numeric_limits<float>::epsilon()) {
            return Identity();
        }

        // Create and return the inverse matrix
        Matrix4 inverse;
        const float inv_det = 1.0f / det;
        std::ranges::transform(cofactors, inverse.m,
                               [inv_det](float x) { return x * inv_det; });

        return inverse;
    }

    // Components
    float m[16]{};
};

// Stream operators
inline std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
    return os << v.x << ", " << v.y << ", " << v.z;
}

inline std::ostream &operator<<(std::ostream &os, const Vector4 &v) {
    return os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
}

inline std::ostream &operator<<(std::ostream &os, const Matrix4 &m) {
    return os << "[\n"
           << m.m[0] << ", " << m.m[1] << ", " << m.m[2] << ", " << m.m[3] << "\n"
           << m.m[4] << ", " << m.m[5] << ", " << m.m[6] << ", " << m.m[7] << "\n"
           << m.m[8] << ", " << m.m[9] << ", " << m.m[10] << ", " << m.m[11] << "\n"
           << m.m[12] << ", " << m.m[13] << ", " << m.m[14] << ", " << m.m[15] << "\n]";
}
