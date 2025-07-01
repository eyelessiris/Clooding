#include <emscripten/emscripten.h>
#include <string.h>
#include <stdio.h>
#include <time.h> // time, localtime, strftime 등을 위해 추가 (실제 시간 사용 시)

// 학생 정보 구조체
typedef struct {
    int studentId;
    const char* password_plain;
    const char* name;
    int classId;
} StudentEntry;

// 학생 데이터 (1301~1318 모두 3반)
StudentEntry all_students[] = {
    {1301, "pass1301", "김서연", 3},
    {1302, "pass1302", "김준표", 3},
    {1303, "pass1303", "김지현", 3},
    {1304, "pass1304", "노민영", 3},
    {1305, "pass1305", "류도호", 3},
    {1306, "pass1306", "송채우", 3},
    {1307, "pass1307", "윤덕영", 3},
    {1308, "pass1308", "윤수연", 3},
    {1309, "pass1309", "이의빈", 3},
    {1310, "pass1310", "이진서", 3},
    {1311, "pass1311", "이찬진", 3},
    {1312, "pass1312", "이하경", 3},
    {1313, "pass1313", "이효은", 3},
    {1314, "pass1314", "임재현", 3},
    {1315, "pass1315", "전준혁", 3},
    {1316, "pass1316", "주현진", 3},
    {1317, "pass1317", "채성원", 3},
    {1318, "pass1318", "천정은", 3}
};
const int NUM_STUDENTS = sizeof(all_students) / sizeof(all_students[0]);

// 시간표 JSON - 요일별 구조 (최신 버전)
static const char schedule_class3_json[] =
    "{\"date\":\"2025.07.01\",\"weekSchedule\":{"
    "\"월\":[{\"period\":1,\"subject\":\"수학\",\"teacher\":\"이주희\"},{\"period\":2,\"subject\":\"과학\",\"teacher\":\"이지영\"},{\"period\":3,\"subject\":\"과학\",\"teacher\":\"이지영\"},{\"period\":4,\"subject\":\"영어\",\"teacher\":\"김선주\"},{\"period\":5,\"subject\":\"체육\",\"teacher\":\"류수일\"},{\"period\":6,\"subject\":\"국어\",\"teacher\":\"서은주\"},{\"period\":7,\"subject\":\"\",\"teacher\":\"\"}],"
    "\"화\":[{\"period\":1,\"subject\":\"수학\",\"teacher\":\"이주희\"},{\"period\":2,\"subject\":\"전일\",\"teacher\":\"박병국\"},{\"period\":3,\"subject\":\"전일\",\"teacher\":\"박병국\"},{\"period\":4,\"subject\":\"체육\",\"teacher\":\"류수일\"},{\"period\":5,\"subject\":\"컴시\",\"teacher\":\"김륜엽\"},{\"period\":6,\"subject\":\"컴시\",\"teacher\":\"김륜엽\"},{\"period\":7,\"subject\":\"과학\",\"teacher\":\"이지영\"}],"
    "\"수\":[{\"period\":1,\"subject\":\"프밍\",\"teacher\":\"박병국\"},{\"period\":2,\"subject\":\"프밍\",\"teacher\":\"박병국\"},{\"period\":3,\"subject\":\"프밍\",\"teacher\":\"박병국\"},{\"period\":4,\"subject\":\"음악\",\"teacher\":\"전지원\"},{\"period\":5,\"subject\":\"창체\",\"teacher\":\"\"},{\"period\":6,\"subject\":\"창체\",\"teacher\":\"\"},{\"period\":7,\"subject\":\"창체\",\"teacher\":\"\"}],"
    "\"목\":[{\"period\":1,\"subject\":\"영어\",\"teacher\":\"김선주\"},{\"period\":2,\"subject\":\"영어\",\"teacher\":\"김선주\"},{\"period\":3,\"subject\":\"창업\",\"teacher\":\"정문정\"},{\"period\":4,\"subject\":\"전일\",\"teacher\":\"박병국\"},{\"period\":5,\"subject\":\"국어\",\"teacher\":\"서은주\"},{\"period\":6,\"subject\":\"음악\",\"teacher\":\"전지원\"},{\"period\":7,\"subject\":\"진로\",\"teacher\":\"신선주\"}],"
    "\"금\":[{\"period\":1,\"subject\":\"컴시\",\"teacher\":\"김륜엽\"},{\"period\":2,\"subject\":\"사물\",\"teacher\":\"이선봉\"},{\"period\":3,\"subject\":\"사물\",\"teacher\":\"이선봉\"},{\"period\":4,\"subject\":\"사물\",\"teacher\":\"이선봉\"},{\"period\":5,\"subject\":\"국어\",\"teacher\":\"서은주\"},{\"period\":6,\"subject\":\"창업\",\"teacher\":\"전문정\"},{\"period\":7,\"subject\":\"수학\",\"teacher\":\"이주희\"}]"
    "}}";

// 급식 JSON (석식만)
static const char meal_json[] =
    "{\"date\":\"2025.07.01\",\"mealType\":\"석식\",\"menu\":\"오늘은 급식이 없습니다.\",\"calories\":0}";

// 기숙사 신청 현황 JSON
static const char dorm_json[] =
    "{\"selfStudy\":{\"current\":0,\"max\":50,\"startTime\":\"20:00\"},"
    "\"massageChair\":{\"current\":0,\"max\":5,\"startTime\":\"20:20\"}}";

// 로그인 함수
EMSCRIPTEN_KEEPALIVE
const char* login_user(int studentId, const char* password) {
    static char result_json[512]; // 충분히 큰 버퍼

    for (int i = 0; i < NUM_STUDENTS; i++) {
        if (all_students[i].studentId == studentId) {
            if (strcmp(all_students[i].password_plain, password) == 0) {
                snprintf(result_json, sizeof(result_json),
                    "{\"status\":\"success\",\"user\":{\"studentId\":%d,\"name\":\"%s\",\"classId\":%d}}",
                    all_students[i].studentId, all_students[i].name, all_students[i].classId);
                return result_json;
            }

            snprintf(result_json, sizeof(result_json),
                     "{\"status\":\"fail\",\"message\":\"비밀번호가 틀렸습니다.\"}");
            return result_json;
        }
    }

    snprintf(result_json, sizeof(result_json),
             "{\"status\":\"fail\",\"message\":\"존재하지 않는 학번입니다.\"}");
    return result_json;
}

// 시간표 조회 (3반만 지원)
EMSCRIPTEN_KEEPALIVE
const char* get_schedule_by_class(int classId) {
    if (classId == 3) {
        return schedule_class3_json;
    }
    return "{\"date\":\"N/A\",\"weekSchedule\":{}}";
}

// 급식 조회
EMSCRIPTEN_KEEPALIVE
const char* get_meal_json() {
    return meal_json;
}

// 기숙사 신청 현황 조회
EMSCRIPTEN_KEEPALIVE
const char* get_dorm_status_json() {
    return dorm_json;
}

// 요일 문자열 반환 함수 추가
EMSCRIPTEN_KEEPALIVE
const char* get_day_of_week_string(int dayIndex) {
    switch(dayIndex) {
        case 0: return "일";
        case 1: return "월";
        case 2: return "화";
        case 3: return "수";
        case 4: return "목";
        case 5: return "금";
        case 6: return "토";
        default: return "";
    }
}