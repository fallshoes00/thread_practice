#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 100

int fib[MAX] = {0}; // 用於存儲Fibonacci數列結果
int n;              // Fibonacci數列的計算目標值
volatile int complete = 0; // 用於指示計算是否完成

void* calculate_fib(void* arg) {
    fib[1] = 1;
    for (int i = 2; i <= n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
        usleep(50000); // 模擬計算時間 (50毫秒)
    }
    complete = 1; // 計算完成
    return NULL;
}

void* progress_bar(void* arg) {
    while (!complete) {
        int filled = (n > 0) ? (50 * (n - fib[0]) / n) : 0;
        printf("\r[%-50s] %d%%", filled > 0 ? "##################################################" : "", filled * 2);
        fflush(stdout);
        usleep(100000); // 每100毫秒刷新一次
    }
    printf("\r[%-50s] 100%%\n", "##################################################");
    return NULL;
}

int main() {
    printf("Enter the value of n: ");
    scanf("%d", &n);

    if (n > MAX) {
        printf("Value of n is too large. Max allowed is %d.\n", MAX);
        return 1;
    }

    pthread_t fib_thread, progress_thread;

    // 創建計算Fibonacci數列的線程
    pthread_create(&fib_thread, NULL, calculate_fib, NULL);
    // 創建顯示進度條的線程
    pthread_create(&progress_thread, NULL, progress_bar, NULL);

    // 等待線程完成
    pthread_join(fib_thread, NULL);
    pthread_join(progress_thread, NULL);

    printf("Fibonacci(%d) = %d\n", n, fib[n]);

    return 0;
}
