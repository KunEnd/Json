#ifndef LEPTJSON_H__
#define LEPTJSON_H__

typedef enum { LEPT_NULL, LEPT_TRUE, LEPT_FALSE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT}lept_type;

typedef struct{
    double n;
    lept_type type;
}lept_value;

/* 返回值 */
enum{
    LEPT_PARSE_OK = 0,/* 无错误时返回 */
    LEPT_PARSE_EXPECT_VALUE,/* 该JSON只含空白 */
    LEPT_PARSE_INVALID_VALUE,/* 该JSON中的值不是指定值 */
    LEPT_PARSE_ROOT_NOT_SINGULAR/* 该JSON在值之后，在空白之后还有其他字符 */
};

/* 解析JSON */
/* 传入JSON是一个字符串，使用const char*类型不更改该输入字符 */
int lept_parse(lept_value* v, const char *json);

/* 访问结果，获取其类型 */
lept_type lept_get_type(const lept_value* v);

/* 记录数字 */
double lept_get_number(const lept_type *v);

#endif /* LEPTJSON_H__ */
