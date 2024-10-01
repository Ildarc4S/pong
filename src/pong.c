#include <stdio.h>

void pong();
void clearLine();
int keyAnalysis();
int checkBlows(int coord, int HEIGHT);
int scoresCounterRight(int ball_coord_x, int scores);
int scoresCounterLeft(int ball_coord_x, int WIDTH, int scores);
void screenRendering(int left_rocket, int right_rocket, int ball_coord_x, int ball_coord_y, const int WIDTH,
                     const int HEIGHT);
void choosWinner(const int SCORES_RIGHT, const int SCORES_LEFT, int rocket_left_coord_y,
                 int rocket_right_coord_y, int ball_coord_x, int ball_coord_y, const int WIDTH,
                 const int HEIGHT);
int ballControlX(int ball_dir_x, int ball_coord_x, int ball_coord_y, const int rocket_left_coord_x,
                 int rocket_left_coord_y, const int rocket_right_coord_x, int rocket_right_coord_y);
int ballControlY(int ball_dir_y, int ball_coord_x, int ball_coord_y, const int rocket_left_coord_x,
                 int rocket_left_coord_y, const int rocket_right_coord_x, int rocket_right_coord_y,
                 int HEIGHT);
void gameCycle(int WIDTH, int HEIGHT, int ball_coord_x, int ball_coord_y, int ball_dir_x, int ball_dir_y,
               const int ROCKET_RIGHT_COORD_X, int rocket_right_coord_y, const int ROCKET_LEFT_COORD_X,
               int rocket_left_coord_y, int scores_right, int scores_left, int temp_new_scores,
               const int MAX_SCORE);

int main() {
    pong();
    return 0;
}

void pong() {
    const int MAX_SCORE = 21;
    const int WIDTH = 82;
    const int HEIGHT = 27;

    int scores_right = 0;
    int scores_left = 0;
    int temp_new_scores = 0;

    int ball_coord_x = (WIDTH - 1) / 2;
    int ball_coord_y = (HEIGHT - 1) / 2;
    int ball_dir_x = -1, ball_dir_y = 0;

    const int ROCKET_RIGHT_COORD_X = WIDTH - 3;
    int rocket_right_coord_y = 2;

    const int ROCKET_LEFT_COORD_X = 2;
    int rocket_left_coord_y = 3;

    gameCycle(WIDTH, HEIGHT, ball_coord_x, ball_coord_y, ball_dir_x, ball_dir_y, ROCKET_RIGHT_COORD_X,
              rocket_right_coord_y, ROCKET_LEFT_COORD_X, rocket_left_coord_y, scores_right, scores_left,
              temp_new_scores, MAX_SCORE);
}

void gameCycle(const int WIDTH, const int HEIGHT, int ball_coord_x, int ball_coord_y, int ball_dir_x,
               int ball_dir_y, const int ROCKET_RIGHT_COORD_X, int rocket_right_coord_y,
               const int ROCKET_LEFT_COORD_X, int rocket_left_coord_y, int scores_right, int scores_left,
               int temp_new_scores, const int MAX_SCORE) {
    while (scores_right < MAX_SCORE && scores_left < MAX_SCORE) {
        printf("Scores left: %d     ", scores_left);
        printf("Scores right: %d\n", scores_right);
        screenRendering(rocket_left_coord_y, rocket_right_coord_y, ball_coord_x, ball_coord_y, WIDTH, HEIGHT);

        int key = keyAnalysis();
        if (key == 1)
            rocket_left_coord_y--;  // Press A
        else if (key == 2)
            rocket_left_coord_y++;  // Press Z
        else if (key == 3)
            rocket_right_coord_y--;  // Press K
        else if (key == 4)
            rocket_right_coord_y++;  // Press M
        else if (key == 0)
            continue;  // Press trash

        rocket_left_coord_y = checkBlows(rocket_left_coord_y, HEIGHT);
        rocket_right_coord_y = checkBlows(rocket_right_coord_y, HEIGHT);

        ball_coord_x += ball_dir_x;
        ball_coord_y += ball_dir_y;

        ball_dir_x = ballControlX(ball_dir_x, ball_coord_x, ball_coord_y, ROCKET_LEFT_COORD_X,
                                  rocket_left_coord_y, ROCKET_RIGHT_COORD_X, rocket_right_coord_y);
        ball_dir_y = ballControlY(ball_dir_y, ball_coord_x, ball_coord_y, ROCKET_LEFT_COORD_X,
                                  rocket_left_coord_y, ROCKET_RIGHT_COORD_X, rocket_right_coord_y, HEIGHT);

        temp_new_scores = scoresCounterRight(ball_coord_x, scores_right);
        if (temp_new_scores != scores_right) {
            ball_coord_x = (WIDTH - 1) / 2;
            ball_coord_y = (HEIGHT - 1) / 2;
            scores_right = temp_new_scores;
            ball_dir_x = 1, ball_dir_y = 0;
        }

        temp_new_scores = scoresCounterLeft(ball_coord_x, WIDTH, scores_left);
        if (temp_new_scores != scores_left) {
            ball_coord_x = (WIDTH - 1) / 2;
            ball_coord_y = (HEIGHT - 1) / 2;
            scores_left = temp_new_scores;
            ball_dir_x = -1, ball_dir_y = 0;
        }
    }
    choosWinner(scores_right, scores_left, rocket_left_coord_y, rocket_right_coord_y, ball_coord_x,
                ball_coord_y, WIDTH, HEIGHT);
}

void choosWinner(const int SCORES_RIGHT, const int SCORES_LEFT, int rocket_left_coord_y,
                 int rocket_right_coord_y, int ball_coord_x, int ball_coord_y, const int WIDTH,
                 const int HEIGHT) {
    printf("Scores left: %d     ", SCORES_LEFT);
    printf("Scores right: %d\n", SCORES_RIGHT);
    screenRendering(rocket_left_coord_y, rocket_right_coord_y, ball_coord_x, ball_coord_y, WIDTH, HEIGHT);

    if (SCORES_RIGHT > SCORES_LEFT) {
        printf("Congratulations to the player on the right!\n");
    } else {
        printf("Congratulations to the player on the left!\n");
    }
}

void screenRendering(int left_rocket, int right_rocket, int ball_coord_x, int ball_coord_y, const int WIDTH,
                     const int HEIGHT) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            if ((i == 0 || i == HEIGHT - 1) && j != WIDTH)
                printf("#");
            else if (j == WIDTH)
                printf("\n");
            else if ((j == 0 || j == WIDTH - 1))
                printf("|");
            else if (i == ball_coord_y && j == ball_coord_x)
                printf("*");
            else if ((i == left_rocket || i == left_rocket + 1 || i == left_rocket + 2) && j == 2)
                printf("]");
            else if ((i == right_rocket || i == right_rocket + 1 || i == right_rocket + 2) && j == WIDTH - 3)
                printf("[");
            else
                printf(" ");
        }
    }
}

int checkBlows(int coord, int HEIGHT) {
    if (coord < 1) {
        return 1;  // ударяемся в верхнюю границу
    } else if (coord + 3 >= HEIGHT) {
        return HEIGHT - 4;  // граница = HEIGHT-1 и ширина рокета = 3
    }
    return coord;
}

int ballControlX(int ball_dir_x, int coord_x, int ball_coord_y, const int rocket_left_coord_x,
                 int rocket_left_coord_y, const int rocket_right_coord_x, int rocket_right_coord_y) {
    if (coord_x == rocket_left_coord_x + 1) {
        if (ball_coord_y == rocket_left_coord_y || ball_coord_y == rocket_left_coord_y + 1 ||
            ball_coord_y == rocket_left_coord_y + 2)
            return -ball_dir_x;
    } else if (coord_x == rocket_right_coord_x - 1) {
        if (ball_coord_y == rocket_right_coord_y || ball_coord_y == rocket_right_coord_y + 1 ||
            ball_coord_y == rocket_right_coord_y + 2)
            return -ball_dir_x;
    }
    return ball_dir_x;
}

int ballControlY(int ball_dir_y, int ball_coord_x, int ball_coord_y, const int rocket_left_coord_x,
                 int rocket_left_coord_y, const int rocket_right_coord_x, int rocket_right_coord_y,
                 int HEIGHT) {
    if (ball_coord_x == rocket_left_coord_x + 1) {
        if (ball_coord_y == rocket_left_coord_y)
            return -1;
        else if (ball_coord_y == rocket_left_coord_y + 1)
            return 0;
        else if (ball_coord_y == rocket_left_coord_y + 2)
            return 1;
    } else if (ball_coord_x == rocket_right_coord_x - 1) {
        if (ball_coord_y == rocket_right_coord_y) {
            return -1;
        } else if (ball_coord_y == rocket_right_coord_y + 1)
            return 0;
        else if (ball_coord_y == rocket_right_coord_y + 2)
            return 1;
    } else if (ball_coord_y == 1 || ball_coord_y == HEIGHT - 2) {
        return -ball_dir_y;
    }
    return ball_dir_y;
}

int scoresCounterRight(int ball_coord_x, int scores) {
    if (ball_coord_x < 1) return ++scores;
    return scores;
}

int scoresCounterLeft(int ball_coord_x, int WIDTH, int scores) {
    if (ball_coord_x > WIDTH - 2) return ++scores;
    return scores;
}

int keyAnalysis() {
    char key;
    scanf("%c", &key);
    clearLine();
    if (key == 'A') {
        return 1;
    } else if (key == 'Z') {
        return 2;
    } else if (key == 'K') {
        return 3;
    } else if (key == 'M') {
        return 4;
    } else if (key == ' ') {
        return 5;
    }
    return 0;
}

void clearLine() {
    while (getchar() != '\n') {
    }
}
