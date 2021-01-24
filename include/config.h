#define TEST

#ifdef TEST
    #define REPATH ("../static/RE_test.txt")
    #define CHARTABLE ("../static/Chars.txt")
    #define RENAME ("RE_test.txt")
    #define TEST_INPUTx
    #define TEST_NFAxz
    #define TEST_DFAxz
    #define TEST_mDFAxz
#else
    #define REPATH ("../static/RE.txt")
    #define CHARTABLE ("../static/Chars.txt")
    #define RENAME ("RE.txt")
#endif