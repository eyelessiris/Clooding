#include <emscripten/emscripten.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_SELF_STUDY 50
#define MAX_MASSAGE_CHAIR 5

// 학생 정보 구조체
typedef struct {
    int studentId;
    const char* password_plain;
    const char* name;
    int classId;
} StudentEntry;

// 학생 데이터 (1301~1318 모두 3반)
StudentEntry all_students[] = {
    {1301, "pass1301", "김서연", 3}, {1302, "pass1302", "김준표", 3},
    {1303, "pass1303", "김지현", 3}, {1304, "pass1304", "노민영", 3},
    {1305, "pass1305", "류도호", 3}, {1306, "pass1306", "송채우", 3},
    {1307, "pass1307", "윤덕영", 3}, {1308, "pass1308", "윤수연", 3},
    {1309, "pass1309", "이의빈", 3}, {1310, "pass1310", "이진서", 3},
    {1311, "pass1311", "이찬진", 3}, {1312, "pass1312", "이하경", 3},
    {1313, "pass1313", "이효은", 3}, {1314, "pass1314", "임재현", 3},
    {1315, "pass1315", "전준혁", 3}, {1316, "pass1316", "주현진", 3},
    {1317, "pass1317", "채성원", 3}, {1318, "pass1318", "천정은", 3}
};
const int NUM_STUDENTS = sizeof(all_students) / sizeof(all_students[0]);

// 정적 JSON 응답 버퍼
static char schedule_json_buffer[8192];
static char meal_json_buffer[256];

// 로그인 함수
EMSCRIPTEN_KEEPALIVE
const char* login_user(int studentId, const char* password) {
    static char result_json[512];

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

// 시간표 조회 (3반만 지원, 현재 날짜 포함)
EMSCRIPTEN_KEEPALIVE
const char* get_schedule_by_class(int classId) {
    if (classId != 3) {
        return "{\"date\":\"N/A\",\"weekSchedule\":{}}";
    }

    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char date_str[11];
    strftime(date_str, sizeof(date_str), "%Y.%m.%d", local);

    snprintf(schedule_json_buffer, sizeof(schedule_json_buffer),
        "{\"date\":\"%s\",\"weekSchedule\":{"
        "\"월\":["
        "{\"period\":1,\"subject\":\"수학\",\"teacher\":\"이주희\"},"
        "{\"period\":2,\"subject\":\"과학\",\"teacher\":\"이지영\"},"
        "{\"period\":3,\"subject\":\"과학\",\"teacher\":\"이지영\"},"
        "{\"period\":4,\"subject\":\"영어\",\"teacher\":\"김선주\"},"
        "{\"period\":5,\"subject\":\"체육\",\"teacher\":\"류수일\"},"
        "{\"period\":6,\"subject\":\"국어\",\"teacher\":\"서은주\"},"
        "{\"period\":7,\"subject\":\"\",\"teacher\":\"\"}"
        "],"
        "\"화\":["
        "{\"period\":1,\"subject\":\"수학\",\"teacher\":\"이주희\"},"
        "{\"period\":2,\"subject\":\"전일\",\"teacher\":\"박병국\"},"
        "{\"period\":3,\"subject\":\"전일\",\"teacher\":\"박병국\"},"
        "{\"period\":4,\"subject\":\"체육\",\"teacher\":\"류수일\"},"
        "{\"period\":5,\"subject\":\"컴시\",\"teacher\":\"김륜엽\"},"
        "{\"period\":6,\"subject\":\"컴시\",\"teacher\":\"김륜엽\"},"
        "{\"period\":7,\"subject\":\"과학\",\"teacher\":\"이지영\"}"
        "],"
        "\"수\":["
        "{\"period\":1,\"subject\":\"프밍\",\"teacher\":\"박병국\"},"
        "{\"period\":2,\"subject\":\"프밍\",\"teacher\":\"박병국\"},"
        "{\"period\":3,\"subject\":\"프밍\",\"teacher\":\"박병국\"},"
        "{\"period\":4,\"subject\":\"음악\",\"teacher\":\"전지원\"},"
        "{\"period\":5,\"subject\":\"창체\",\"teacher\":\"\"},"
        "{\"period\":6,\"subject\":\"창체\",\"teacher\":\"\"},"
        "{\"period\":7,\"subject\":\"창체\",\"teacher\":\"\"}"
        "],"
        "\"목\":["
        "{\"period\":1,\"subject\":\"영어\",\"teacher\":\"김선주\"},"
        "{\"period\":2,\"subject\":\"영어\",\"teacher\":\"김선주\"},"
        "{\"period\":3,\"subject\":\"창업\",\"teacher\":\"정문정\"},"
        "{\"period\":4,\"subject\":\"전일\",\"teacher\":\"박병국\"},"
        "{\"period\":5,\"subject\":\"국어\",\"teacher\":\"서은주\"},"
        "{\"period\":6,\"subject\":\"음악\",\"teacher\":\"전지원\"},"
        "{\"period\":7,\"subject\":\"진로\",\"teacher\":\"신선주\"}"
        "],"
        "\"금\":["
        "{\"period\":1,\"subject\":\"컴시\",\"teacher\":\"김륜엽\"},"
        "{\"period\":2,\"subject\":\"사물\",\"teacher\":\"이선봉\"},"
        "{\"period\":3,\"subject\":\"사물\",\"teacher\":\"이선봉\"},"
        "{\"period\":4,\"subject\":\"사물\",\"teacher\":\"이선봉\"},"
        "{\"period\":5,\"subject\":\"국어\",\"teacher\":\"서은주\"},"
        "{\"period\":6,\"subject\":\"창업\",\"teacher\":\"전문정\"},"
        "{\"period\":7,\"subject\":\"수학\",\"teacher\":\"이주희\"}"
        "]"
        "}}", date_str
    );

    return schedule_json_buffer;
}


// 급식 조회
EMSCRIPTEN_KEEPALIVE
const char* get_meal_json() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char date_str[11];
    strftime(date_str, sizeof(date_str), "%Y.%m.%d", local);

    snprintf(meal_json_buffer, sizeof(meal_json_buffer),
        "{\"date\":\"%s\",\"mealType\":\"석식\",\"menu\":\"오늘은 급식이 없습니다.\",\"calories\":0}",
        date_str);

    return meal_json_buffer;
}

// 요일 문자열 반환
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

typedef struct {
    int studentId;
    char name[32];
} Applicant;

static Applicant selfStudyApplicants[MAX_SELF_STUDY];
static int selfStudyCount = 0;
static Applicant massageChairApplicants[MAX_MASSAGE_CHAIR];
static int massageChairCount = 0;


EMSCRIPTEN_KEEPALIVE
int get_self_study_count() {
    return selfStudyCount;
}

EMSCRIPTEN_KEEPALIVE
int get_massage_chair_count() {
    return massageChairCount;
}

EMSCRIPTEN_KEEPALIVE
const char* get_dorm_status_json() {
    static char dorm_json_buffer[256];
    snprintf(dorm_json_buffer, sizeof(dorm_json_buffer),
        "{\"selfStudy\":{\"current\":%d,\"max\":%d,\"startTime\":\"20:00\"},"
        "\"massageChair\":{\"current\":%d,\"max\":%d,\"startTime\":\"20:20\"}}",
        selfStudyCount, MAX_SELF_STUDY, massageChairCount, MAX_MASSAGE_CHAIR);
    return dorm_json_buffer;
}

EMSCRIPTEN_KEEPALIVE
const char* get_self_study_applicants_json() {
    static char buffer[4096];
    char temp[128];
    strcpy(buffer, "[");
    for (int i = 0; i < selfStudyCount; i++) {
        snprintf(temp, sizeof(temp),
            "{\"studentId\":%d,\"name\":\"%s\"}%s",
            selfStudyApplicants[i].studentId,
            selfStudyApplicants[i].name,
            (i == selfStudyCount -1) ? "" : ",");
        strcat(buffer, temp);
    }
    strcat(buffer, "]");
    return buffer;
}

EMSCRIPTEN_KEEPALIVE
const char* get_massage_chair_applicants_json() {
    static char buffer[2048];
    char temp[128];
    strcpy(buffer, "[");
    for (int i = 0; i < massageChairCount; i++) {
        snprintf(temp, sizeof(temp),
            "{\"studentId\":%d,\"name\":\"%s\"}%s",
            massageChairApplicants[i].studentId,
            massageChairApplicants[i].name,
            (i == massageChairCount -1) ? "" : ",");
        strcat(buffer, temp);
    }
    strcat(buffer, "]");
    return buffer;
}

EM_JS(void, apply_self_study_js, (int studentId, const char* name), {
  const jsName = UTF8ToString(name);
  fetch("http://localhost:3000/apply/self-study", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ studentId: studentId, name: jsName }),
  }).then(res => res.json())
    .then(data => {
      alert(data.message);
    })
    .catch(() => {
      alert("서버 연결에 실패했습니다.");
    });
});

EM_JS(void, apply_massage_chair_js, (int studentId, const char* name), {
  const jsName = UTF8ToString(name);
  fetch("http://localhost:3000/apply/massage-chair", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ studentId: studentId, name: jsName }),
  }).then(res => res.json())
    .then(data => {
      alert(data.message);
    })
    .catch(() => {
      alert("서버 연결에 실패했습니다.");
    });
});

EMSCRIPTEN_KEEPALIVE
void apply_self_study(int studentId, const char* name) {
  apply_self_study_js(studentId, name);
}

EMSCRIPTEN_KEEPALIVE
void apply_massage_chair(int studentId, const char* name) {
  apply_massage_chair_js(studentId, name);
}
