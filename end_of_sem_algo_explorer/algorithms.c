#include <gtk/gtk.h>

#include "algorithms.h"

// insertion sort
void n_insertion_step(Step *prev_step)
{
    if (prev_step->done) return;

    InsertionState *s = prev_step->state;

    if (s->i >= s->count) {
        prev_step->done = true;
        s->compared = -1;
        return;
    }

    s->compared = s->j;
    count_step(prev_step, s->j >= 0 ? 1 : 0);

    if (s->j >= 0 && s->values[s->j] > s->saved) {
        s->values[s->j + 1] = s->values[s->j];
        s->j--;
        return;
    }

    s->values[s->j + 1] = s->saved;
    s->i++;

    if (s->i < s->count) {
        s->saved = s->values[s->i];
        s->j = s->i - 1;
    } else {
        prev_step->done = true;
        s->compared = -1;
    }
}

// bubble soert

void n_bubble_step(Step *prev_step)
{
    if (prev_step->done) return;

    BubbleState *s = prev_step->state;
    int i = s->i;
    s->compared = i;

    count_step(prev_step, 1);

    if (s->values[i] > s->values[i + 1]) {
        int temp = s->values[i];
        s->values[i] = s->values[i + 1];
        s->values[i + 1] = temp;
    }

    if (++s->i == s->end) {
        s->i = 0;
        prev_step->done = (--s->end == 0);
    }
}

// fibonacci
void n_fib_step(Step *prev_step) {

    if (prev_step->done) return;

    FibState *fib = prev_step->state; // get the fibState of previous step

    printf("Current Iteration: %d, Current Number: %llu, Next Number: %llu \n", fib->id, fib->current, fib->next);

    uint64_t previous = fib->current; // the current operand of the prev_step becomes the previous operand
    fib->current = fib->next; // the next operand of the prev_state becomes the current
    fib->id++; // id of the current fibState in the operands list
    prev_step->done = (fib->id == fib->target);

    if (!prev_step->done)
        fib->next = previous + fib->current;
}