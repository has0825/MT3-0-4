#include <Novice.h>
#include <cmath>
#include <cassert>
#include <cstdio>

const char kWindowTitle[] = "LC1B_12_シミズグチ_ハル";
const int kRowHeight = 20;

// ===== 構造体 =====
struct Vector3 {
    float x, y, z;
};

struct Matrix4x4 {
    float m[4][4];
};

// ===== 行列作成関数 =====

// X軸回転
Matrix4x4 MakeRotateXMatrix(float radian) {
    Matrix4x4 result = {};
    result.m[0][0] = 1;
    result.m[1][1] = std::cos(radian);
    result.m[1][2] = std::sin(radian);
    result.m[2][1] = -std::sin(radian);
    result.m[2][2] = std::cos(radian);
    result.m[3][3] = 1;
    return result;
}

// Y軸回転
Matrix4x4 MakeRotateYMatrix(float radian) {
    Matrix4x4 result = {};
    result.m[0][0] = std::cos(radian);
    result.m[0][2] = -std::sin(radian);
    result.m[1][1] = 1;
    result.m[2][0] = std::sin(radian);
    result.m[2][2] = std::cos(radian);
    result.m[3][3] = 1;
    return result;
}

// Z軸回転
Matrix4x4 MakeRotateZMatrix(float radian) {
    Matrix4x4 result = {};
    result.m[0][0] = std::cos(radian);
    result.m[0][1] = -std::sin(radian); // ←符号反転
    result.m[1][0] = std::sin(radian);  // ←符号反転
    result.m[1][1] = std::cos(radian);
    result.m[2][2] = 1.0f;
    result.m[3][3] = 1.0f;
    return result;
}


// ===== 行列同士の積 =====
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result = {};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++) {
                result.m[row][col] += m1.m[row][k] * m2.m[k][col];
            }
        }
    }
    return result;
}

// ===== 行列の表示 =====
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
    Novice::ScreenPrintf(x, y, "%s:", label);
    for (int row = 0; row < 4; ++row) {
        Novice::ScreenPrintf(x, y + (row + 1) * kRowHeight,
            "%.2f %.2f %.2f %.2f",
            matrix.m[row][0], matrix.m[row][1],
            matrix.m[row][2], matrix.m[row][3]);
    }
}

// ===== エントリーポイント =====
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    Novice::Initialize(kWindowTitle, 1280, 720);

    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // ✅ 指定された回転角に修正（ラジアン）
    Vector3 rotate = {
        0.4f,   // X軸回転 → sin ≈ 0.39, cos ≈ 0.92
        1.43f,  // Y軸回転 → sin ≈ 0.99, cos ≈ 0.14
        0.8f    // Z軸回転 → sin ≈ 0.72, cos ≈ 0.70
    };

    // ✅ 計算によって指定された行列になる
    Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
    Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
    Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

    // 合成回転行列（Z→Y→X）
    Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

    // メインループ
    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        MatrixScreenPrintf(0, 0, rotateXMatrix, "RotateXMatrix");
        MatrixScreenPrintf(0, 120, rotateYMatrix, "RotateYMatrix");
        MatrixScreenPrintf(0, 240, rotateZMatrix, "RotateZMatrix");
        MatrixScreenPrintf(0, kRowHeight * 18, rotateXYZMatrix, "RotateXYZMatrix");

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    Novice::Finalize();
    return 0;
}
