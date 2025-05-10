/**
 * @file karm_example.c
 * @brief N维卡尔曼滤波算法使用示例
 */

 #include <stdio.h>
 #include "kalman.h"
 
 /**
  * @brief 一维卡尔曼滤波示例
  */
 void kalman_1d_example(void) {
     printf("一维卡尔曼滤波示例\n");
     
     // 创建一维卡尔曼滤波器
     KalmanFilter* kf = kalman_create(1);
     if (!kf) {
         printf("创建卡尔曼滤波器失败\n");
         return;
     }
     
     // 初始状态
     float x_init = 0.0f;
     kalman_init(kf, &x_init);
     
     // 设置过程噪声和测量噪声
     float Q = 0.01f;  // 过程噪声方差
     float R = 0.1f;   // 测量噪声方差
     kalman_set_process_noise(kf, &Q);
     kalman_set_measurement_noise(kf, &R);
     
     // 模拟测量数据（带噪声）
     float measurements[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
     
     // 运行卡尔曼滤波
     printf("测量值\t估计值\n");
     for (int i = 0; i < 10; i++) {
         // 更新滤波器
         kalman_filter(kf, &measurements[i]);
         
         // 获取当前状态估计
         float state;
         kalman_get_state(kf, &state);
         
         printf("%.2f\t%.2f\n", measurements[i], state);
     }
     
     // 释放资源
     kalman_destroy(kf);
 }
 
 /**
  * @brief 二维卡尔曼滤波示例（位置-速度模型）
  */
 void kalman_2d_example(void) {
     printf("\n二维卡尔曼滤波示例（位置-速度模型）\n");
     
     // 创建二维卡尔曼滤波器
     KalmanFilter* kf = kalman_create(2);
     if (!kf) {
         printf("创建卡尔曼滤波器失败\n");
         return;
     }
     
     // 初始状态 [位置, 速度]
     float x_init[2] = {0.0f, 1.0f};
     kalman_init(kf, x_init);
     
     // 设置状态转移矩阵 (考虑时间步长dt=1)
     // [ 1 dt ]
     // [ 0  1 ]
     float A[4] = {
         1.0f, 1.0f,
         0.0f, 1.0f
     };
     kalman_set_transition_matrix(kf, A);
     
     // 设置过程噪声
     float Q[4] = {
         0.01f, 0.0f,
         0.0f, 0.01f
     };
     kalman_set_process_noise(kf, Q);
     
     // 设置观测矩阵 (只观测位置)
     // [ 1 0 ]
     float H[4] = {
         1.0f, 0.0f,
         0.0f, 0.0f
     };
     kalman_set_observation_matrix(kf, H);
     
     // 设置测量噪声
     float R[4] = {
         0.1f, 0.0f,
         0.0f, 0.1f
     };
     kalman_set_measurement_noise(kf, R);
     
     // 模拟测量数据（带噪声的位置）
     float measurements[10][2] = {
         {1.1f, 0.0f}, {2.2f, 0.0f}, {3.1f, 0.0f}, {4.3f, 0.0f}, {5.2f, 0.0f},
         {6.1f, 0.0f}, {7.0f, 0.0f}, {8.2f, 0.0f}, {9.1f, 0.0f}, {10.3f, 0.0f}
     };
     
     // 运行卡尔曼滤波
     printf("测量位置\t估计位置\t估计速度\n");
     for (int i = 0; i < 10; i++) {
         // 更新滤波器
         kalman_filter(kf, measurements[i]);
         
         // 获取当前状态估计
         float state[2];
         kalman_get_state(kf, state);
         
         printf("%.2f\t\t%.2f\t\t%.2f\n", measurements[i][0], state[0], state[1]);
     }
     
     // 释放资源
     kalman_destroy(kf);
 }
 
 /**
  * @brief 主函数
  */
 int main(void) {
     // 运行一维卡尔曼滤波示例
     kalman_1d_example();
     
     // 运行二维卡尔曼滤波示例
     kalman_2d_example();
     
     return 0;
 }