#include "algorithms.h"

#include <assert.h>

static void test_bubble_sort_steps(void)
{
    int values[] = {3, -1, 2};
    BubbleState state = {
        .values = values, .count = 3, .compared = -1, .i = 0, .end = 2
    };
    Step run = {.state = &state, .advance = n_bubble_step};

    n_bubble_step(&run);
    assert(values[0] == -1 && values[1] == 3 && values[2] == 2);
    assert(state.compared == 0);
    assert(run.operations == 1 && run.comparisons == 1 && !run.done);

    n_bubble_step(&run);
    assert(values[0] == -1 && values[1] == 2 && values[2] == 3);
    assert(state.compared == 1);
    assert(run.operations == 2 && run.comparisons == 2 && !run.done);

    n_bubble_step(&run);
    assert(values[0] == -1 && values[1] == 2 && values[2] == 3);
    assert(state.compared == 0 && run.done);
    assert(run.operations == 3 && run.comparisons == 3);

    n_bubble_step(&run);
    assert(run.operations == 3 && run.comparisons == 3);
}

static void test_bubble_sort_small_inputs(void)
{
    int one_value[] = {0};
    BubbleState one_state = {
        .values = one_value, .count = 1, .compared = -1, .end = 0
    };
    Step one_run = {.state = &one_state, .advance = n_bubble_step};

    n_bubble_step(&one_run);
    assert(one_run.done);
    assert(one_run.operations == 0 && one_run.comparisons == 0);

    BubbleState empty_state = {
        .values = NULL, .count = 0, .compared = -1, .end = -1
    };
    Step empty_run = {.state = &empty_state, .advance = n_bubble_step};
    n_bubble_step(&empty_run);
    assert(empty_run.done);
    assert(empty_run.operations == 0 && empty_run.comparisons == 0);
}

static void test_binary_search_steps(void)
{
    int values[] = {-4, 0, 3, 3, 9};
    BinarySearchState state = {
        .values = values, .count = 5, .target = 3,
        .low = 0, .high = 5, .compared = -1, .found = -1
    };
    Step run = {.state = &state, .advance = n_binary_search_step};

    n_binary_search_step(&run);
    assert(state.compared == 2 && state.found == 2 && run.done);
    assert(run.operations == 1 && run.comparisons == 1);

    n_binary_search_step(&run);
    assert(state.found == 2);
    assert(run.operations == 1 && run.comparisons == 1);
}

static void test_binary_search_missing_and_single_value(void)
{
    int values[] = {-4, 0, 3, 6, 9};
    BinarySearchState missing = {
        .values = values, .count = 5, .target = 5,
        .low = 0, .high = 5, .compared = -1, .found = -1
    };
    Step missing_run = {.state = &missing, .advance = n_binary_search_step};
    while (!missing_run.done) n_binary_search_step(&missing_run);
    assert(missing.found == -1);
    assert(missing_run.operations == 3 && missing_run.comparisons == 5);
    assert(missing.compared == 3);

    int only_value[] = {7};
    BinarySearchState single = {
        .values = only_value, .count = 1, .target = 8,
        .low = 0, .high = 1, .compared = -1, .found = -1
    };
    Step single_run = {.state = &single, .advance = n_binary_search_step};
    n_binary_search_step(&single_run);
    assert(single_run.done && single.found == -1);
    assert(single_run.operations == 1 && single_run.comparisons == 1);
}

static void test_binary_search_empty_input(void)
{
    BinarySearchState state = {
        .values = NULL, .count = 0, .target = 1,
        .low = 0, .high = 0, .compared = -1, .found = -1
    };
    Step run = {.state = &state, .advance = n_binary_search_step};

    n_binary_search_step(&run);
    assert(run.done);
    assert(run.operations == 0 && run.comparisons == 0);
    assert(state.compared == -1 && state.found == -1);
}

int main(void)
{
    test_bubble_sort_steps();
    test_bubble_sort_small_inputs();
    test_binary_search_steps();
    test_binary_search_missing_and_single_value();
    test_binary_search_empty_input();
    return 0;
}
