/**
 * @file karm.h
 * @brief N维卡尔曼滤波算法头文件
 * @details 定义了可配置维度的卡尔曼滤波算法接口
 */

 #ifndef KARM_H
 #define KARM_H
 
 /**
  * @brief 卡尔曼滤波器结构体
  */
 typedef struct {
     int dim;        /**< 状态向量维度 */
     float* x;       /**< 状态向量 */
     float* P;       /**< 状态协方差矩阵 */
     float* Q;       /**< 过程噪声协方差矩阵 */
     float* R;       /**< 测量噪声协方差矩阵 */
     float* K;       /**< 卡尔曼增益 */
     float* A;       /**< 状态转移矩阵 */
     float* H;       /**< 观测矩阵 */
 } KalmanFilter;
 
 /**
  * @brief 创建n维卡尔曼滤波器
  * @param dim 状态向量维度
  * @return 卡尔曼滤波器结构体指针，失败返回NULL
  */
 KalmanFilter* kalman_create(int dim);
 
 /**
  * @brief 销毁卡尔曼滤波器并释放内存
  * @param kf 卡尔曼滤波器结构体指针
  */
 void kalman_destroy(KalmanFilter* kf);
 
 /**
  * @brief 初始化卡尔曼滤波器状态
  * @param kf 卡尔曼滤波器结构体指针
  * @param x_init 初始状态向量
  */
 void kalman_init(KalmanFilter* kf, const float* x_init);
 
 /**
  * @brief 设置状态转移矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param A 状态转移矩阵，大小为dim*dim
  */
 void kalman_set_transition_matrix(KalmanFilter* kf, const float* A);
 
 /**
  * @brief 设置观测矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param H 观测矩阵，大小为dim*dim
  */
 void kalman_set_observation_matrix(KalmanFilter* kf, const float* H);
 
 /**
  * @brief 设置过程噪声协方差矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param Q 过程噪声协方差矩阵，大小为dim*dim
  */
 void kalman_set_process_noise(KalmanFilter* kf, const float* Q);
 
 /**
  * @brief 设置测量噪声协方差矩阵
  * @param kf 卡尔曼滤波器结构体指针
  * @param R 测量噪声协方差矩阵，大小为dim*dim
  */
 void kalman_set_measurement_noise(KalmanFilter* kf, const float* R);
 
 /**
  * @brief 卡尔曼滤波预测步骤
  * @param kf 卡尔曼滤波器结构体指针
  */
 void kalman_predict(KalmanFilter* kf);
 
 /**
  * @brief 卡尔曼滤波更新步骤
  * @param kf 卡尔曼滤波器结构体指针
  * @param z 观测向量
  */
 void kalman_update(KalmanFilter* kf, const float* z);
 
 /**
  * @brief 获取当前状态估计
  * @param kf 卡尔曼滤波器结构体指针
  * @param x_out 输出状态向量
  */
 void kalman_get_state(const KalmanFilter* kf, float* x_out);
 
 /**
  * @brief 一步完成预测和更新
  * @param kf 卡尔曼滤波器结构体指针
  * @param z 观测向量
  */
 void kalman_filter(KalmanFilter* kf, const float* z);
 
 #endif /* KARM_H */