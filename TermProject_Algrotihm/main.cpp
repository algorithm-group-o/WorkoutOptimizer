#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
void calculate_schedule(workOut* workout, int size, int available_time, float target_calories, int prefer_num) { ///추가로 구현해야 할것 칼로리에 비해 시간이 남은경우 시간 처리 어떻게 할건지
                                                                                                                /// 그다음에 추가로 구현해야할것 intensity고려해서 어떻게 처리할건지
    int max_sessions = available_time / 30; // 운동 가능한 최대 세션 수
    float* dp = (float*)malloc(sizeof(float) * (max_sessions + 1)); // DP 배열 선언

    for (int j = 0; j <= max_sessions; j++) {
        dp[j] = target_calories; // 초기화: 목표 칼로리로 시작
    }

    // prefer_num 운동 포함
    dp[1] = dp[0] - workout[prefer_num].calories;

    // DP 계산
    for (int i = 0; i < size; i++) {
        if (i == prefer_num) continue; // prefer_num 운동은 이미 포함했으므로 제외
        for (int j = max_sessions; j >1; j--) {
            if (dp[j - 1] >= workout[i].calories) { // 칼로리가 충분할 때만
                dp[j] = fmin(dp[j], dp[j - 1] - workout[i].calories);
            }
        }
    }

    // 목표에 가장 근접한 dp[j] 찾기
    float closest_calories = target_calories;
    for (int j = 1; j <= max_sessions; j++) {
        if (dp[j] > 0 && dp[j] < closest_calories) {
            closest_calories = dp[j];
        }
    }

    // 결과 출력
    if (closest_calories == target_calories) {
        printf("운동 세션으로 목표 칼로리를 달성할 수 없습니다.\n");
    }
    else {
        printf("소모한 칼로리: %.2f, 남은 칼로리: %.2f\n", target_calories - closest_calories, closest_calories);
    }

    free(dp); // 메모리 해제
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
