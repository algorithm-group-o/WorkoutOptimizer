#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TIME 600  // 최대 운동 시간(분 단위, 10시간)

// 구조체 정의
typedef struct {
    char name[20];
    float calories; // 30분당 소모 칼로리
    char intensity; // 운동 강도 (H, M, L)
} workOut;

// 파일에서 운동 정보 읽기
int count_file(char* fname) {
    FILE* file;
    workOut work;
    file = fopen(fname, "r");
    if (file == NULL) {
        printf("File cannot be opened\n");
        exit(1);
    }
    int count = 0;
    while (fscanf(file, "%s %f %c", work.name, &work.calories, &work.intensity) == 3) {
        count++;
    }
    fclose(file);
    return count;
}

void read_file(char* fname, workOut* workout) {
    FILE* file;
    file = fopen(fname, "r");
    if (file == NULL) {
        printf("File cannot be opened\n");
        exit(1);
    }
    int i = 0;
    while (fscanf(file, "%s %f %c", workout[i].name, &workout[i].calories, &workout[i].intensity) == 3) {
        i++;
    }
    fclose(file);
}

// DP를 이용해 최적 스케줄 계산
void calculate_schedule(workOut* workout, int size, int available_time, float target_calories, int prefer_num) {
    
}

int main() {
    char fname[20] = "exercise.txt";
    int size = count_file(fname);
    workOut* workout = (workOut*)malloc(sizeof(workOut) * size);
    read_file(fname, workout);

    int prefer_num;
    float target_calories;
    int available_time;

    printf("Choose your preferred exercise\nHere's our workout list:\n");
    for (int i = 0; i < size; i++) {
        printf("NO.%d Exercise Name: %s, Burned Calories per 30 min: %.2f, Intensity: %c\n",
            i + 1, workout[i].name, workout[i].calories, workout[i].intensity);
    }

    printf("\nChoose a number for your preferred exercise: ");
    scanf("%d", &prefer_num);
    prefer_num--;

    printf("\nEnter your target calories to burn: ");
    scanf("%f", &target_calories);

    printf("\nEnter available time (in minutes): ");
    scanf("%d", &available_time);

    if (available_time < 30) {
        printf("Insufficient time for any workout.\n");
    }
    else {
        calculate_schedule(workout, size, available_time, target_calories, prefer_num);
    }

    free(workout);
    return 0;
}
