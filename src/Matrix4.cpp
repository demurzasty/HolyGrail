#include "Matrix4.hpp"

#include <assert.h>
#include <string.h>

const Matrix4 Matrix4::Identity = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

Vector3 Matrix4::ExtractPosition(const Matrix4& matrix) {
    return Vector3(matrix[12], matrix[13], matrix[14]);
}

Vector3 Matrix4::ExtractEuler(const Matrix4& matrix) {
    Vector3 euler;

    const float m12 = matrix[6];

    if (m12 < (1.0f - Math::Epsilon)) {
        if (m12 > -(1.0f - Math::Epsilon)) {
            // is this a pure X rotation?
            if (matrix[4] == 0.0f && matrix[1] == 0.0f && matrix[2] == 0.0f && matrix[8] == 0.0f && matrix[0] == 1.0f) {
                euler.X = Math::Atan2(-m12, matrix[5]);
                euler.Y = 0.0f;
                euler.Z = 0.0f;
            }
            else {
                euler.X = Math::Asin(-m12);
                euler.Y = Math::Atan2(matrix[2], matrix[10]);
                euler.Z = Math::Atan2(matrix[4], matrix[5]);
            }
        }
        else { // m12 == -1
            euler.X = Math::PI * 0.5f;
            euler.Y = Math::Atan2(matrix[1], matrix[0]);
            euler.Z = 0.0f;
        }
    }
    else { // m12 == 1
        euler.X = -Math::PI * 0.5f;
        euler.Y = -Math::Atan2(matrix[1], matrix[0]);
        euler.Z = 0.0f;
    }

#if 0
    euler.X = Math::Atan2(matrix[6], matrix[10]);
    euler.Y = Math::Atan2(-matrix[2], Math::Sqrt(matrix[6] * matrix[6] + matrix[10] * matrix[10]));
    euler.Z = Math::Atan2(matrix[1], matrix[0]);
#endif

    return euler;
}

Vector3 Matrix4::ExtractDirection(const Matrix4& matrix) {
    return Vector3::Normalize(TransformNormal(matrix, Vector3::AxisZ));
}

Matrix4 Matrix4::CreatePerspective(float fovy, float aspect, float near, float far) {
    const float r = fovy / 2.0f;
    const float delta = near - far;
    const float s = Math::Sin(r);

    if (delta == 0 || s == 0 || aspect == 0) {
        return Identity;
    }

    const float cotangent = Math::Cos(r) / s;

    return {
        cotangent / aspect, 0, 0, 0,
        0, cotangent, 0, 0,
        0, 0, (near + far) / delta, -1,
        0, 0, (2 * near * far) / delta, 0
    };
}

Matrix4 Matrix4::CreateOrthographic(float left, float right, float bottom, float top, float near, float far) {
    const float tx = -((right + left) / (right - left));
    const float ty = -((top + bottom) / (top - bottom));
    const float tz = -((far + near) / (far - near));

    return {
        2 / (right - left), 0, 0, 0,
        0, 2 / (top - bottom), 0, 0,
        0, 0, -2 / (far - near), 0,
        tx, ty, tz, 1
    };
}

Matrix4 Matrix4::CreateTranslation(const Vector3& position) {
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        position.X, position.Y, position.Z, 1
    };
}

Matrix4 Matrix4::CreateRotationX(float radians) {
    const float c = Math::Cos(radians);
    const float s = Math::Sin(radians);

    return {
        1, 0, 0, 0,
        0, c, s, 0,
        0, -s, c, 0,
        0, 0, 0, 1
    };
}

Matrix4 Matrix4::CreateRotationY(float radians) {
    const float c = Math::Cos(radians);
    const float s = Math::Sin(radians);

    return {
        c, 0, -s, 0,
        0, 1, 0, 0,
        s, 0, c, 0,
        0, 0, 0, 1
    };
}

Matrix4 Matrix4::CreateRotationZ(float radians) {
    const float c = Math::Cos(radians);
    const float s = Math::Sin(radians);

    return {
        c, s, 0, 0,
        -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

Matrix4 Matrix4::CreateRotation(const Vector3& rotation) {
    return CreateRotationY(rotation.Y) * CreateRotationX(rotation.X) * CreateRotationZ(rotation.Z);
}

Matrix4 Matrix4::CreateRotation(float radians, const Vector3& axis) {
    const float a = radians;
    const float c = Math::Cos(a);
    const float s = Math::Sin(a);
    const Vector3 naxis = Vector3::Normalize(axis);
    const Vector3 temp = naxis * (1.0f - c);

    Matrix4 result = Matrix4::Identity;
    result[0] = c + temp[0] * naxis[0];
    result[1] = 0 + temp[0] * naxis[1] + s * naxis[2];
    result[2] = 0 + temp[0] * naxis[2] - s * naxis[1];

    result[4] = 0 + temp[1] * naxis[0] - s * naxis[2];
    result[5] = c + temp[1] * naxis[1];
    result[6] = 0 + temp[1] * naxis[2] + s * naxis[0];

    result[8] = 0 + temp[2] * naxis[0] + s * naxis[1];
    result[9] = 0 + temp[2] * naxis[1] - s * naxis[0];
    result[10] = c + temp[2] * naxis[2];
    return result;
}

Matrix4 Matrix4::CreateScaling(const Vector3& scale) {
    return {
        scale.X, 0, 0, 0,
        0, scale.Y, 0, 0,
        0, 0, scale.Z, 0,
        0, 0, 0, 1
    };
}

Matrix4 Matrix4::Invert(const Matrix4& matrix) {
    const Matrix4 out = {
        matrix[5] * matrix[10] * matrix[15] -
        matrix[5] * matrix[11] * matrix[14] -
        matrix[9] * matrix[6] * matrix[15] +
        matrix[9] * matrix[7] * matrix[14] +
        matrix[13] * matrix[6] * matrix[11] -
        matrix[13] * matrix[7] * matrix[10],

        -matrix[1] * matrix[10] * matrix[15] +
        matrix[1] * matrix[11] * matrix[14] +
        matrix[9] * matrix[2] * matrix[15] -
        matrix[9] * matrix[3] * matrix[14] -
        matrix[13] * matrix[2] * matrix[11] +
        matrix[13] * matrix[3] * matrix[10],

        matrix[1] * matrix[6] * matrix[15] -
        matrix[1] * matrix[7] * matrix[14] -
        matrix[5] * matrix[2] * matrix[15] +
        matrix[5] * matrix[3] * matrix[14] +
        matrix[13] * matrix[2] * matrix[7] -
        matrix[13] * matrix[3] * matrix[6],

        -matrix[1] * matrix[6] * matrix[11] +
        matrix[1] * matrix[7] * matrix[10] +
        matrix[5] * matrix[2] * matrix[11] -
        matrix[5] * matrix[3] * matrix[10] -
        matrix[9] * matrix[2] * matrix[7] +
        matrix[9] * matrix[3] * matrix[6],

        -matrix[4] * matrix[10] * matrix[15] +
        matrix[4] * matrix[11] * matrix[14] +
        matrix[8] * matrix[6] * matrix[15] -
        matrix[8] * matrix[7] * matrix[14] -
        matrix[12] * matrix[6] * matrix[11] +
        matrix[12] * matrix[7] * matrix[10],

        matrix[0] * matrix[10] * matrix[15] -
        matrix[0] * matrix[11] * matrix[14] -
        matrix[8] * matrix[2] * matrix[15] +
        matrix[8] * matrix[3] * matrix[14] +
        matrix[12] * matrix[2] * matrix[11] -
        matrix[12] * matrix[3] * matrix[10],

        -matrix[0] * matrix[6] * matrix[15] +
        matrix[0] * matrix[7] * matrix[14] +
        matrix[4] * matrix[2] * matrix[15] -
        matrix[4] * matrix[3] * matrix[14] -
        matrix[12] * matrix[2] * matrix[7] +
        matrix[12] * matrix[3] * matrix[6],

        matrix[0] * matrix[6] * matrix[11] -
        matrix[0] * matrix[7] * matrix[10] -
        matrix[4] * matrix[2] * matrix[11] +
        matrix[4] * matrix[3] * matrix[10] +
        matrix[8] * matrix[2] * matrix[7] -
        matrix[8] * matrix[3] * matrix[6],

        matrix[4] * matrix[9] * matrix[15] -
        matrix[4] * matrix[11] * matrix[13] -
        matrix[8] * matrix[5] * matrix[15] +
        matrix[8] * matrix[7] * matrix[13] +
        matrix[12] * matrix[5] * matrix[11] -
        matrix[12] * matrix[7] * matrix[9],

        -matrix[0] * matrix[9] * matrix[15] +
        matrix[0] * matrix[11] * matrix[13] +
        matrix[8] * matrix[1] * matrix[15] -
        matrix[8] * matrix[3] * matrix[13] -
        matrix[12] * matrix[1] * matrix[11] +
        matrix[12] * matrix[3] * matrix[9],

        matrix[0] * matrix[5] * matrix[15] -
        matrix[0] * matrix[7] * matrix[13] -
        matrix[4] * matrix[1] * matrix[15] +
        matrix[4] * matrix[3] * matrix[13] +
        matrix[12] * matrix[1] * matrix[7] -
        matrix[12] * matrix[3] * matrix[5],

        -matrix[0] * matrix[5] * matrix[11] +
        matrix[0] * matrix[7] * matrix[9] +
        matrix[4] * matrix[1] * matrix[11] -
        matrix[4] * matrix[3] * matrix[9] -
        matrix[8] * matrix[1] * matrix[7] +
        matrix[8] * matrix[3] * matrix[5],

        -matrix[4] * matrix[9] * matrix[14] +
        matrix[4] * matrix[10] * matrix[13] +
        matrix[8] * matrix[5] * matrix[14] -
        matrix[8] * matrix[6] * matrix[13] -
        matrix[12] * matrix[5] * matrix[10] +
        matrix[12] * matrix[6] * matrix[9],

        matrix[0] * matrix[9] * matrix[14] -
        matrix[0] * matrix[10] * matrix[13] -
        matrix[8] * matrix[1] * matrix[14] +
        matrix[8] * matrix[2] * matrix[13] +
        matrix[12] * matrix[1] * matrix[10] -
        matrix[12] * matrix[2] * matrix[9],

        -matrix[0] * matrix[5] * matrix[14] +
        matrix[0] * matrix[6] * matrix[13] +
        matrix[4] * matrix[1] * matrix[14] -
        matrix[4] * matrix[2] * matrix[13] -
        matrix[12] * matrix[1] * matrix[6] +
        matrix[12] * matrix[2] * matrix[5],

        matrix[0] * matrix[5] * matrix[10] -
        matrix[0] * matrix[6] * matrix[9] -
        matrix[4] * matrix[1] * matrix[10] +
        matrix[4] * matrix[2] * matrix[9] +
        matrix[8] * matrix[1] * matrix[6] -
        matrix[8] * matrix[2] * matrix[5]
    };

    const float det = matrix[0] * out[0] + matrix[1] * out[4] + matrix[2] * out[8] + matrix[3] * out[12];
    const float inv_det = det != 0 ? (1 / det) : 0;

    return {
        out[0] * inv_det, out[1] * inv_det, out[2] * inv_det, out[3] * inv_det,
        out[4] * inv_det, out[5] * inv_det, out[6] * inv_det, out[7] * inv_det,
        out[8] * inv_det, out[9] * inv_det, out[10] * inv_det, out[11] * inv_det,
        out[12] * inv_det, out[13] * inv_det, out[14] * inv_det, out[15] * inv_det
    };
}

Matrix4 Matrix4::Transpose(const Matrix4& matrix) {
    return {
        matrix[0], matrix[4], matrix[8], matrix[12],
        matrix[1], matrix[5], matrix[9], matrix[13],
        matrix[2], matrix[6], matrix[10], matrix[14],
        matrix[3], matrix[7], matrix[11], matrix[15]
    };
}

Vector3 Matrix4::TransformNormal(const Matrix4& matrix, const Vector3& vector) {
    return Vector3(
        vector.X * matrix[0] + vector.Y * matrix[4] + vector.Z * matrix[8],
        vector.X * matrix[1] + vector.Y * matrix[5] + vector.Z * matrix[9],
        vector.X * matrix[2] + vector.Y * matrix[6] + vector.Z * matrix[10]
    );
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
    const Matrix4& lhs = *this;

    return {
        lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8] * rhs[2] + lhs[12] * rhs[3],
        lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9] * rhs[2] + lhs[13] * rhs[3],
        lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3],
        lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3],

        lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8] * rhs[6] + lhs[12] * rhs[7],
        lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9] * rhs[6] + lhs[13] * rhs[7],
        lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7],
        lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7],

        lhs[0] * rhs[8] + lhs[4] * rhs[9] + lhs[8] * rhs[10] + lhs[12] * rhs[11],
        lhs[1] * rhs[8] + lhs[5] * rhs[9] + lhs[9] * rhs[10] + lhs[13] * rhs[11],
        lhs[2] * rhs[8] + lhs[6] * rhs[9] + lhs[10] * rhs[10] + lhs[14] * rhs[11],
        lhs[3] * rhs[8] + lhs[7] * rhs[9] + lhs[11] * rhs[10] + lhs[15] * rhs[11],

        lhs[0] * rhs[12] + lhs[4] * rhs[13] + lhs[8] * rhs[14] + lhs[12] * rhs[15],
        lhs[1] * rhs[12] + lhs[5] * rhs[13] + lhs[9] * rhs[14] + lhs[13] * rhs[15],
        lhs[2] * rhs[12] + lhs[6] * rhs[13] + lhs[10] * rhs[14] + lhs[14] * rhs[15],
        lhs[3] * rhs[12] + lhs[7] * rhs[13] + lhs[11] * rhs[14] + lhs[15] * rhs[15]
    };
}

Vector4 Matrix4::operator*(const Vector4& rhs) const {
    return Vector4(
        rhs.X * Values[0] + rhs.Y * Values[4] + rhs.Z * Values[8] + rhs.W * Values[12],
        rhs.X * Values[1] + rhs.Y * Values[5] + rhs.Z * Values[9] + rhs.W * Values[13],
        rhs.X * Values[2] + rhs.Y * Values[6] + rhs.Z * Values[10] + rhs.W * Values[14],
        rhs.X * Values[3] + rhs.Y * Values[7] + rhs.Z * Values[11] + rhs.W * Values[15]
    );
}

float& Matrix4::operator[](int index) {
    assert(index < 16);
    return Values[index];
}

const float& Matrix4::operator[](int index) const {
    assert(index < 16);
    return Values[index];
}

bool Matrix4::operator==(const Matrix4& rhs) const {
    return memcmp(Values, rhs.Values, sizeof(Values)) == 0;
}

bool Matrix4::operator!=(const Matrix4& rhs) const {
    return memcmp(Values, rhs.Values, sizeof(Values)) != 0;
}
