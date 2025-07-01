#include <emscripten/emscripten.h>

// 시간표 JSON 문자열 (static으로 선언)
static const char schedule_json[] =
    "{\"date\":\"2025.07.01\",\"items\":["
    "{\"period\":1,\"subject\":\"공통수학1\"},"
    "{\"period\":2,\"subject\":\"전기·전자 일반\"},"
    "{\"period\":3,\"subject\":\"전기·전자 일반\"},"
    "{\"period\":4,\"subject\":\"체육1\"},"
    "{\"period\":5,\"subject\":\"컴퓨터 시스템 일반\"},"
    "{\"period\":6,\"subject\":\"컴퓨터 시스템 일반\"},"
    "{\"period\":7,\"subject\":\"통합과학1\"}"
    "]}";

// 급식 JSON 문자열
    static const char meal_json[] =
        "{\"date\":\"2025.07.01\",\"mealType\":\"석식\",\"menu\":\"오늘은 급식이 없습니다.\",\"calories\":0}";

    // 프로필 JSON 문자열
    static const char profile_json[] =
        "{\"name\":\"류도호\",\"studentId\":1305}";

    // 기숙사 신청 현황 JSON 문자열
    static const char dorm_json[] =
        "{\"selfStudy\":{\"current\":0,\"max\":50,\"startTime\":\"20:00\"},"
        "\"massageChair\":{\"current\":0,\"max\":5,\"startTime\":\"20:20\"}}";

    EMSCRIPTEN_KEEPALIVE
    const char* get_schedule_json() {
        return schedule_json;
    }

    EMSCRIPTEN_KEEPALIVE
    const char* get_meal_json() {
        return meal_json;
    }

    EMSCRIPTEN_KEEPALIVE
    const char* get_profile_json() {
        return profile_json;
    }

    EMSCRIPTEN_KEEPALIVE
    const char* get_dorm_status_json() {
        return dorm_json;
    }