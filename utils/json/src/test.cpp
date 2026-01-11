#include "../inc/Json.h"
#include "utils/log/inc/Log.h"

namespace utils::json {
    static void LogSection(const char* title)
    {
        log::Info("========================================");
        log::Info("TEST SECTION: {}", title);
        log::Info("========================================");
    }

    void TestJsonAllFeatures_Log()
    {
        // ===== 0. 初始化 =====
        // cJSON_Init();
        LogSection("0. Init");

        // =========================================================
        LogSection("1. Root construction & basic object assignment");

        Json j;

        j["int"] = 123;
        j["bool"] = true;
        j["double"] = 3.1415;
        j["string"] = "hello";

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("2. Object overwrite (replace semantics)");

        j["int"] = 999;
        j["string"] = "world";

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("3. Array basic write & random index write");

        j["arr"][0] = 1;
        j["arr"][3] = 3;
        j["arr"][1] = "middle";
        j["arr"][2] = false;   // ← 你之前发生错误的位置

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("4. Array overwrite");

        j["arr"][1] = 100;
        j["arr"][3] = "tail";

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("5. Nested object creation");

        j["obj"]["a"] = 1;
        j["obj"]["b"] = true;
        j["obj"]["c"] = "text";

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("6. Nested object + array");

        j["obj"]["list"][0] = "zero";
        j["obj"]["list"][2] = "two";
        j["obj"]["list"][1] = 1;

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("7. Replace object with array (type switch)");

        j["switch"] = 1;
        j["switch"][0] = "array-now";
        j["switch"][2] = 42;

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("8. Replace array with object (type switch)");

        j["switch"]["x"] = 100;
        j["switch"]["y"] = false;

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("9. Deep nesting stress test");

        j["deep"][0]["a"][1]["b"][2]["c"] = "deep-value";

        log::Info("jsonData: {}", j.dump(true));

        // =========================================================
        LogSection("10. Parse from string");

        const char* jsonText =
            R"({
            "p": 1,
            "q": [10, 20, 30],
            "r": { "x": true }
        })";

        Json parsed;
        bool ok = parsed.parse(jsonText);

        log::Info("jsonData: {}", parsed.dump(true));

        // =========================================================
        LogSection("11. Modify parsed JSON");

        parsed["q"][1] = 999;
        parsed["r"]["y"] = "added";

        log::Info("jsonData: {}", parsed.dump(true));

        // =========================================================
        LogSection("12. Dump compact");

        log::Info("jsonData: {}", parsed.dump(false));

        // =========================================================
        LogSection("13. Reset & clear");

        parsed.reset();
        // assert(!parsed);

        log::Info("jsonData: {}", parsed.dump(true));

        log::Info("[OK] All Json feature tests completed.");
    }
}