#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define M 2 //行
#define N 2 //列
#define random(a,b) (rand() % (b-a))+ a
#define new_area int**
//任意向量分解为基向量(x,0),(0,y)->任意(x,y)=(a*xi+b*xj, a*yi+b*yj)
int area_x[M][N] = {0};

int area_y[M][N] = {0};


typedef struct 
{
    int* area;
    int m;
    int n;
}matrix;


matrix X = {NULL,M,N};
matrix Y = {NULL,M,N};

void init_area(matrix* area)
{
    area->area = (int*)malloc((area->m)*(area->n)*sizeof(int));
    for(int i = 0; i<M; i++)
    {
        for(int j = 0; j<N; j++)
        {
            *(area->area+i*(area->n)+j) = random(1,100);
        }
    }
}


void list_area(matrix area)
{
    for(int i = 0; i<M; i++)
    {
        for(int j = 0; j<N; j++)
        {
            printf(" %d",*(area.area+i*area.n+j));
        }
        printf("\n");
    }
}

matrix area_add(matrix x, matrix y)
{
    matrix new = {NULL,0,0};
    if((x.m != y.m)||(x.n != y.n))
    {
        return new;
    }
    else
    {
        new.m = x.m;
        new.n = x.n;
        new.area = (int *)malloc(new.m*new.n*sizeof(int));
        for(int i = 0; i<new.m; i++)
        {
            for(int j = 0; j<new.n; j++)
            {
                *(new.area+i*new.n+j) = *(x.area+i*x.n+j)+*(y.area+i*y.n+j);
            }
        }
        return new;
    }
    
}
//规定x为矩阵，y为单位（1维）矩阵（基向量）
matrix area_x_vector(matrix x, matrix y)
{
    matrix new = {NULL,0,0};
    if((x.m == 0)||(y.m == 0))
    {
        return new;
    }
    else
    {
        new.m = x.m;
        new.n = y.n;
    }
}




int main(void)
{
    init_area(&X);
    init_area(&Y);
    matrix z = area_add(X,Y);
    list_area(X);
    list_area(Y);
    list_area(z);
    free(z.area);



    return 0;
}



