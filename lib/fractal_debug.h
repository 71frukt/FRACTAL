#ifndef FRACTAL_DEBUG_H
#define FRACTAL_DEBUG_H

#define GRAPH_DEBUG

enum FractalExitCode
{
    USER_SCREEN_PTR_ERR,
    WINDOW_PTR_ERR,
    SCALE_SIGN_ERR,
    KEY_EVENT_ERR,
    FRACTAL_OK
};

#define FRACTAL_EXIT_CODE_TO_STR(code)                                          \
(                                                                                \
    code == USER_SCREEN_PTR_ERR ? "user_screen ptr = NULL"                   :    \
    code == WINDOW_PTR_ERR      ? "window ptr = NULL"                        :     \
    code == SCALE_SIGN_ERR      ? "scale < 0"                                :      \
    code == KEY_EVENT_ERR       ? "key_event ptr = NULL"                     :       \
    code == FRACTAL_OK          ? "no error (why did the error occur then?)" :        \
    "UNKNOWN_ERROR"                                                                    \
)

struct CodePlace
{
    const char *const file;
    const char *const func;
    const int         line;
};


struct FractalError
{
    FractalExitCode code;
    CodePlace assert_place;
};




#ifdef GRAPH_DEBUG
#define SUCCESS_EXIT  {FRACTAL_OK, {}}

#define ERROR_HANDLER(fractal_err) do                                                            \
{                                                                                                 \
    if (fractal_err.code != FRACTAL_OK)                                                            \
    {                                                                                               \
        CodePlace assert_place = fractal_err.assert_place;                                           \
                                                                                                      \
        fprintf(stderr, "FRACTAL ERROR in %s:%d:\t\033[31m%s\033[0m\n\tassert called in %s:%d (%s)\n", \
             __FILE__, __LINE__, FRACTAL_EXIT_CODE_TO_STR(fractal_err.code),                            \
             assert_place.file, assert_place.line, assert_place.func);                                   \
        abort();                                                                                          \
    }                                                                                                      \
} while (0)

#else
#define SUCCESS_EXIT
#define ERROR_HANDLER(fractal_err)
#endif

#endif