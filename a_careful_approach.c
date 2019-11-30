#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// maximum number of landing scenarios
#define MAX_N 8

// we adjusted this iteration number a few times
// this is the least one feasible on OJ test cases 
#define ITER_NUM 18

// a struct used to hold the start and end of a time window during
// which a plane can safely land
typedef struct time_window {
    double start;
    double end;
} time_window;


// landing scenarios, each containing a start time and an end time.
time_window timetable[MAX_N];

// pointers to items in timetable. used for permutation.
time_window *p_items_in_timetable[MAX_N];


// swap ith and jth items in `p_items_in_timetable`
void swap(int i, int j) {
    time_window *temp;

    if (i == j) {
        return;
    }

    temp = p_items_in_timetable[i];
    p_items_in_timetable[i] = p_items_in_timetable[j];
    p_items_in_timetable[j] = temp;
}


// check whether the given mininum gap is feasible or not
bool check_min_time_gap(int n, double min_gap, double landing_time) {
    int i;
    bool feasible;
    double min_next_landing_time, next_landing_time;

    // the `min_gap` is feasible if all items in timetable are visited
    if (n == 0) {
        return true;
    }

    // increase landing time by `min_gap` at least
    min_next_landing_time = landing_time + min_gap;

    // traverse through the `timetable`. recursively check `min_gap`
    // items visited in `timetable` are swapped to the end of the array
    for (i = 0; i < n; i++) {
        if (min_next_landing_time <= p_items_in_timetable[i]->end) {
            // decide `next_landing_time` greedily
            next_landing_time = (
                min_next_landing_time > p_items_in_timetable[i]->start ?
                min_next_landing_time : p_items_in_timetable[i]->start);

            swap(i, n - 1);
            feasible = check_min_time_gap(n - 1, min_gap, next_landing_time);
            swap(i, n - 1);

            if (feasible) {
                return true;
            }
        }
    }
    
    return false;
}


// binary search for the optimal minimum time gap.
// for a given minimum time gap, recursively check whether it is feasible
double binary_search(int n, double lower_bound, double upper_bound) {
    int i, j;
    bool feasible;
    double mid, landing_time;

    // `ITER_NUM` should be sufficiently large here (but not too large) to
    // make sure we have enough precision for `second` value
    for (i = 0; i < ITER_NUM; i++) {
        mid = (lower_bound + upper_bound) / 2;

        for (j = 0; j < n; j++) {
            // note that `landing_time` should be fetched out of two `swap`
            landing_time = p_items_in_timetable[j]->start;

            swap(j, n - 1);
            feasible = check_min_time_gap(n - 1, mid, landing_time);
            swap(j, n - 1);

            // if feasible, increase lower bound
            if (feasible) {
                lower_bound = mid;
                break;
            }
        }

        // if not feasible, decrease upper bound
        if (!feasible) {
            upper_bound = mid;
        }
    }

    return lower_bound;
}


// initialize `timetable` and `p_items_in_timetable`.
// also return the mininum start time and the maximum end time
// of all time windows.
void read_inputs(int n, double *min_start, double *max_end) {
    int i;

    *min_start = 1440;
    *max_end = 0;

    for (i = 0; i < n; i++) {
        scanf("%lf%lf", &timetable[i].start, &timetable[i].end);
        p_items_in_timetable[i] = timetable + i;

        // update `min_start`
        if (timetable[i].start < *min_start) {
            *min_start = timetable[i].start;
        }

        // update `max_end`
        if (timetable[i].end > *max_end) {
            *max_end = timetable[i].end;
        }
    }
}


// output should be displayed in a pattern like
// 'Case <index>: <minute>:<second>'
void display_output(double optimal_min_time_gap, int case_index) {
    int minute, second;

    minute = (int) optimal_min_time_gap;
    // second is rounded to the nearest integer
    second = (int) ((optimal_min_time_gap - minute) * 60 + 0.5);

    if (second == 60) {
        minute += 1;
        second = 0;
    }
    
    printf("Case %d: %d:%02d\n", case_index, minute, second);
}


int main(void) {
    int i, n, case_index;
    double min_start, max_end, optimal_min_time_gap, upper_bound;

    // used in output to indicate the case number
    case_index = 1;

    while (scanf("%d", &n) != EOF) {
        // exit if n is zero
        if (n == 0) {
            break;
        }

        // initialize `timetable` and `p_items_in_timetable`. we will need
        // the mininal start time and the maximal end time to calculate
        // the upper bound for binary search
        read_inputs(n, &min_start, &max_end);

        // upper bound can be decided by `max_end`, `min_start` and `n`
        upper_bound = (max_end - min_start) / (n - 1);
        optimal_min_time_gap = binary_search(n, 0, upper_bound);

        display_output(optimal_min_time_gap, case_index);

        ++case_index;
    }

    return 0;
}
