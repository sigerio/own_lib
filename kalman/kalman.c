/**
 * @file kalman.c
 * @brief N维卡尔曼滤波算法实现
 * @details 实现了可配置维度的卡尔曼滤波算法，支持状态估计和噪声抑制
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include "kalman.h"
 
 /**
  * @brief 创建n维卡尔曼滤波器
  * @param dim 状态向量维度
  * @return 卡尔曼滤波器结构体指针，失败返回NULL
  */
 KalmanFilter* kalman_create(int dim) {
     if (dim <= 0) {
         return NULL;
     }
     
     KalmanFilter* kf = (KalmanFilter*)malloc(sizeof(KalmanFilter));
     if (kf == NULL) {
         return NULL;
     }
     
     // 初始化维度
     kf->dim = dim;
     
     // 分配内存
     kf->x = (float*)malloc(dim * sizeof(float));  // 状态向量
     kf->P = (float*)malloc(dim * dim * sizeof(float));  // 状态协方差矩阵
     kf->Q = (float*)malloc(dim * dim * sizeof(float));  // 过程噪声协方差
     kf->R = (float*)malloc(dim * dim * sizeof(float));  // 测量噪声协方差
     kf->K = (float*)malloc(dim * dim * sizeof(float));  // 卡尔曼增益
     kf->A = (float*)malloc(dim * dim * sizeof(float));  // 状态转移矩阵
     kf->H = (float*)malloc(dim * dim * sizeof(float));  // 观测矩阵
     
     // 检查内存分配
     if (!kf->x || !kf->P || !kf->Q || !kf->R || !kf->K || !kf->A || !kf->H) {
         kalman_destroy(kf);
         return NULL;
     }
     
     // 初始化为0
     memset(kf->x, 0, dim * sizeof(float));
     memset(kf->P, 0, dim * dim * sizeof(float));
     memset(kf->Q, 0, dim * dim * sizeof(float));
     memset(kf->R, 0, dim * dim * sizeof(float));
     memset(kf->K, 0, dim * dim * sizeof(float));
     memset(kf->A, 0, dim * dim * sizeof(float));
     memset(kf->H, 0, dim * dim * sizeof(float));
     
     // 设置默认值
     // 状态转移矩阵默认为单位矩阵
     // 观测矩阵默认为单位矩阵
     for (int i = 0; i < dim; i++) {
         kf->A[i * dim + i] = 1.0f;
         kf->H[i * dim + i] = 1.0f;
         
         // 状态协方差矩阵对角线设为1.0
         kf->P[i * dim + i] = 1.0f;
         
         // 过程噪声和测量噪声默认值
         kf->Q[i * dim + i] = 0.01f;
         kf->R[i * dim + i] = 0.1f;
     }
     
     return kf;
 }
 
 /**
  * @brief 销毁卡尔曼滤波器并释放内存
  * @param kf 卡尔曼滤波器结构体指针
  */
 void kalman_destroy(KalmanFilter* kf) {
     if (kf) {
         if (kf->x) free(kf->x);
         if (kf->P) free(kf->P);
         if (kf->Q) free(kf->Q);
         if (kf->R) free(kf->R);
         if (kf->K) free(kf->K);
         if (kf->A) free(kf->A);
         if (kf->H) free(kf->H);
         free(kf);
     }
 }
 
 /**
  * @brief 初始化卡尔曼滤波器状态
  * @param kf 卡尔曼滤波器结构体指针
  * @param x_init 初始状态向量
  */
 void kalman_init(KalmanFilter* kf, const float* x_init) {
     if (!kf || !x_init) return;
     
     // 复制初始状态
     memcpy(kf->x, x_init, kf->dim * sizeof(float));
 }
 
 /**
  * @brief 设置状态转移矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param A 状态转移矩阵，大小为dim*dim
  */
 void kalman_set_transition_matrix(KalmanFilter* kf, const float* A) {
     if (!kf || !A) return;
     
     memcpy(kf->A, A, kf->dim * kf->dim * sizeof(float));
 }
 
 /**
  * @brief 设置观测矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param H 观测矩阵，大小为dim*dim
  */
 void kalman_set_observation_matrix(KalmanFilter* kf, const float* H) {
     if (!kf || !H) return;
     
     memcpy(kf->H, H, kf->dim * kf->dim * sizeof(float));
 }
 
 /**
  * @brief 设置过程噪声协方差矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param Q 过程噪声协方差矩阵，大小为dim*dim
  */
 void kalman_set_process_noise(KalmanFilter* kf, const float* Q) {
     if (!kf || !Q) return;
     
     memcpy(kf->Q, Q, kf->dim * kf->dim * sizeof(float));
 }
 
 /**
  * @brief 设置测量噪声协方差矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param R 测量噪声协方差矩阵，大小为dim*dim
  */
 void kalman_set_measurement_noise(KalmanFilter* kf, const float* R) {
     if (!kf || !R) return;
     
     memcpy(kf->R, R, kf->dim * kf->dim * sizeof(float));
 }
 
 /**
  * @brief 矩阵乘法 C = A * B
  * @param A 矩阵A
  * @param B 矩阵B
  * @param C 结果矩阵C
  * @param rows_a A的行数
  * @param cols_a A的列数
  * @param cols_b B的列数
  */
 static void matrix_multiply(const float* A, const float* B, float* C, int rows_a, int cols_a, int cols_b) {
     for (int i = 0; i < rows_a; i++) {
         for (int j = 0; j < cols_b; j++) {
             C[i * cols_b + j] = 0;
             for (int k = 0; k < cols_a; k++) {
                 C[i * cols_b + j] += A[i * cols_a + k] * B[k * cols_b + j];
             }
         }
     }
 }
 
 /**
  * @brief 矩阵加法 C = A + B
  * @param A 矩阵A
  * @param B 矩阵B
  * @param C 结果矩阵C
  * @param rows 矩阵行数
  * @param cols 矩阵列数
  */
 static void matrix_add(const float* A, const float* B, float* C, int rows, int cols) {
     for (int i = 0; i < rows * cols; i++) {
         C[i] = A[i] + B[i];
     }
 }
 
 /**
  * @brief 矩阵减法 C = A - B
  * @param A 矩阵A
  * @param B 矩阵B
  * @param C 结果矩阵C
  * @param rows 矩阵行数
  * @param cols 矩阵列数
  */
 static void matrix_subtract(const float* A, const float* B, float* C, int rows, int cols) {
     for (int i = 0; i < rows * cols; i++) {
         C[i] = A[i] - B[i];
     }
 }
 
 /**
  * @brief 矩阵转置 B = A^T
  * @param A 矩阵A
  * @param B 结果矩阵B
  * @param rows 矩阵A的行数
  * @param cols 矩阵A的列数
  */
 static void matrix_transpose(const float* A, float* B, int rows, int cols) {
     for (int i = 0; i < rows; i++) {
         for (int j = 0; j < cols; j++) {
             B[j * rows + i] = A[i * cols + j];
         }
     }
 }
 
 /**
  * @brief 矩阵求逆 (仅适用于小维度矩阵，使用高斯-约旦消元法)
  * @param A 输入矩阵
  * @param Ainv 结果矩阵（A的逆）
  * @param n 矩阵维度
  * @return 0成功，-1失败
  */
 static int matrix_inverse(const float* A, float* Ainv, int n) {
     // 创建增广矩阵 [A|I]
     float* aug = (float*)malloc(n * 2 * n * sizeof(float));
     if (!aug) return -1;
     
     // 初始化增广矩阵
     for (int i = 0; i < n; i++) {
         for (int j = 0; j < n; j++) {
             aug[i * (2 * n) + j] = A[i * n + j];
             aug[i * (2 * n) + j + n] = (i == j) ? 1.0f : 0.0f;
         }
     }
     
     // 高斯-约旦消元
     for (int i = 0; i < n; i++) {
         // 查找主元
         float max_val = fabsf(aug[i * (2 * n) + i]);
         int max_row = i;
         for (int j = i + 1; j < n; j++) {
             if (fabsf(aug[j * (2 * n) + i]) > max_val) {
                 max_val = fabsf(aug[j * (2 * n) + i]);
                 max_row = j;
             }
         }
         
         // 如果主元接近于0，矩阵可能是奇异的
         if (max_val < 1e-10) {
             free(aug);
             return -1;
         }
         
         // 交换行
         if (max_row != i) {
             for (int j = 0; j < 2 * n; j++) {
                 float temp = aug[i * (2 * n) + j];
                 aug[i * (2 * n) + j] = aug[max_row * (2 * n) + j];
                 aug[max_row * (2 * n) + j] = temp;
             }
         }
         
         // 缩放行
         float pivot = aug[i * (2 * n) + i];
         for (int j = 0; j < 2 * n; j++) {
             aug[i * (2 * n) + j] /= pivot;
         }
         
         // 消元
         for (int j = 0; j < n; j++) {
             if (j != i) {
                 float factor = aug[j * (2 * n) + i];
                 for (int k = 0; k < 2 * n; k++) {
                     aug[j * (2 * n) + k] -= factor * aug[i * (2 * n) + k];
                 }
             }
         }
     }
     
     // 提取逆矩阵部分
     for (int i = 0; i < n; i++) {
         for (int j = 0; j < n; j++) {
             Ainv[i * n + j] = aug[i * (2 * n) + j + n];
         }
     }
     
     free(aug);
     return 0;
 }
 
 /**
  * @brief 卡尔曼滤波预测步骤
  * @param kf 卡尔曼滤波器结构体指针
  */
 void kalman_predict(KalmanFilter* kf) {
     if (!kf) return;
     
     int dim = kf->dim;
     
     // 临时矩阵
     float* temp = (float*)malloc(dim * dim * sizeof(float));
     float* temp2 = (float*)malloc(dim * sizeof(float));
     
     if (!temp || !temp2) {
         if (temp) free(temp);
         if (temp2) free(temp2);
         return;
     }
     
     // 1. 预测状态: x = A * x
     matrix_multiply(kf->A, kf->x, temp2, dim, dim, 1);
     memcpy(kf->x, temp2, dim * sizeof(float));
     
     // 2. 预测误差协方差: P = A * P * A^T + Q
     // 计算 A * P
     matrix_multiply(kf->A, kf->P, temp, dim, dim, dim);
     
     // 计算 (A * P) * A^T
     float* AT = (float*)malloc(dim * dim * sizeof(float));
     if (!AT) {
         free(temp);
         free(temp2);
         return;
     }
     
     matrix_transpose(kf->A, AT, dim, dim);
     matrix_multiply(temp, AT, kf->P, dim, dim, dim);
     
     // 加上过程噪声 P = P + Q
     matrix_add(kf->P, kf->Q, kf->P, dim, dim);
     
     // 释放临时内存
     free(temp);
     free(temp2);
     free(AT);
 }
 
 /**
  * @brief 卡尔曼滤波更新步骤
  * @param kf 卡尔曼滤波器结构体指针
  * @param z 观测向量
  */
 void kalman_update(KalmanFilter* kf, const float* z) {
     if (!kf || !z) return;
     
     int dim = kf->dim;
     
     // 临时矩阵
     float* temp = (float*)malloc(dim * dim * sizeof(float));
     float* temp2 = (float*)malloc(dim * dim * sizeof(float));
     float* temp3 = (float*)malloc(dim * sizeof(float));
     float* y = (float*)malloc(dim * sizeof(float));
     float* S = (float*)malloc(dim * dim * sizeof(float));
     float* S_inv = (float*)malloc(dim * dim * sizeof(float));
     float* HT = (float*)malloc(dim * dim * sizeof(float));
     
     if (!temp || !temp2 || !temp3 || !y || !S || !S_inv || !HT) {
         if (temp) free(temp);
         if (temp2) free(temp2);
         if (temp3) free(temp3);
         if (y) free(y);
         if (S) free(S);
         if (S_inv) free(S_inv);
         if (HT) free(HT);
         return;
     }
     
     // 1. 计算测量残差: y = z - H * x
     matrix_multiply(kf->H, kf->x, temp3, dim, dim, 1);
     matrix_subtract(z, temp3, y, dim, 1);
     
     // 2. 计算残差协方差: S = H * P * H^T + R
     matrix_multiply(kf->H, kf->P, temp, dim, dim, dim);
     matrix_transpose(kf->H, HT, dim, dim);
     matrix_multiply(temp, HT, temp2, dim, dim, dim);
     matrix_add(temp2, kf->R, S, dim, dim);
     
     // 3. 计算卡尔曼增益: K = P * H^T * S^-1
     if (matrix_inverse(S, S_inv, dim) != 0) {
         // 矩阵求逆失败
         free(temp);
         free(temp2);
         free(temp3);
         free(y);
         free(S);
         free(S_inv);
         free(HT);
         return;
     }
     
     matrix_multiply(kf->P, HT, temp, dim, dim, dim);
     matrix_multiply(temp, S_inv, kf->K, dim, dim, dim);
     
     // 4. 更新状态估计: x = x + K * y
     matrix_multiply(kf->K, y, temp3, dim, dim, 1);
     matrix_add(kf->x, temp3, kf->x, dim, 1);
     
     // 5. 更新误差协方差: P = (I - K * H) * P
     matrix_multiply(kf->K, kf->H, temp, dim, dim, dim);
     
     // 创建单位矩阵 I
     for (int i = 0; i < dim * dim; i++) {
         temp2[i] = 0;
     }
     for (int i = 0; i < dim; i++) {
         temp2[i * dim + i] = 1.0f;
     }
     
     // 计算 I - K * H
     matrix_subtract(temp2, temp, temp, dim, dim);
     
     // 计算 (I - K * H) * P
     matrix_multiply(temp, kf->P, temp2, dim, dim, dim);
     memcpy(kf->P, temp2, dim * dim * sizeof(float));
     
     // 释放临时内存
     free(temp);
     free(temp2);
     free(temp3);
     free(y);
     free(S);
     free(S_inv);
     free(HT);
 }
 
 /**
  * @brief 获取当前状态估计
  * @param kf 卡尔曼滤波器结构体指针
  * @param x_out 输出状态向量
  */
 void kalman_get_state(const KalmanFilter* kf, float* x_out) {
     if (!kf || !x_out) return;
     
     memcpy(x_out, kf->x, kf->dim * sizeof(float));
 }
 
 /**
  * @brief 一步完成预测和更新
  * @param kf 卡尔曼滤波器结构体指针
  * @param z 观测向量
  */
 void kalman_filter(KalmanFilter* kf, const float* z) {
     if (!kf || !z) return;
     
     kalman_predict(kf);
     kalman_update(kf, z);
 }