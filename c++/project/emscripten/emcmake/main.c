#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "cJSON/cJSON.h"

int json_parse(const char *jsonstr, char *output) {
    cJSON *json = cJSON_Parse(jsonstr);
    cJSON *data = cJSON_GetObjectItem(json, "data");
    cJSON_SetValuestring(data, "Hi!");

    const char *string = cJSON_Print(json);
    memcpy(output, string, strlen(string));

    cJSON_Delete(json);
    return 0;
}


int main() {
    const char jsonstr[] = "{\"data\":\"Hello World!\"}";
    cJSON *json = cJSON_Parse(jsonstr);

    const cJSON *data = cJSON_GetObjectItem(json, "data");
    printf("%s\n", cJSON_GetStringValue(data));

    cJSON_Delete(json);
    return 0;
}
