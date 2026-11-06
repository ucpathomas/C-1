#include "func.h"

/* ============================================================
 *  전역 데이터
 * ============================================================ */
char combo_name[COMBO_NUM][20] = {
    "Aces", "Twos", "Threes", "Fours", "Fives", "Sixes",
    "Choice", "4 of a Kind", "Full House",
    "S. Straight", "L. Straight", "Yacht"
};

/* ============================================================
 *  보조 함수
 * ============================================================ */

/* min ~ max 사이 숫자를 입력받을 때까지 반복 */
int read_int(int min, int max)
{
    char line[100];
    int n;

    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n입력이 끝나서 게임을 종료합니다.\n");
            exit(0);
        }
        if (sscanf(line, "%d", &n) == 1 && n >= min && n <= max) {
            return n;
        }
        printf("%d ~ %d 사이의 숫자를 입력하세요 >> ", min, max);
    }
}

/* 엔터를 누를 때까지 대기 */
void wait_enter(void)
{
    char line[100];

    printf("\n[Enter]를 누르면 계속합니다...");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return;
    }
}

/* 증강 1개의 정보를 채운다 */
void set_augment(Augment aug_list[], int i, char name[], char desc[],
                 int grade, int type, int value)
{
    aug_list[i].id = i;
    strcpy(aug_list[i].name, name);
    strcpy(aug_list[i].desc, desc);
    aug_list[i].grade = grade;
    aug_list[i].type = type;
    aug_list[i].value = value;
}

/* 가장 많이 나온 눈을 반환 (동률이면 큰 눈) */
int most_common(int dice[])
{
    int cnt[7] = {0};
    int i, best = 1;

    for (i = 0; i < DICE_NUM; i++) {
        cnt[dice[i]]++;
    }
    for (i = 1; i <= 6; i++) {
        if (cnt[i] >= cnt[best]) {
            best = i;
        }
    }
    return best;
}

/* ============================================================
 *  ① 시작/설정 블록
 * ============================================================ */

void show_title(void)
{
#ifdef _WIN32
    system("chcp 65001 > nul");   /* 윈도우 콘솔에서 한글이 깨지지 않게 (맨 처음에 실행) */
#endif
    printf("\n");
    printf("  ==================================================\n");
    printf("    Y A C H T   D I C E   :   A U G M E N T\n");
    printf("               증 강  요 트  다 이 스\n");
    printf("  ==================================================\n");
    printf("   - 주사위 5개를 굴려 족보를 만들고 점수를 겨룹니다.\n");
    printf("   - 총 %d라운드, 12개 족보 중 %d개만 채웁니다.\n", ROUND_MAX, ROUND_MAX);
    printf("   - 매 라운드 증강 카드 3장 중 1장을 골라 판을 뒤집으세요!\n");
    printf("  ==================================================\n\n");
}

int select_mode(void)
{
    printf(" 게임 모드를 선택하세요\n");
    printf("   1. 사람 vs 사람 (한 컴퓨터에서 번갈아 플레이)\n");
    printf("   2. 사람 vs AI\n");
    printf("선택 >> ");
    return read_int(MODE_PVP, MODE_AI);
}

void init_game(Player p[], Augment aug_list[], int mode)
{
    srand((unsigned int)time(NULL));

    /* 플레이어 설정 */
    if (mode == MODE_AI) {
        init_player(p, 0, "PLAYER", 0);
        init_player(p, 1, "AI", 1);
    } else {
        init_player(p, 0, "PLAYER 1", 0);
        init_player(p, 1, "PLAYER 2", 0);
    }

    /* 증강 목록 (일반 4개 / 희귀 5개 / 전설 3개) */
    set_augment(aug_list, 0, "한 번 더!", "이번 턴 재굴림 +1",
                GRADE_NORMAL, AUG_REROLL_PLUS, 1);
    set_augment(aug_list, 1, "행운의 1", "주사위 1개를 원하는 눈으로 변경",
                GRADE_NORMAL, AUG_SET_DIE, 0);
    set_augment(aug_list, 2, "짝수의 축복", "홀수 눈 주사위 전부 +1 (6은 그대로)",
                GRADE_NORMAL, AUG_EVEN_BLESS, 0);
    set_augment(aug_list, 3, "보너스 머니", "이번 턴 점수 +10",
                GRADE_NORMAL, AUG_SCORE_PLUS, 10);
    set_augment(aug_list, 4, "더블 업", "이번 턴 점수 x2",
                GRADE_RARE, AUG_SCORE_MULT, 2);
    set_augment(aug_list, 5, "올 인", "재굴림 1회만 가능, 대신 점수 x3",
                GRADE_RARE, AUG_ALL_IN, 3);
    set_augment(aug_list, 6, "주사위 복제", "주사위 1개의 눈을 다른 주사위에 복사",
                GRADE_RARE, AUG_COPY_DIE, 0);
    set_augment(aug_list, 7, "방해 공작", "상대의 다음 턴 재굴림 -1",
                GRADE_RARE, AUG_SABOTAGE, 1);
    set_augment(aug_list, 8, "도박사", "동전 던지기: 앞면이면 점수 x2, 뒷면이면 점수 0",
                GRADE_RARE, AUG_GAMBLER, 2);
    set_augment(aug_list, 9, "요트 확정권", "주사위 5개를 모두 같은 눈으로 (눈은 선택)",
                GRADE_LEGEND, AUG_YACHT_GUARD, 0);
    set_augment(aug_list, 10, "점수 강탈", "상대 총점의 10%를 내 점수로 가져옴",
                GRADE_LEGEND, AUG_STEAL, 10);
    set_augment(aug_list, 11, "카오스", "주사위 5개 전부 재굴림 후 점수 x2",
                GRADE_LEGEND, AUG_CHAOS, 2);
}

void init_player(Player p[], int idx, char name[], int is_ai)
{
    int i;

    strcpy(p[idx].name, name);
    p[idx].is_ai = is_ai;
    for (i = 0; i < COMBO_NUM; i++) {
        p[idx].score[i] = 0;
        p[idx].used[i] = 0;
    }
    p[idx].bonus = 0;
    p[idx].total = 0;
    p[idx].reroll_max = BASE_REROLL;
    p[idx].score_mult = 1;
    p[idx].score_plus = 0;
    p[idx].aug_count = 0;
}

/* ============================================================
 *  ② 턴 진행 블록
 * ============================================================ */

void play_turn(Player p[], int me, int enemy, Augment aug_list[], int round)
{
    int dice[DICE_NUM];
    int keep[DICE_NUM];
    int choice[AUG_CHOICE];
    int aug_idx, combo, base, final_score;
    int i, left, kept;

    printf("\n\n");
    printf("##################################################\n");
    printf("   라운드 %d / %d   -   %s 의 차례\n", round, ROUND_MAX, p[me].name);
    printf("##################################################\n");

    /* 1. 증강 선택 */
    offer_augment(aug_list, choice);
    print_augment_cards(aug_list, choice);
    if (p[me].is_ai == 1) {
        aug_idx = ai_pick_augment(aug_list, choice);
    } else {
        printf("선택 (1~%d) >> ", AUG_CHOICE);
        aug_idx = pick_augment(choice);
    }

    /* 2. 주사위 굴리기 */
    for (i = 0; i < DICE_NUM; i++) {
        keep[i] = 0;
    }
    roll_dice(dice);
    printf("\n [첫 번째 굴림]\n");
    print_dice(dice, keep);

    /* 3. 증강 적용 (주사위를 바꾸는 증강이 있어서 굴린 뒤에 적용) */
    apply_augment(p, me, enemy, aug_list, aug_idx, dice);
    printf("\n [증강 적용 후 주사위]\n");
    print_dice(dice, keep);

    /* 4. 재굴림 반복 */
    left = p[me].reroll_max;
    while (left > 0) {
        printf("\n 남은 재굴림 : %d회\n", left);
        if (p[me].is_ai == 1) {
            ai_select_keep(dice, keep);
        } else {
            select_keep(keep);
        }

        kept = 0;
        for (i = 0; i < DICE_NUM; i++) {
            if (keep[i] == 1) {
                kept++;
            }
        }
        if (kept == DICE_NUM) {
            printf(" 전부 고정했으므로 재굴림을 마칩니다.\n");
            break;
        }

        reroll(dice, keep);
        left--;
        printf("\n [재굴림 결과]\n");
        print_dice(dice, keep);
    }

    /* 5. 족보 선택과 점수 기록 */
    if (p[me].is_ai == 1) {
        combo = ai_select_category(p, me, dice);
    } else {
        combo = select_category(p, me, dice);
    }
    base = calc_score(dice, combo);
    final_score = apply_bonus(p, me, base);
    p[me].score[combo] = final_score;
    p[me].used[combo] = 1;
    update_total(p, me);

    printf("\n >> %s : [%s] 족보에 기록!  기본 %d점 -> 증강 보정 후 %d점\n",
           p[me].name, combo_name[combo], base, final_score);

    /* 6. 턴 종료: 이번 턴 효과 초기화 */
    p[me].reroll_max = BASE_REROLL;
    p[me].score_mult = 1;
    p[me].score_plus = 0;

    print_scoreboard(p);
    wait_enter();
}

void roll_dice(int dice[])
{
    int i;

    for (i = 0; i < DICE_NUM; i++) {
        dice[i] = rand() % 6 + 1;
    }
}

void select_keep(int keep[])
{
    char line[100];
    int i;

    for (i = 0; i < DICE_NUM; i++) {
        keep[i] = 0;
    }

    printf(" 고정할 주사위 번호를 붙여서 입력 (예: 134)\n");
    printf(" 없으면 0, 전부 고정하면 12345 (재굴림 종료) >> ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("\n입력이 끝나서 게임을 종료합니다.\n");
        exit(0);
    }
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] >= '1' && line[i] <= '5') {
            keep[line[i] - '1'] = 1;
        }
    }
}

void reroll(int dice[], int keep[])
{
    int i;

    for (i = 0; i < DICE_NUM; i++) {
        if (keep[i] == 0) {
            dice[i] = rand() % 6 + 1;
        }
    }
}

/* ============================================================
 *  ③ 점수 블록
 * ============================================================ */

/* 족보 조건을 만족하면 1 */
int check_combo(int dice[], int combo)
{
    int cnt[7] = {0};
    int i, has3 = 0, has2 = 0, same4 = 0, same5 = 0;

    for (i = 0; i < DICE_NUM; i++) {
        cnt[dice[i]]++;
    }
    for (i = 1; i <= 6; i++) {
        if (cnt[i] == 3) has3 = 1;
        if (cnt[i] == 2) has2 = 1;
        if (cnt[i] >= 4) same4 = 1;
        if (cnt[i] == 5) same5 = 1;
    }

    if (combo <= COMBO_CHOICE) {
        return 1;   /* Aces ~ Choice 는 항상 기록 가능 */
    }
    if (combo == COMBO_FOUR_KIND) {
        return same4;
    }
    if (combo == COMBO_FULL_HOUSE) {
        return has3 && has2;
    }
    if (combo == COMBO_S_STRAIGHT) {
        if (cnt[1] && cnt[2] && cnt[3] && cnt[4]) return 1;
        if (cnt[2] && cnt[3] && cnt[4] && cnt[5]) return 1;
        if (cnt[3] && cnt[4] && cnt[5] && cnt[6]) return 1;
        return 0;
    }
    if (combo == COMBO_L_STRAIGHT) {
        if (cnt[1] && cnt[2] && cnt[3] && cnt[4] && cnt[5]) return 1;
        if (cnt[2] && cnt[3] && cnt[4] && cnt[5] && cnt[6]) return 1;
        return 0;
    }
    if (combo == COMBO_YACHT) {
        return same5;
    }
    return 0;
}

/* 족보별 기본 점수 */
int calc_score(int dice[], int combo)
{
    int cnt[7] = {0};
    int i, sum = 0;

    for (i = 0; i < DICE_NUM; i++) {
        cnt[dice[i]]++;
        sum += dice[i];
    }

    if (combo >= COMBO_ACES && combo <= COMBO_SIXES) {
        return cnt[combo + 1] * (combo + 1);   /* 해당 눈 개수 x 눈 */
    }
    if (combo == COMBO_CHOICE) {
        return sum;
    }
    if (check_combo(dice, combo) == 0) {
        return 0;
    }
    if (combo == COMBO_FOUR_KIND || combo == COMBO_FULL_HOUSE) {
        return sum;
    }
    if (combo == COMBO_S_STRAIGHT) {
        return 15;
    }
    if (combo == COMBO_L_STRAIGHT) {
        return 30;
    }
    if (combo == COMBO_YACHT) {
        return 50;
    }
    return 0;
}

int select_category(Player p[], int me, int dice[])
{
    int i, n, base;

    printf("\n 기록할 족보를 선택하세요 (증강 보정 포함 예상 점수)\n");
    for (i = 0; i < COMBO_NUM; i++) {
        if (p[me].used[i] == 0) {
            base = calc_score(dice, i);
            printf("   %2d. %-12s : %3d점\n", i + 1, combo_name[i],
                   apply_bonus(p, me, base));
        }
    }
    printf("선택 >> ");

    while (1) {
        n = read_int(1, COMBO_NUM);
        if (p[me].used[n - 1] == 0) {
            return n - 1;
        }
        printf("이미 사용한 족보입니다. 다시 선택하세요 >> ");
    }
}

int apply_bonus(Player p[], int me, int base_score)
{
    return base_score * p[me].score_mult + p[me].score_plus;
}

void update_total(Player p[], int me)
{
    int i, sum = 0;

    for (i = 0; i < COMBO_NUM; i++) {
        sum += p[me].score[i];
    }
    p[me].total = sum + p[me].bonus;
}

/* ============================================================
 *  ④ 증강 블록
 * ============================================================ */

/* 등급 확률: 일반 55% / 희귀 35% / 전설 10% */
int random_grade(void)
{
    int r = rand() % 100;

    if (r < 55) {
        return GRADE_NORMAL;
    }
    if (r < 90) {
        return GRADE_RARE;
    }
    return GRADE_LEGEND;
}

/* 서로 다른 증강 3개를 뽑아 choice[] 에 저장 */
void offer_augment(Augment aug_list[], int choice[])
{
    int slot, i, idx, grade, dup;

    for (slot = 0; slot < AUG_CHOICE; slot++) {
        grade = random_grade();
        while (1) {
            idx = rand() % AUG_NUM;
            if (aug_list[idx].grade != grade) {
                continue;
            }
            dup = 0;
            for (i = 0; i < slot; i++) {
                if (choice[i] == idx) {
                    dup = 1;
                }
            }
            if (dup == 0) {
                break;
            }
        }
        choice[slot] = idx;
    }
}

int pick_augment(int choice[])
{
    int n = read_int(1, AUG_CHOICE);

    return choice[n - 1];
}

void apply_augment(Player p[], int me, int enemy, Augment aug_list[], int aug_idx, int dice[])
{
    Augment a = aug_list[aug_idx];
    int t = a.type;
    int i, v, src, dst, low;

    /* 보유 증강 기록 */
    if (p[me].aug_count < ROUND_MAX) {
        p[me].my_aug[p[me].aug_count] = a;
        p[me].aug_count++;
    }

    printf("\n >>> 증강 발동 : %s  (%s)\n", a.name, a.desc);

    /* 가장 낮은 눈의 주사위 위치 (AI가 사용) */
    low = 0;
    for (i = 1; i < DICE_NUM; i++) {
        if (dice[i] < dice[low]) {
            low = i;
        }
    }

    if (t == AUG_REROLL_PLUS) {
        p[me].reroll_max += a.value;
        printf("     재굴림이 %d회가 되었습니다!\n", p[me].reroll_max);
    }
    else if (t == AUG_SET_DIE) {
        if (p[me].is_ai == 1) {
            dst = low;
            v = most_common(dice);
        } else {
            printf("     바꿀 주사위 번호 (1~%d) >> ", DICE_NUM);
            dst = read_int(1, DICE_NUM) - 1;
            printf("     바꿀 눈 (1~6) >> ");
            v = read_int(1, 6);
        }
        dice[dst] = v;
        printf("     %d번 주사위가 %d(으)로 바뀌었습니다!\n", dst + 1, v);
    }
    else if (t == AUG_EVEN_BLESS) {
        for (i = 0; i < DICE_NUM; i++) {
            if (dice[i] % 2 == 1) {
                dice[i] = dice[i] + 1;   /* 1->2, 3->4, 5->6 */
            }
        }
        printf("     홀수 눈이 모두 +1 되었습니다!\n");
    }
    else if (t == AUG_SCORE_PLUS) {
        p[me].score_plus += a.value;
        printf("     이번 턴 점수에 +%d점이 붙습니다!\n", a.value);
    }
    else if (t == AUG_SCORE_MULT) {
        p[me].score_mult *= a.value;
        printf("     이번 턴 점수가 x%d 됩니다!\n", a.value);
    }
    else if (t == AUG_ALL_IN) {
        if (p[me].reroll_max > 1) {
            p[me].reroll_max = 1;
        }
        p[me].score_mult *= a.value;
        printf("     재굴림은 최대 1회, 대신 점수 x%d 입니다!\n", a.value);
    }
    else if (t == AUG_COPY_DIE) {
        if (p[me].is_ai == 1) {
            v = most_common(dice);
            src = 0;
            for (i = 0; i < DICE_NUM; i++) {
                if (dice[i] == v) {
                    src = i;
                    break;
                }
            }
            dst = low;
        } else {
            printf("     복사할 주사위 번호 (1~%d) >> ", DICE_NUM);
            src = read_int(1, DICE_NUM) - 1;
            printf("     덮어쓸 주사위 번호 (1~%d) >> ", DICE_NUM);
            dst = read_int(1, DICE_NUM) - 1;
        }
        dice[dst] = dice[src];
        printf("     %d번 주사위의 눈을 %d번에 복사했습니다!\n", src + 1, dst + 1);
    }
    else if (t == AUG_SABOTAGE || t == AUG_STEAL) {
        attack_enemy(p, me, enemy, a);
    }
    else if (t == AUG_GAMBLER) {
        printf("     동전을 던집니다...\n");
        if (rand() % 2 == 1) {
            p[me].score_mult *= a.value;
            printf("     앞면! 이번 턴 점수 x%d !!\n", a.value);
        } else {
            p[me].score_mult = 0;
            printf("     뒷면... 이번 턴 점수는 0점입니다. (버리고 싶은 족보에 쓰세요)\n");
        }
    }
    else if (t == AUG_YACHT_GUARD) {
        if (p[me].is_ai == 1) {
            v = 6;
        } else {
            printf("     만들고 싶은 눈 (1~6) >> ");
            v = read_int(1, 6);
        }
        for (i = 0; i < DICE_NUM; i++) {
            dice[i] = v;
        }
        printf("     주사위 5개가 모두 %d이(가) 되었습니다! YACHT!\n", v);
    }
    else if (t == AUG_CHAOS) {
        roll_dice(dice);
        p[me].score_mult *= a.value;
        printf("     주사위 5개를 전부 다시 굴렸고, 이번 턴 점수 x%d !\n", a.value);
    }
}

/* 상대에게 영향을 주는 증강 */
void attack_enemy(Player p[], int me, int enemy, Augment a)
{
    int amount;

    if (a.type == AUG_SABOTAGE) {
        p[enemy].reroll_max -= a.value;
        if (p[enemy].reroll_max < 0) {
            p[enemy].reroll_max = 0;
        }
        printf("     %s 의 다음 턴 재굴림이 %d회로 줄었습니다!\n",
               p[enemy].name, p[enemy].reroll_max);
    }
    else if (a.type == AUG_STEAL) {
        amount = p[enemy].total * a.value / 100;
        if (amount < 1 && p[enemy].total > 0) {
            amount = 1;
        }
        p[enemy].bonus -= amount;
        p[me].bonus += amount;
        update_total(p, enemy);
        update_total(p, me);
        printf("     %s 에게서 %d점을 빼앗았습니다!\n", p[enemy].name, amount);
    }
}

/* ============================================================
 *  ⑤ AI 블록
 * ============================================================ */

void ai_select_keep(int dice[], int keep[])
{
    int i, v, kept = 0;

    v = most_common(dice);
    for (i = 0; i < DICE_NUM; i++) {
        if (dice[i] == v) {
            keep[i] = 1;
            kept++;
        } else {
            keep[i] = 0;
        }
    }
    printf(" AI가 %d(이)가 나온 주사위 %d개를 고정했습니다.\n", v, kept);
}

int ai_select_category(Player p[], int me, int dice[])
{
    int i, s, best = -1, best_score = -1;

    for (i = 0; i < COMBO_NUM; i++) {
        if (p[me].used[i] == 0) {
            s = apply_bonus(p, me, calc_score(dice, i));
            if (s > best_score) {
                best_score = s;
                best = i;
            }
        }
    }
    printf(" AI가 [%s] 족보를 선택했습니다.\n", combo_name[best]);
    return best;
}

int ai_pick_augment(Augment aug_list[], int choice[])
{
    int i, best = 0;

    for (i = 1; i < AUG_CHOICE; i++) {
        if (aug_list[choice[i]].grade > aug_list[choice[best]].grade) {
            best = i;
        }
    }
    printf(" AI가 %d번 카드 [%s] 를 선택했습니다.\n", best + 1,
           aug_list[choice[best]].name);
    return choice[best];
}

/* ============================================================
 *  ⑥ 출력 블록
 * ============================================================ */

void print_dice(int dice[], int keep[])
{
    char face[7][3][10] = {
        {"", "", ""},
        {"|       |", "|   o   |", "|       |"},
        {"| o     |", "|       |", "|     o |"},
        {"| o     |", "|   o   |", "|     o |"},
        {"| o   o |", "|       |", "| o   o |"},
        {"| o   o |", "|   o   |", "| o   o |"},
        {"| o   o |", "| o   o |", "| o   o |"}
    };
    int r, i;

    printf("\n");
    for (i = 0; i < DICE_NUM; i++) {
        printf("+-------+  ");
    }
    printf("\n");
    for (r = 0; r < 3; r++) {
        for (i = 0; i < DICE_NUM; i++) {
            printf("%s  ", face[dice[i]][r]);
        }
        printf("\n");
    }
    for (i = 0; i < DICE_NUM; i++) {
        printf("+-------+  ");
    }
    printf("\n");
    for (i = 0; i < DICE_NUM; i++) {
        if (keep[i] == 1) {
            printf("  *[%d]*    ", i + 1);
        } else {
            printf("   [%d]     ", i + 1);
        }
    }
    printf("\n  (*는 고정된 주사위)\n");
}

void print_scoreboard(Player p[])
{
    int i;
    char s0[20], s1[20];

    printf("\n============== SCOREBOARD ==============\n");
    printf(" %-14s %10s %10s\n", "Combo", p[0].name, p[1].name);
    printf(" ----------------------------------------\n");
    for (i = 0; i < COMBO_NUM; i++) {
        if (p[0].used[i] == 1) {
            sprintf(s0, "%d", p[0].score[i]);
        } else {
            strcpy(s0, "-");
        }
        if (p[1].used[i] == 1) {
            sprintf(s1, "%d", p[1].score[i]);
        } else {
            strcpy(s1, "-");
        }
        printf(" %-14s %10s %10s\n", combo_name[i], s0, s1);
    }
    if (p[0].bonus != 0 || p[1].bonus != 0) {
        printf(" %-14s %10d %10d\n", "BONUS(steal)", p[0].bonus, p[1].bonus);
    }
    printf(" ----------------------------------------\n");
    printf(" %-14s %10d %10d\n", "TOTAL", p[0].total, p[1].total);
    printf("========================================\n");
}

void print_augment_cards(Augment aug_list[], int choice[])
{
    char stars[4][5] = {"", "*", "**", "***"};
    int i;
    Augment a;

    printf("\n [증강 선택]  3개 중 1개를 고르세요!\n");
    printf(" --------------------------------------------------\n");
    for (i = 0; i < AUG_CHOICE; i++) {
        a = aug_list[choice[i]];
        printf("  [%d] (%-3s) %s\n", i + 1, stars[a.grade], a.name);
        printf("             %s\n", a.desc);
    }
    printf(" --------------------------------------------------\n");
}

/* ============================================================
 *  ⑦ 종료 블록
 * ============================================================ */

int check_end(int round)
{
    if (round > ROUND_MAX) {
        return 1;
    }
    return 0;
}

void show_result(Player p[])
{
    print_scoreboard(p);
    printf("\n==================== 최종 결과 ====================\n");
    printf("  %s : %d점\n", p[0].name, p[0].total);
    printf("  %s : %d점\n", p[1].name, p[1].total);
    printf("---------------------------------------------------\n");
    if (p[0].total > p[1].total) {
        printf("  승자 : %s !!!\n", p[0].name);
    } else if (p[1].total > p[0].total) {
        printf("  승자 : %s !!!\n", p[1].name);
    } else {
        printf("  무승부입니다!\n");
    }
    printf("===================================================\n\n");
}
