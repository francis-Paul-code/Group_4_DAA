#include <gtk/gtk.h>

#include "algorithms.h"

// One logical step compares a single adjacent pair and swaps it if needed.
void n_bubble_step(Step *step)
{
    if (!step || step->done || !step->state) return;

    BubbleState *s = step->state;
    if (s->count < 2 || s->end <= 0) {
        step->done = true;
        return;
    }

    int i = s->i;
    s->compared = i;

    count_step(step, 1);

    if (s->values[i] > s->values[i + 1]) {
        int temp = s->values[i];
        s->values[i] = s->values[i + 1];
        s->values[i + 1] = temp;
    }

    if (++s->i == s->end) {
        s->i = 0;
        step->done = (--s->end == 0);
    }
}

// One logical step examines one midpoint and narrows the active search range.
void n_binary_search_step(Step *step)
{
    if (!step || step->done || !step->state) return;

    BinarySearchState *s = step->state;
    if (s->low >= s->high) {
        step->done = true;
        return;
    }

    int middle = s->low + (s->high - s->low) / 2;
    s->compared = middle;
    int comparisons = 1;

    if (s->values[middle] < s->target) {
        s->low = middle + 1;
    } else if (s->values[middle] > s->target) {
        s->high = middle;
        comparisons++;
    } else {
        s->found = middle;
        step->done = true;
    }

    count_step(step, (guint64)comparisons);
    if (s->low >= s->high) step->done = true;
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