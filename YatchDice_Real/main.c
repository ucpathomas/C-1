#include "func.h"

/* ============================================================
 *  증강 요트 다이스 - main.c
 *  게임 전체 흐름만 담당하고, 실제 기능은 모두 func.c 의 함수가 처리한다.
 * ============================================================ */

int main(void)
{
    Player  p[PLAYER_NUM];        /* 플레이어 2명 */
    Augment aug_list[AUG_NUM];    /* 전체 증강 목록 */
    int mode;
    int round;
    int i;

    show_title();
    mode = select_mode();
    init_game(p, aug_list, mode);

    round = 1;
    while (check_end(round) == 0) {
        for (i = 0; i < PLAYER_NUM; i++) {
            /* i 번 플레이어가 턴 진행, 상대는 (1 - i) */
            play_turn(p, i, 1 - i, aug_list, round);
        }
        round++;
    }

    show_result(p);
    return 0;
}
