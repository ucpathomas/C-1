#ifndef FUNC_H
#define FUNC_H

/* ============================================================
 *  증강 요트 다이스 (Augment Yacht Dice)
 *  func.h : 공용 상수 / 구조체 / 함수 선언
 *
 *  - main.c 와 func.c 가 둘 다 #include "func.h" 해서 사용한다.
 *  - 포인터를 쓰지 않고, 배열 + 인덱스(me, enemy)로 플레이어를 구분한다.
 *    (배열은 함수에 넘기면 원본이 바뀌므로 p[me].total += 10; 처럼 쓸 수 있다.)
 * ============================================================ */

#define _CRT_SECURE_NO_WARNINGS   /* Visual Studio 의 scanf/strcpy 경고 끄기 */

#include <stdio.h>
#include <stdlib.h>   /* rand, srand, exit, system */
#include <time.h>     /* time (srand 시드용) */
#include <string.h>   /* strcpy */

/* ---------------- 기본 상수 ---------------- */
#define PLAYER_NUM   2    /* 플레이어 수 */
#define ROUND_MAX    6    /* 총 라운드 수 (12개 족보 중 6개만 채움) */
#define DICE_NUM     5    /* 주사위 개수 */
#define COMBO_NUM    12   /* 족보 개수 */
#define AUG_NUM      12   /* 전체 증강 개수 */
#define AUG_CHOICE   3    /* 라운드마다 보여줄 증강 카드 수 */
#define BASE_REROLL  2    /* 기본 재굴림 횟수 (총 3번 굴림) */

/* ---------------- 게임 모드 ---------------- */
#define MODE_PVP     1    /* 사람 vs 사람 */
#define MODE_AI      2    /* 사람 vs AI   */

/* ---------------- 족보 번호 (score[], used[] 의 인덱스) ---------------- */
#define COMBO_ACES        0
#define COMBO_TWOS        1
#define COMBO_THREES      2
#define COMBO_FOURS       3
#define COMBO_FIVES       4
#define COMBO_SIXES       5
#define COMBO_CHOICE      6
#define COMBO_FOUR_KIND   7
#define COMBO_FULL_HOUSE  8
#define COMBO_S_STRAIGHT  9
#define COMBO_L_STRAIGHT  10
#define COMBO_YACHT       11

/* ---------------- 증강 등급 ---------------- */
#define GRADE_NORMAL  1   /* *   일반 */
#define GRADE_RARE    2   /* **  희귀 */
#define GRADE_LEGEND  3   /* *** 전설 */

/* ---------------- 증강 효과 종류 (Augment.type) ---------------- */
#define AUG_REROLL_PLUS   1   /* 한 번 더!    : 재굴림 +value            */
#define AUG_SET_DIE       2   /* 행운의 1     : 주사위 1개를 원하는 눈으로 */
#define AUG_EVEN_BLESS    3   /* 짝수의 축복  : 홀수 눈 +1 (6은 그대로)   */
#define AUG_SCORE_PLUS    4   /* 보너스 머니  : 점수 +value              */
#define AUG_SCORE_MULT    5   /* 더블 업      : 점수 x value             */
#define AUG_ALL_IN        6   /* 올 인        : 재굴림 1회, 점수 x value  */
#define AUG_COPY_DIE      7   /* 주사위 복제  : 한 주사위 눈을 다른 곳에 복사 */
#define AUG_SABOTAGE      8   /* 방해 공작    : 상대 재굴림 -value        */
#define AUG_GAMBLER       9   /* 도박사       : 동전 던지기 x value 또는 0 */
#define AUG_YACHT_GUARD  10   /* 요트 확정권  : 5개를 같은 눈으로          */
#define AUG_STEAL        11   /* 점수 강탈    : 상대 총점의 value% 가져옴  */
#define AUG_CHAOS        12   /* 카오스       : 전부 재굴림 후 점수 x value */

/* ============================================================
 *  구조체
 * ============================================================ */

/* 증강 1개 */
typedef struct {
    int  id;            /* 증강 번호 (0 ~ AUG_NUM-1) */
    char name[30];      /* 증강 이름 */
    char desc[100];     /* 효과 설명 */
    int  grade;         /* GRADE_NORMAL / GRADE_RARE / GRADE_LEGEND */
    int  type;          /* 효과 종류 (AUG_...) */
    int  value;         /* 효과 수치 */
} Augment;

/* 플레이어 1명 */
typedef struct {
    char    name[20];
    int     is_ai;                  /* 0 = 사람, 1 = AI */
    int     score[COMBO_NUM];       /* 족보별 점수 */
    int     used[COMBO_NUM];        /* 족보 사용 여부 (0 = 미사용, 1 = 사용) */
    int     bonus;                  /* 점수 강탈로 생긴 점수 증감 */
    int     total;                  /* 총점 = score[] 합계 + bonus */
    int     reroll_max;             /* 다음(또는 이번) 턴 재굴림 횟수 */
    int     score_mult;             /* 이번 턴 점수 배수 (기본 1) */
    int     score_plus;             /* 이번 턴 추가 점수 (기본 0) */
    Augment my_aug[ROUND_MAX];      /* 지금까지 고른 증강 기록 */
    int     aug_count;              /* 보유 증강 수 */
} Player;

/* ============================================================
 *  전역 데이터 (정의는 func.c 에 있음)
 * ============================================================ */
extern char combo_name[COMBO_NUM][20];   /* 족보 이름 문자열 */

/* ============================================================
 *  함수 선언
 *  main.c 에서 선언하는 변수 :
 *    Player  p[PLAYER_NUM];
 *    Augment aug_list[AUG_NUM];
 *  주사위 dice[], 고정 여부 keep[] 는 play_turn() 안에서 선언한다.
 * ============================================================ */

/* ---------- ① 시작/설정 블록 ---------- */
void show_title(void);                                    /* 타이틀·간단 규칙 출력 */
int  select_mode(void);                                   /* 모드 선택 → MODE_PVP / MODE_AI 반환 */
void init_game(Player p[], Augment aug_list[], int mode); /* srand, 플레이어 초기화, 증강 목록 채우기 */
void init_player(Player p[], int idx, char name[], int is_ai); /* p[idx] 한 명 초기화 */

/* ---------- ② 턴 진행 블록 ---------- */
/* 한 플레이어의 한 라운드 진행 (me = 내 번호, enemy = 상대 번호)
 * 흐름: 증강 카드 선택 → roll_dice → apply_augment → 재굴림 반복 → 족보 선택 */
void play_turn(Player p[], int me, int enemy, Augment aug_list[], int round);
void roll_dice(int dice[]);                               /* 주사위 5개 전부 굴리기 */
void select_keep(int keep[]);                             /* 사람이 고정할 주사위 선택 (keep[i]=1 이면 고정) */
void reroll(int dice[], int keep[]);                      /* keep[i]==0 인 주사위만 다시 굴리기 */

/* ---------- ③ 점수 블록 ---------- */
int  check_combo(int dice[], int combo);                  /* 해당 족보 조건 충족이면 1, 아니면 0 */
int  calc_score(int dice[], int combo);                   /* 해당 족보의 기본 점수 (미충족이면 0) */
int  select_category(Player p[], int me, int dice[]);     /* 사람이 족보 선택 → 족보 번호 반환 */
int  apply_bonus(Player p[], int me, int base_score);     /* score_mult, score_plus 적용한 최종 점수 */
void update_total(Player p[], int me);                    /* score[] 합 + bonus 로 total 갱신 */

/* ---------- ④ 증강 블록 ---------- */
void offer_augment(Augment aug_list[], int choice[]);     /* choice[0~2] 에 후보 증강 번호 3개 뽑기 */
int  pick_augment(int choice[]);                          /* 사람이 1~3 입력 → 고른 증강 번호 반환 */
void apply_augment(Player p[], int me, int enemy, Augment aug_list[], int aug_idx, int dice[]);
                                                          /* 증강 효과 적용 (type 에 따라 분기) */
void attack_enemy(Player p[], int me, int enemy, Augment a); /* 방해 공작 / 점수 강탈 */
int  random_grade(void);                                  /* 확률로 등급 결정 (전설은 낮은 확률) */

/* ---------- ⑤ AI 블록 ---------- */
void ai_select_keep(int dice[], int keep[]);              /* 가장 많이 나온 눈을 남김 */
int  ai_select_category(Player p[], int me, int dice[]);  /* 보정 점수가 가장 높은 미사용 족보 */
int  ai_pick_augment(Augment aug_list[], int choice[]);   /* 등급이 가장 높은 카드 선택 */

/* ---------- ⑥ 출력 블록 ---------- */
void print_dice(int dice[], int keep[]);                  /* 아스키 주사위 5개 출력 (고정은 * 표시) */
void print_scoreboard(Player p[]);                        /* 두 플레이어 점수판 출력 */
void print_augment_cards(Augment aug_list[], int choice[]); /* 증강 카드 3장 출력 */

/* ---------- ⑦ 종료 블록 ---------- */
int  check_end(int round);                                /* round > ROUND_MAX 이면 1, 아니면 0 */
void show_result(Player p[]);                             /* 최종 점수와 승자 출력 */

/* ---------- 보조 함수 ---------- */
int  read_int(int min, int max);                          /* min~max 숫자를 입력받을 때까지 반복 */
void wait_enter(void);                                    /* 엔터 입력 대기 */
void set_augment(Augment aug_list[], int i, char name[], char desc[],
                 int grade, int type, int value);         /* 증강 1개 채우기 */
int  most_common(int dice[]);                             /* 가장 많이 나온 눈 (동률이면 큰 눈) */

#endif /* FUNC_H */
