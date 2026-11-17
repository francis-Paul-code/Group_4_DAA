#include <gtk/gtk.h>

#include "algorithms.h"

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