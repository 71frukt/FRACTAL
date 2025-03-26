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

struct CodePlace
{
    const char *const file;
    const char *const func;
    const int         line;
};


struct FractalError
{
    FractalExitCode exit_code;
    CodePlace assert_place;
};



#ifdef GRAPH_DEBUG
#define SUCCESS_EXIT  {FRACTAL_OK, {}}

#define ERROR_HANDLER(fractal_err) do                                               \
{                                                                                    \
    if (fractal_err.exit_code != FRACTAL_OK)                                          \
    {                                                                                  \
        CodePlace assert_place = fractal_err.assert_place;                              \
                                                                                         \
        fprintf(stderr, "FRACTAL ERROR in %s:%d:\n\tassert called in %s:%d (%s)\n",       \
             __FILE__, __LINE__, assert_place.file, assert_place.line, assert_place.func); \
    }                                                                                       \
} while (0)

#else
#endif

#endif