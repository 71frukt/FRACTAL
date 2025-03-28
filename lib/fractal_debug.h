#ifndef FRACTAL_DEBUG_H
#define FRACTAL_DEBUG_H

#define GRAPH_DEBUG

enum FractalExitCode
{
    ENV_INFO_PTR_ERR,
    WINDOW_PTR_ERR,
    SCALE_SIGN_ERR,
    KEY_EVENT_ERR,
    FRACTAL_OK,
    SOURCE_SETTINGS_FILE_ERR,
    DEST_SETTINGS_FILE_ERR,
    FONT_ERROR
};

#define FRACTAL_EXIT_CODE_TO_STR(code)                                                              \
(                                                                                                    \
    code == ENV_INFO_PTR_ERR         ? "env_info ptr = NULL"                                     :    \
    code == WINDOW_PTR_ERR           ? "window ptr = NULL"                                       :     \
    code == SCALE_SIGN_ERR           ? "scale < 0"                                               :      \
    code == KEY_EVENT_ERR            ? "key_event ptr = NULL"                                    :       \
    code == FRACTAL_OK               ? "no error (why did the error occur then?)"                :        \
    code == SOURCE_SETTINGS_FILE_ERR ? "source settings file ptr = NULL whilst it's flag stands" :         \
    code == DEST_SETTINGS_FILE_ERR   ? "dest settings file ptr = NULL whilst it's flag stands"   :          \
    code == FONT_ERROR               ? "error opening the font"                                  :           \
    "UNKNOWN_ERROR"                                                                                           \
)

struct CodePlace
{
    const char *const file;
    const char *const func;
    const int         line;
};

#define CUR_PLACE  {__FILE__, __func__, __LINE__}


#ifdef GRAPH_DEBUG

struct FractalError
{
    FractalExitCode code;
    CodePlace assert_place;
};

#define ON_GRAPH_DEBUG(...)  __VA_ARGS__
#define FRACTAL_ERROR(code)  {code, {__FILE__, __func__, __LINE__}}

#define SUCCESS_EXIT  {FRACTAL_OK, {}}

#define ERROR_HANDLER(func_returning_frac_error) do                                              \
{                                                                                                 \
    FractalError error = func_returning_frac_error;                                                \
    if (error.code != FRACTAL_OK)                                                                   \
    {                                                                                                \
        CodePlace assert_place = error.assert_place;                                                  \
                                                                                                       \
        fprintf(stderr, "FRACTAL ERROR in %s:%d:\t\033[31m%s\033[0m\n\tassert called in %s:%d (%s)\n",  \
             __FILE__, __LINE__, FRACTAL_EXIT_CODE_TO_STR(error.code),                                   \
             assert_place.file, assert_place.line, assert_place.func);                                    \
        abort();                                                                                           \
    }                                                                                                       \
} while (0)

#else
typedef void FractalError;
#define ON_GRAPH_DEBUG(...)
#define FRACTAL_ERROR(code)
#define SUCCESS_EXIT
#define ERROR_HANDLER(func_returning_frac_error)  func_returning_frac_error
#endif

#endif