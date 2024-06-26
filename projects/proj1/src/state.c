#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

#define WALL '#'
#define FRUIT '*'
#define RIGHT_HEAD 'D'
#define LEFT_HEAD 'A'
#define UP_HEAD 'W'
#define DOWN_HEAD 'S'
#define RIGHT_TAIL 'd'
#define LEFT_TAIL 'a'
#define UP_TAIL 'w'
#define DOWN_TAIL 's'
#define RIGHT_BODY '>'
#define LEFT_BODY '<'
#define UP_BODY '^'
#define DOWN_BODY 'v'
#define END '\n'
#define TERMINATION '\0'

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row , unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);
static void allocation_failed();

static unsigned int init_rows =  18;
static unsigned int init_cols = 20;

/* Task 1 */
game_state_t *create_default_state() {
  game_state_t *default_state = malloc(sizeof(game_state_t));
  if (default_state == NULL) {
    allocation_failed();
  }
  
  default_state->num_rows = init_rows;
  default_state->num_snakes = 1;
  default_state->snakes = malloc(sizeof(snake_t));
  if (default_state->snakes == NULL)
  {
    free(default_state);
    allocation_failed();
  }
  snake_t* snake = default_state->snakes;
  snake->head_col = 4;
  snake->head_row = 2;
  snake->tail_row = 2;
  snake->tail_col = 2;
  snake->live = true;
  

  default_state->board = malloc(init_rows *sizeof(char*));
  if (default_state->board == NULL) {
    free(default_state);
    allocation_failed();
  }
  
  for (unsigned int i = 0; i < init_rows ; i++) {
    default_state->board[i] = malloc((init_cols+2)*sizeof(char));
    
    if (default_state->board[i] == NULL)
    {
      free_state(default_state);
      allocation_failed();
    }
    set_board_at(default_state, i, init_cols+1, TERMINATION);
    set_board_at(default_state, i, init_cols, END);
    
    set_board_at(default_state, i, 0, WALL);
    set_board_at(default_state, i, init_cols-1, WALL);
    for (unsigned int j = 1; j < init_cols-1; j++) {
      if (i == 0 || i == init_rows -1)
      {        
        set_board_at(default_state, i, j, WALL);
      } else {
        set_board_at(default_state, i, j, ' ');
      }
    }
  }
  set_board_at(default_state, 2, 9, FRUIT);
  set_board_at(default_state, 2, 2, RIGHT_TAIL);
  set_board_at(default_state, 2, 3, RIGHT_BODY);
  set_board_at(default_state, 2, 4, RIGHT_HEAD);
  return default_state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  for (int i = 0; i < state->num_rows; i++)
  {
    free(state->board[i]);
  }
  
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for(int i = 0; i < state->num_rows; i++) {
    fprintf(fp, state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Utility function to handle allocation failures. In this
   case we print a message and exit. */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row , unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row , unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  return c == RIGHT_TAIL || c == LEFT_TAIL || c == UP_TAIL|| c == DOWN_TAIL;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  return c == RIGHT_HEAD || c == LEFT_HEAD || c == UP_HEAD|| c == DOWN_HEAD;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  return is_head(c) || is_tail(c) 
  || c == RIGHT_BODY || c == LEFT_BODY || c == UP_BODY|| c == DOWN_BODY;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  switch (c)
  {
  case RIGHT_BODY: {
    return RIGHT_TAIL;
  }
  case LEFT_BODY: {
    return LEFT_TAIL;
  }
  case UP_BODY: {
    return UP_TAIL;
  }
  case DOWN_BODY: {
    return DOWN_TAIL;
  }
  default:
    return '?';
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  switch (c)
  {
  case RIGHT_HEAD: {
    return RIGHT_BODY;
  }
  case LEFT_HEAD: {
    return LEFT_BODY;
  }
  case UP_HEAD: {
    return UP_BODY;
  }
  case DOWN_HEAD: {
    return DOWN_BODY;
  }
  default:
    return '?';
  }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  if (c == DOWN_TAIL || c == DOWN_BODY ||c == DOWN_HEAD )
  {
    return cur_row + 1;
  }
  if (c == UP_TAIL || c == UP_BODY || c == UP_HEAD )
  {
    return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  if (c == LEFT_TAIL || c == LEFT_BODY ||c == LEFT_HEAD )
  {
    return cur_col - 1;
  }
  if (c == RIGHT_TAIL || c == RIGHT_BODY ||c == RIGHT_HEAD )
  {
    return cur_col + 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  snake_t* snake = &state->snakes[snum];
  char head = get_board_at(state, snake->head_row, snake->head_col);
  unsigned int row  = get_next_row(snake->head_row, head);
  unsigned int col = get_next_col(snake->head_col, head);
  if (head == UP_HEAD)
  {
    printf("%d,%d\n", row, col);
  }
  
  return get_board_at(state, row, col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  snake_t* snake = &state->snakes[snum];
  char head = get_board_at(state, snake->head_row, snake->head_col);
  unsigned int row  = get_next_row(snake->head_row, head);
  unsigned int col = get_next_col(snake->head_col, head);
  set_board_at(state, row, col, head);
  char body = head_to_body(head);
  set_board_at(state, snake->head_row, snake->head_col, body);
  snake->head_row = row;
  snake->head_col = col;
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  snake_t* snake = &state->snakes[snum];
  char tail = get_board_at(state, snake->tail_row, snake->tail_col);
  unsigned int row  = get_next_row(snake->tail_row, tail);
  unsigned int col = get_next_col(snake->tail_col, tail);
  char body = get_board_at(state, row, col);
  tail = body_to_tail(body);
  set_board_at(state, row, col, tail);
  set_board_at(state, snake->tail_row, snake->tail_col, ' ');
  snake->tail_row = row;
  snake->tail_col = col;
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  for(unsigned  i = 0; i < state->num_snakes;i++) {
    char next = next_square(state, i);
    snake_t* snake = &state->snakes[i];
    if (is_snake(next) || next == WALL) {
      snake->live = false;
      set_board_at(state, snake->head_row, snake->head_col, 'x');
      continue;
    } 
    update_head(state, i);
    if (next == FRUIT) {
      add_food(state);
    } else {
      update_tail(state, i);
    }
    
  }
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  int buffer_size = 100;
  char *line = NULL;
  char buffer[buffer_size];
  size_t current_length = 0;
  while (fgets(buffer, buffer_size, fp) != NULL) {
    size_t length = strlen(buffer);
        
    // Reallocate memory to hold the accumulated line
    line = realloc(line, current_length + length + 1);
    if (line == NULL) {
      return line;
    }

    // Copy the buffer to the accumulated line
    strcpy(line + current_length, buffer);
    current_length += length;

    // Check if the line is fully read (last character is a newline)
    if (buffer[length - 1] == '\n') {
      break;
    }
  }
  return line;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  game_state_t *state = malloc(sizeof(game_state_t));
  char *line = read_line(fp);
  unsigned int rows = 0;
  state->board = NULL;
  while (line != NULL)
  {
    rows++;
    state->board = realloc(state->board, rows*sizeof(char*));
    state->board[rows-1] = line;
    line = read_line(fp);
  }
  state->num_rows = rows;
  state->num_snakes = 0;
  state->snakes = NULL;
  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  snake_t* snake = &state->snakes[snum];;
  unsigned int row = snake->tail_row, col = snake->tail_col;
  char c = get_board_at(state, row, col);
  while (!is_head(c) && is_snake(c)) {
    row = get_next_row(row, c);
    col = get_next_col(col, c);
    c = get_board_at(state, row, col);
  }
  snake->head_row = row;
  snake->head_col = col;
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  char** board = state->board;
  unsigned int snum = 0;
  for (unsigned int i = 1; i < state->num_rows-1; i++) {
    for (unsigned int j = 1; board[i][j] != WALL; j++) {
      if (!is_tail(board[i][j]))
      {
        
       continue;
      }
      state->snakes = realloc(state->snakes, (snum+1)*sizeof(snake_t));
      state->snakes[snum].tail_row = i; 
      state->snakes[snum].tail_col = j;
      find_head(state, snum);
      snum++;
    }
  }
  state->num_snakes = snum;
  return state;
}
