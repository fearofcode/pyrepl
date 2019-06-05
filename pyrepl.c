#include <stdio.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + 256))

int
main(int argc, char *argv[])
{
    char buf[BUF_LEN] __attribute__ ((aligned(8)));

    if (argc != 2 ) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        exit(1);
    }

    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        fprintf(stderr, "inotify_init() returned -1");
        exit(1);
    }

    const char* watch_path = argv[1];

    if (access(watch_path, F_OK) == -1) {
        fprintf(stderr, "File '%s' does not exist.\n", watch_path);
        exit(1);
    }

    int wd = inotify_add_watch(inotify_fd, watch_path, IN_CLOSE_WRITE);
    if (wd == -1) {
        fprintf(stderr, "inotify_add_watch() returned -1\n");
        exit(1);
    }

    printf("Watching %s\n", watch_path);

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);

    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    Py_SetProgramName(program);
    Py_Initialize();

    for (;;) {
        ssize_t num_read = read(inotify_fd, buf, BUF_LEN);
        if (num_read <= 0) {
            fprintf(stderr, "read() read <= 0 bytes\n");
            exit(1);
        }

        for (char* p = buf; p < buf + num_read; ) {
            struct inotify_event* event = (struct inotify_event *) p;
            time_t now;
            time(&now);
            char date_str[100];
            struct tm* local_now = localtime(&now);
            strftime(date_str, 100, "%Y-%m-%d %T", local_now);

            printf("=== %s ===\n", date_str);

            FILE *file = _Py_fopen(watch_path, "r+");
            PyRun_SimpleFile(file, watch_path);
            printf("\n");
            p += sizeof(struct inotify_event) + event->len;
        }
    }

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }

    PyMem_RawFree(program);
    return 0;
}
