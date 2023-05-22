#include "leptjson.h"
#include <assert.h>
#include <stdlib.h>

#define EXPECT(c, ch) do {assert(*c->json == (ch)); c->json++;}while(0)

/* 为了减少解析函数之间传递多个参数，将数据放入一个结构体 */
typedef struct {
    const char* json;
}lept_context;

/* ws = *(%x20 / %x09 / %x0A / %x0D) 表示空格符、制表符、换行符、回车符，*（）表示其中的一个或多个 */
static void lept_parse_whitespace(lept_context *c) {
    const char *p = c->json;
    while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

#if 0
/* null = "null" */
static int lept_parse_null(lept_context *c, lept_value *v) {
    EXPECT(c, 'n');
    if(c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

/* false == "false" */
static int lept_parse_false(lept_context *c, lept_value *v) {
    EXPECT(c, 'f');
    if(c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

/* true == "true" */
static int lept_parse_true(lept_context *c, lept_value *v) {
    EXPECT(c, 't');
    if(c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json +=3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}
#endif

static int lept_parse_literal(lept_context *c, lept_value *v, const char *literal, lept_type type) {
    size_t i;
    EXPECT(c, literal[0]);
    for(i = 0; literal[i+1]; i++)
        if(c->json[i] != literal[i + 1]
            return LEPT_PARSE_INVALID_VALUE;
    c->json += i;
    v->type = type;
    return LEPT_PARSE_OK;
}

/* 十进制转换至二进制 */
/* 使用标准库的strtod()转换 */
static int lept_parse_number(lept_context *c, lept_type *v) {
    char *end;
    /* \R0D0 validate number */
    v->n = strtod(c->json, &end);
    if(c->json == end)
        return LEPT_PARSE_INVALID_VALUE;
    c->json = end;
    v->lept_type = LEPT_NUMBER;
    return LEPT_PARSE_OK;
}

/* value == null / false / true */
static int lept_parse_value(lept_context *c, lept_value *v) {
    switch(*c->json) {
        /*
        case 'n': return lept_parse_null(c, v);
        case 'f': return lept_parse_false(c, v);
        case 't': return lept_parse_true(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default : return lept_parse_number(c, v);
        */

        case 't': return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f': return lept_parse_literal(c, v, "false", LEPT_FALSE);
        case 'n': return lept_parse_literal(c, v, "null", LEPT_NULL);
    }
}

/* 这里的格式应该是 JSON-text = ws value ws */
int lept_parse(lept_value* v, const char *json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if(*c.json != '\0')
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

/* 仅当type == LEPT_NUMBER时，n才表示JSON数字的值*/
double lept_get_number(const lept_type *v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}


