#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */
#include <assert.h>
#include <stdbool.h>
#include <time.h>

/* functions to manipulate link list */
void push(time_t timestamp);
bool has_next();
time_t get_next();
time_t pop();
void print_list();
/* end */


typedef struct node {
    time_t timestamp;
    struct node *next;
} node;
node *head = NULL;

time_t start;


void reschedule_alarm() {
    time_t now;

    if(!(now = time(NULL))) {
        fprintf(stderr, "Error in _reschedule_alarm;\n");
    }

    node *itr = head;

    while (itr) {
        if (itr->timestamp > now) {
            // set an alarm clock for delivery of a signal
            alarm(itr->timestamp - now);
            break;
        }
        itr = itr->next;
    }
}


void sig_alrm() {
    printf("In sig_alrm, now: %lld, from start to now: %lld\n",
            (long long) time(NULL),
            (long long)(time(NULL) - start)
          );

    reschedule_alarm();
}


void init_alarm() {
    // use to examine and change a signal action, and initialize it.
    struct sigaction act = {{0}};

    // signal set operations
    sigemptyset(&act.sa_mask);
    act.sa_handler = sig_alrm;
    sigaction(SIGALRM, &act, NULL);
}


void set_alarm(int seconds) {
    time_t now = time(NULL);
    time_t scheduled = now + seconds;

    push(scheduled);

    reschedule_alarm();
}


void clear_alarm() {
    printf("\nOverview: ");
    print_list();

    // If argument is zero, any pending alarm is canceled.
    alarm(0);
}


int main() {
    init_alarm();

    start = time(NULL);
    printf("Start time: %lld\n", (long long)start);

    // set 2 sec alarm at 0s, it will finish at 2s after execution.
    set_alarm(2);
    sleep(1);

    // set 6 sec alarm at 1s, it will finish at 7s after execution.
    set_alarm(6);
    sleep(1);

    // set 3 sec alarm at 2s, it will finish at 5s after execution.
    set_alarm(3);
    sleep(4);

    // clear all alarms at 6s after execution.
    clear_alarm();

    return 0;
}


// Timestamps to wake up linked list {{{
void push(time_t timestamp) {
    node *new_node = malloc(sizeof(node));
    new_node->timestamp = timestamp;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
        return;
    }

    if (head->timestamp > new_node->timestamp) {
        new_node->next = head;
        head = new_node;
        return;
    }

    node *prev = head;
    node *itr = head->next;
    while (itr) {
        if (itr->timestamp > new_node->timestamp) {
            prev->next = new_node;
            new_node->next = itr;
            return;
        }
        prev = itr;
        itr = itr->next;
    }

    prev->next = new_node;
}


bool has_next() {
    bool ret = head != NULL;
   return ret;
}


time_t get_next() {
    time_t ret = head->timestamp;
    return ret;
}


time_t pop() {
    assert(has_next());
    node old_head = *head;
    free(head);
    head = old_head.next;
    return old_head.timestamp;
}

void print_list() {
    node *itr = head;
    while (itr) {
        printf("%lld -> ", (long long)itr->timestamp);
        itr = itr->next;
    }
    printf("NULL\n");
}
