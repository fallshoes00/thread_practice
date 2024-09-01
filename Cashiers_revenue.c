#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CASHIERS 3 
#define NUM_TRANSACTIONS 10 

pthread_mutex_t revenue_mutex; //Mutex宣告
int total_revenue = 0; 

void* cashier(void* arg) {
    //收銀機ID
    int id = *(int*)arg; 
    int revenue = 0;

    for (int i = 0; i < NUM_TRANSACTIONS; i++) {
        int transaction_amount = rand() % 100 + 1; //隨機交易金額
        revenue += transaction_amount;

        pthread_mutex_lock(&revenue_mutex); // 用Mutex保護總營業額
        total_revenue += transaction_amount;
        pthread_mutex_unlock(&revenue_mutex); // 解鎖

        printf("收銀機 %d 號 結了一單 $%d. 總營業額: $%d\n", id, transaction_amount, total_revenue);
        usleep(rand() % 500000); // 加一點延遲
    }

    printf("收銀機 %d 號 完成了全部交易，本機營業額 $%d.\n", id, revenue);
    return NULL;
}

int main() {
    pthread_t cashiers[NUM_CASHIERS];
    int cashier_ids[NUM_CASHIERS];

    // Mutex initialize
    pthread_mutex_init(&revenue_mutex, NULL);

    // 建立三台收銀機
    for (int i = 0; i < NUM_CASHIERS; i++) {
        cashier_ids[i] = i + 1; //給收銀機編號
        pthread_create(&cashiers[i], NULL, cashier, &cashier_ids[i]);
    }

    // 等所有thread完成
    for (int i = 0; i < NUM_CASHIERS; i++) {
        pthread_join(cashiers[i], NULL);
    }

    // 把mutex鎖變數消掉
    pthread_mutex_destroy(&revenue_mutex);

    printf("總營業額: $%d\n", total_revenue);
    return 0;
}
