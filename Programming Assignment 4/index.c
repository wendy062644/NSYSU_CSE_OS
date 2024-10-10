#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define request_num 1001
#define cylinders 5000
#define initial 2500
#define run_times 10

/*
    request_num 設定 request 數量，1001代表生成1000個隨機數及1個初始位置
    cylinders 設定 request 範圍
    initial 設定初始位置
    run_times 設定測試次數
*/

int requests[request_num+1] = {initial}; // 紀錄 request

int compare(const void *a, const void *b) { // c 的 qsort 排序
    return (*(int*)a - *(int*)b);
}

int FCFS() {
    int total = requests[0]; // 紀錄總距離
    for(int i = 1; i <= request_num; i++) { // 直接依順序跑
        total += abs(requests[i] - requests[i-1]);
    }
    return total;
}

int SSTF() {
    int had_used[request_num+1] = {0}; // 紀錄是否執行過該request
    int total = 0, last_position = 0, now_position;
    for(int i = 1; i <= request_num; i++) {
        now_position = -1;
        for(int j = 1; j <= request_num; j++) { // 從頭到尾尋找離last位置最近的request
            if(now_position == -1 || (abs(requests[j] - requests[last_position]) <= abs(requests[now_position] - requests[last_position]))) {
                if(had_used[j] == 0) // 找到了，且沒處理過該request
                    now_position = j;
            }
        }
        had_used[now_position] = 1; // 設定為已處理
        total += abs(requests[now_position] - requests[last_position]); // 計算移動距離
        last_position = now_position;
    }
    return total;
}

int SCAN() {
    qsort(requests, request_num, sizeof(int), compare); // 排序

    int total = 0, direction = 1; // 紀錄總距離與方向
    int current_head = initial;

    int start_index = 0;

    while (start_index < request_num && requests[start_index] < initial) { // 尋找初始位置的index
        start_index++;
    }

    if (direction == 1) { // 由內往外跑
        for (int i = start_index; i < request_num; i++) { // 依序處理request
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index-1 >= 0) // 因為會跑到底，因此最後一個request到最外圈的距離需算進去
            total += abs(cylinders - 1 - current_head);

        current_head = cylinders - 1;

        for (int i = start_index - 1; i >= 0; i--) { // 開始往內跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
        //total += abs(current_head - 0);
    } else { // 由外往內跑
        for (int i = start_index - 1; i >= 0; i--) { // 依序處理request
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
        if(start_index < request_num) // 跑到最內圈，需將距離算進去
            total += abs(current_head - 0);

        current_head = 0;

        for (int i = start_index; i < request_num; i++) { // 往外跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
        //total += abs(4999 - current_head);
    }

    return total;
}

int C_SCAN() {
    qsort(requests, request_num, sizeof(int), compare); // 排序

    int total = 0, direction = 1; // 紀錄總距離與方向
    int current_head = initial;

    int start_index = 0;

    while (start_index < request_num && requests[start_index] < initial) { // 尋找初始位置的index
        start_index++;
    }

    if (direction == 1) { // 由內往外跑
        for (int i = start_index; i < request_num; i++) {
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index > 0) // 計算最後一個request到最內圈的距離
            total += abs(current_head - 0);

        current_head = 0; // 跳到最內圈

        for (int i = 0; i < start_index; i++) { //由內往外跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    } else { // 由外往內跑
        for (int i = start_index - 1; i >= 0; i--) {
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(request_num - 1 >= start_index) // 計算最後一個request到最外圈的距離
            total += abs(cylinders - 1 - current_head);

        current_head = cylinders - 1; // 跳到最外圈

        for (int i = request_num - 1; i >= start_index; i--) { //由外往內跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int LOOK() {
    qsort(requests, request_num, sizeof(int), compare); // 排序

    int total = 0, direction = 1; // 紀錄總距離與方向
    int current_head = initial;

    int start_index = 0;

    while (start_index < request_num && requests[start_index] < initial) { // 尋找初始位置的index
        start_index++;
    }

    if (direction == 1) {
        for (int i = start_index; i < request_num; i++) { // 由內往外跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        for (int i = start_index - 1; i >= 0; i--) { // 再由外往內跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    } else { // 由外往內跑
        for (int i = start_index - 1; i >= 0; i--) { // 由外往內跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        for (int i = start_index; i < request_num; i++) { // 再由內往外跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int C_LOOK() {
    qsort(requests, request_num, sizeof(int), compare); // 排序

    int total = 0;
    int current_head = initial;

    int start_index = 0;

    while (start_index <= request_num && requests[start_index] < initial) { // 尋找初始位置的index
        start_index++;
    }

    for (int i = start_index; i < request_num; i++) { // 由內往外跑
        total += abs(requests[i] - current_head);
        current_head = requests[i];
    }

    if (start_index > 0) { // 如果裡面還有request
        total += abs(requests[0] - current_head); // 加上移動到最裡面的request的距離
        current_head = requests[0];

        for (int i = 0; i < start_index; i++) { // 由內往外跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int Optimal() {
    qsort(requests, request_num+1, sizeof(int), compare); // 排序

    int total = 0;
    int start_index = 0, current_head = initial;

    while (start_index <= request_num && requests[start_index] < initial) { // 尋找初始位置的index
        start_index++;
    }

    if(abs(requests[request_num] - initial) < abs(requests[0] - initial)) { // 如果最右邊的距離較近
        for (int i = start_index; i <= request_num; i++) { // 由內往外跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index - 1 >= 0)
            total += abs(requests[request_num] - initial); // 移動至出始位置的距離
        current_head = initial;

        for (int i = start_index - 1; i >= 0; i--) { //由外往內跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    } else {
        for (int i = start_index - 1; i >= 0; i--) { //由外往內跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index <= request_num)
            total += abs(requests[0] - initial); // 移動至出始位置的距離
        current_head = initial;

        for (int i = start_index; i <= request_num; i++) { // 由內往外跑
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int main() {
    srand(time(NULL));
    int fcfs = 0, sstf = 0, scan = 0, c_scan = 0, look = 0, c_look = 0, optimal = 0;
    for(int i = 0; i < run_times; i++){ // 測試 n 次
        requests[0] = initial;
        for(int i = 1; i <= request_num; i++) { // 隨機生成 request
            requests[i] = rand()%cylinders;
        }
        fcfs += FCFS();
        sstf += SSTF();
        scan += SCAN();
        c_scan += C_SCAN();
        look += LOOK();
        c_look += C_LOOK();
        optimal += Optimal();
    }
    printf("FCFS total head movement: %d, total time = %.2f s\n", fcfs/run_times, fcfs/100000.0/run_times);
    printf("SSTF total head movement: %d, total time = %.2f s\n", sstf/run_times, sstf/100000.0/run_times);
    printf("SCAN total head movement: %d, total time = %.2f s\n", scan/run_times, scan/100000.0/run_times);
    printf("C_SCAN total head movement: %d, total time = %.2f s\n", c_scan/run_times, c_scan/100000.0/run_times);
    printf("LOOK total head movement: %d, total time = %.2f s\n", look/run_times, look/100000.0/run_times);
    printf("C-LOOK total head movement: %d, total time = %.2f s\n", c_look/run_times, c_look/100000.0/run_times);
    printf("Optimal total head movement: %d, total time = %.2f s\n", optimal/run_times, optimal/100000.0/run_times);
    return 0;
}
