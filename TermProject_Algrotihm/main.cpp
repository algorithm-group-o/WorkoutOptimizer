#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TIME 600  // 최대 운동 시간 (분 단위, 10시간)

// 구조체 정의
typedef struct {
    char name[20];     // 운동 이름
    float calories;    // 30분당 소모 칼로리
    char intensity;    // 운동 강도 (H: High, M: Medium, L: Low)
} workOut;

// 파일에서 운동 정보의 개수를 세는 함수
int count_file(char* fname) {
    FILE* file = fopen(fname, "r");
    if (file == NULL) {
        printf("File cannot be opened\n");
        exit(1);
    }
    workOut temp;
    int count = 0;
    // 각 줄의 데이터를 읽으며 카운트 증가
    while (fscanf(file, "%s %f %c", temp.name, &temp.calories, &temp.intensity) == 3) {
        count++;
    }
    fclose(file);
    return count;
}

// 파일에서 운동 데이터를 읽어와 구조체 배열에 저장
void read_file(char* fname, workOut* workout) {
    FILE* file = fopen(fname, "r");
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

// DP를 이용해 최적의 운동 스케줄 계산
void calculate_schedule(workOut* workout, int size, int available_time, float target_calories, int prefer_num) {
    int max_sessions = available_time / 30; // 가능한 최대 세션 수
    float* dp = (float*)malloc(sizeof(float) * (max_sessions + 1)); // DP 배열 선언
    int* used = (int*)calloc(size, sizeof(int)); // 운동 사용 여부 기록 (0: 미사용, 1: 사용)

    // DP 배열 초기화
    for (int j = 0; j <= max_sessions; j++) {
        dp[j] = target_calories; // 목표 칼로리로 초기화
    }

    char last_intensity = ' '; // 직전 운동의 강도
    dp[1] = dp[0] - workout[prefer_num].calories; // 선호 운동 먼저 수행
    available_time -= 30; // 남은 시간 업데이트
    used[prefer_num] = 1; // 선호 운동 사용 기록
    last_intensity = workout[prefer_num].intensity; // 강도 기록

    printf("Selected Exercise: %s (Calories: %.2f, Intensity: %c, Remaining Calories: %.2f)\n",
        workout[prefer_num].name, workout[prefer_num].calories, workout[prefer_num].intensity, dp[1]);

    // DP 계산 반복
    for (int session = 2; session <= max_sessions && available_time >= 30; session++) {
        float min_remaining_calories = dp[session - 1]; // 이전 세션의 남은 칼로리
        int best_exercise = -1;

        // 가능한 운동 탐색
        for (int i = 0; i < size; i++) {
            if (used[i]) continue; // 이미 수행한 운동 제외
            if (workout[i].intensity == 'h' && last_intensity == 'h') continue; // 연속 고강도 운동 방지
            if (dp[session - 1] >= workout[i].calories) { // 남은 칼로리로 운동 가능 여부 확인
                float remaining_calories = dp[session - 1] - workout[i].calories;
                if (best_exercise == -1 || remaining_calories < min_remaining_calories) {
                    best_exercise = i; // 최적의 운동 선택
                    min_remaining_calories = remaining_calories;
                }
            }
        }

        if (best_exercise == -1) break; // 선택 가능한 운동이 없으면 종료

        // 선택한 운동을 수행
        dp[session] = min_remaining_calories;
        available_time -= 30; // 남은 시간 업데이트
        used[best_exercise] = 1; // 사용한 운동 기록
        last_intensity = workout[best_exercise].intensity; // 강도 갱신

        printf("Selected Exercise: %s (Calories: %.2f, Intensity: %c, Remaining Calories: %.2f)\n",
            workout[best_exercise].name, workout[best_exercise].calories, workout[best_exercise].intensity, dp[session]);
    }

    // 결과 출력
 
    
      printf("소모한 칼로리: %.2f, 남은 칼로리: %.2f\n", target_calories - dp[max_sessions], dp[max_sessions]);

    free(dp);
    free(used);
}

int main() {
    char fname[20] = "exercise.txt";
    int size = count_file(fname); // 운동 데이터 개수 읽기
    workOut* workout = (workOut*)malloc(sizeof(workOut) * size); // 운동 배열 동적 할당
    read_file(fname, workout); // 운동 데이터 읽기

    int prefer_num;
    float target_calories;
    int available_time;

    // 운동 리스트 출력
    printf("Choose your preferred exercise\nHere's our workout list:\n");
    for (int i = 0; i < size; i++) {
        printf("NO.%d Exercise Name: %s, Burned Calories per 30 min: %.2f, Intensity: %c\n",
            i + 1, workout[i].name, workout[i].calories, workout[i].intensity);
    }

    // 사용자 입력 받기
    printf("\nChoose a number for your preferred exercise: ");
    scanf("%d", &prefer_num);
    prefer_num--; // 배열 인덱스 맞추기

    printf("\nEnter your target calories to burn: ");
    scanf("%f", &target_calories);

    printf("\nEnter available time (in minutes): ");
    scanf("%d", &available_time);

    // 입력 유효성 확인
    if (available_time < 30) {
        printf("Insufficient time for any workout.\n");
    }
    else {
        calculate_schedule(workout, size, available_time, target_calories, prefer_num);
    }

    free(workout); // 동적 메모리 해제
    return 0;
}
