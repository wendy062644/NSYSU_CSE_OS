#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define request_num 1001
#define cylinders 5000
#define initial 2500
#define run_times 10

/*
    request_num �]�w request �ƶq�A1001�N��ͦ�1000���H���Ƥ�1�Ӫ�l��m
    cylinders �]�w request �d��
    initial �]�w��l��m
    run_times �]�w���զ���
*/

int requests[request_num+1] = {initial}; // ���� request

int compare(const void *a, const void *b) { // c �� qsort �Ƨ�
    return (*(int*)a - *(int*)b);
}

int FCFS() {
    int total = requests[0]; // �����`�Z��
    for(int i = 1; i <= request_num; i++) { // �����̶��Ƕ]
        total += abs(requests[i] - requests[i-1]);
    }
    return total;
}

int SSTF() {
    int had_used[request_num+1] = {0}; // �����O�_����L��request
    int total = 0, last_position = 0, now_position;
    for(int i = 1; i <= request_num; i++) {
        now_position = -1;
        for(int j = 1; j <= request_num; j++) { // �q�Y����M����last��m�̪�request
            if(now_position == -1 || (abs(requests[j] - requests[last_position]) <= abs(requests[now_position] - requests[last_position]))) {
                if(had_used[j] == 0) // ���F�A�B�S�B�z�L��request
                    now_position = j;
            }
        }
        had_used[now_position] = 1; // �]�w���w�B�z
        total += abs(requests[now_position] - requests[last_position]); // �p�Ⲿ�ʶZ��
        last_position = now_position;
    }
    return total;
}

int SCAN() {
    qsort(requests, request_num, sizeof(int), compare); // �Ƨ�

    int total = 0, direction = 1; // �����`�Z���P��V
    int current_head = initial;

    int start_index = 0;

    while (start_index < request_num && requests[start_index] < initial) { // �M���l��m��index
        start_index++;
    }

    if (direction == 1) { // �Ѥ����~�]
        for (int i = start_index; i < request_num; i++) { // �̧ǳB�zrequest
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index-1 >= 0) // �]���|�]�쩳�A�]���̫�@��request��̥~�骺�Z���ݺ�i�h
            total += abs(cylinders - 1 - current_head);

        current_head = cylinders - 1;

        for (int i = start_index - 1; i >= 0; i--) { // �}�l�����]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
        //total += abs(current_head - 0);
    } else { // �ѥ~�����]
        for (int i = start_index - 1; i >= 0; i--) { // �̧ǳB�zrequest
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
        if(start_index < request_num) // �]��̤���A�ݱN�Z����i�h
            total += abs(current_head - 0);

        current_head = 0;

        for (int i = start_index; i < request_num; i++) { // ���~�]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
        //total += abs(4999 - current_head);
    }

    return total;
}

int C_SCAN() {
    qsort(requests, request_num, sizeof(int), compare); // �Ƨ�

    int total = 0, direction = 1; // �����`�Z���P��V
    int current_head = initial;

    int start_index = 0;

    while (start_index < request_num && requests[start_index] < initial) { // �M���l��m��index
        start_index++;
    }

    if (direction == 1) { // �Ѥ����~�]
        for (int i = start_index; i < request_num; i++) {
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index > 0) // �p��̫�@��request��̤��骺�Z��
            total += abs(current_head - 0);

        current_head = 0; // ����̤���

        for (int i = 0; i < start_index; i++) { //�Ѥ����~�]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    } else { // �ѥ~�����]
        for (int i = start_index - 1; i >= 0; i--) {
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(request_num - 1 >= start_index) // �p��̫�@��request��̥~�骺�Z��
            total += abs(cylinders - 1 - current_head);

        current_head = cylinders - 1; // ����̥~��

        for (int i = request_num - 1; i >= start_index; i--) { //�ѥ~�����]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int LOOK() {
    qsort(requests, request_num, sizeof(int), compare); // �Ƨ�

    int total = 0, direction = 1; // �����`�Z���P��V
    int current_head = initial;

    int start_index = 0;

    while (start_index < request_num && requests[start_index] < initial) { // �M���l��m��index
        start_index++;
    }

    if (direction == 1) {
        for (int i = start_index; i < request_num; i++) { // �Ѥ����~�]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        for (int i = start_index - 1; i >= 0; i--) { // �A�ѥ~�����]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    } else { // �ѥ~�����]
        for (int i = start_index - 1; i >= 0; i--) { // �ѥ~�����]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        for (int i = start_index; i < request_num; i++) { // �A�Ѥ����~�]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int C_LOOK() {
    qsort(requests, request_num, sizeof(int), compare); // �Ƨ�

    int total = 0;
    int current_head = initial;

    int start_index = 0;

    while (start_index <= request_num && requests[start_index] < initial) { // �M���l��m��index
        start_index++;
    }

    for (int i = start_index; i < request_num; i++) { // �Ѥ����~�]
        total += abs(requests[i] - current_head);
        current_head = requests[i];
    }

    if (start_index > 0) { // �p�G�̭��٦�request
        total += abs(requests[0] - current_head); // �[�W���ʨ�̸̭���request���Z��
        current_head = requests[0];

        for (int i = 0; i < start_index; i++) { // �Ѥ����~�]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int Optimal() {
    qsort(requests, request_num+1, sizeof(int), compare); // �Ƨ�

    int total = 0;
    int start_index = 0, current_head = initial;

    while (start_index <= request_num && requests[start_index] < initial) { // �M���l��m��index
        start_index++;
    }

    if(abs(requests[request_num] - initial) < abs(requests[0] - initial)) { // �p�G�̥k�䪺�Z������
        for (int i = start_index; i <= request_num; i++) { // �Ѥ����~�]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index - 1 >= 0)
            total += abs(requests[request_num] - initial); // ���ʦܥX�l��m���Z��
        current_head = initial;

        for (int i = start_index - 1; i >= 0; i--) { //�ѥ~�����]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    } else {
        for (int i = start_index - 1; i >= 0; i--) { //�ѥ~�����]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }

        if(start_index <= request_num)
            total += abs(requests[0] - initial); // ���ʦܥX�l��m���Z��
        current_head = initial;

        for (int i = start_index; i <= request_num; i++) { // �Ѥ����~�]
            total += abs(requests[i] - current_head);
            current_head = requests[i];
        }
    }

    return total;
}

int main() {
    srand(time(NULL));
    int fcfs = 0, sstf = 0, scan = 0, c_scan = 0, look = 0, c_look = 0, optimal = 0;
    for(int i = 0; i < run_times; i++){ // ���� n ��
        requests[0] = initial;
        for(int i = 1; i <= request_num; i++) { // �H���ͦ� request
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
