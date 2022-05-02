#include<pthread.h>
#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<unistd.h>
#define NUM 40000
#define MAX 40000
int initlist[NUM];
int a[NUM/2];
int b[NUM/2];
int sortlist[NUM];

typedef struct
{
    int start;
    int end;
}parameters;// 开始和结束的范围
void *sort(void *data);//排序函数
void *merge(void *arg);//归并函数

/*随机生成4*10^4个数作为实验数据*/
void initElem(){
    srand((unsigned)time(NULL));
    for(int i=0; i<NUM; i++){
        initlist[i] = rand()%MAX;
    }
}

/*归并算法*/
void *merge(void *arg){
    int x=NUM/2-1;
    int y=NUM-1;
    int num=NUM-1;
    while( x>=0 && y>=NUM/2 ){
        if(initlist[x] >= initlist[y]){
            sortlist[num--] = initlist[x--];
        }
        else{
            sortlist[num--] = initlist[y--];
        }
        while( y >= NUM/2){
            sortlist[num--] = initlist[y--];
        }
        while( x >= 0){
            sortlist[num--] = initlist[x--];
        }
        pthread_exit(0);//退出线程
    }
}

/*排序算法*/
void *sort(void *data){
    parameters *r=(parameters *)data;
    int temp;
    int i,j;
    for(int i=r->start + 1; i<r->end ; i++){
        for(j = r->start; j<r->end - i + r->start; j++){
            if(initlist[j] > initlist[j+1]){
                temp = initlist[j];
                initlist[j] = initlist[j+1];
                initlist[j+1] = temp; 
            }
        }
    }
    pthread_exit(0);
}

int main(int argc,char* argv[]){
    int i;
    parameters *a,*b,*c;
    a = (parameters *)malloc(sizeof(parameters));
    b = (parameters *)malloc(sizeof(parameters));
    c = (parameters *)malloc(sizeof(parameters));
    initElem();
    a->start = 0;
    a->end = NUM/2;
    b->start = NUM/2;
    b->end = NUM;
    c->start = 0;
    c->end = NUM;
    
    struct timeval starttime,endtime;//计时1
    struct timeval start,end;//计时2
    gettimeofday(&starttime, NULL);

    pthread_t sort1,sort2,mer,sort3;
/*创建了一个线程 sort1*/
    pthread_create(&sort1, NULL, sort, (void *)a);
/*创建一个线程 sort2*/
    pthread_create(&sort2, NULL, sort, (void *)b);
    pthread_join(sort1, NULL);
    pthread_join(sort2, NULL);

/*创建一个 merge线程*/
    pthread_create(&mer, NULL, merge, NULL);
    pthread_join(mer, NULL);

/*屏幕输出*/
    // printf("The sorted list is:\n");
    // for(i=0; i<NUM;i++){
    //     printf("%d\t",sortlist[i]);
    // }
    printf("\n");
/*时间输出1*/
    gettimeofday(&endtime, NULL);
    double timeuse = 1000000*(endtime.tv_sec-starttime.tv_sec)+endtime.tv_usec-starttime.tv_usec;
    timeuse /= 1000;//除以1000则进行一毫秒计时
    printf("Double thread and merge time = %f ms\n",timeuse);


gettimeofday(&start, NULL);
    pthread_create(&sort3, NULL, sort, (void *)c);
    pthread_join(sort3, NULL);
/*时间输出2*/
    gettimeofday(&end, NULL);
    double timeuse2 = 1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
    timeuse2 /= 1000;//除以1000则进行一毫秒计时
    printf("Single thread and merge time = %f ms\n",timeuse2);

}


