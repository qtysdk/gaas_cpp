## GuessNumber Use Case

完成開局之後，接著得實作讓玩家猜數字的功能。我們來實作 GuessNumber UseCase 唄！本來想使用 `dragon_ctl` 再生一個 Controller 的檔，但冷靜想了一下。其實，我可以在同一個檔案內加新的 Controller 就好！乾脆就這麼做吧！於是，也順手改名為 `GameCtrls`：

```diff
diff --git a/CMakeLists.txt b/CMakeLists.txt
index 7f01bb7..62b7ce7 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -19,7 +19,7 @@ endif ()
 set(CMAKE_CXX_STANDARD_REQUIRED ON)
 set(CMAKE_CXX_EXTENSIONS OFF)
 
-add_executable(${PROJECT_NAME} main.cc NothingCtrl.cc CreateGameCtrl.cc CreateGameUseCase.cc GameRepository.cc Models.cpp)
+add_executable(${PROJECT_NAME} main.cc NothingCtrl.cc GameCtrls.cc CreateGameUseCase.cc GameRepository.cc Models.cpp)
 
 
 # add dependencies
diff --git a/CreateGameCtrl.cc b/CreateGameCtrl.cc
deleted file mode 100644
index 36a1cee..0000000
--- a/CreateGameCtrl.cc
+++ /dev/null
@@ -1,24 +0,0 @@
-#include "CreateGameCtrl.h"
-#include "CreateGameUseCase.h"
-#include <nlohmann/json.hpp>
-#include "GameRepository.h"
-
-using json = nlohmann::json;
-
-
-
-void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
-                                            std::function<void(const HttpResponsePtr &)> &&callback) {
-    CreateGameUseCase uc;
-    Output output;
-
-    auto data = json::parse(req->getBody());
-    uc.execute(Input(data["player_name"]), output);
-
-    auto resp = HttpResponse::newHttpResponse();
-    resp->setStatusCode(k200OK);
-    resp->setContentTypeCode(CT_TEXT_HTML);
-    resp->setBody(output.to_json());
-    resp->setContentTypeCode(CT_APPLICATION_JSON);
-    callback(resp);
-}
diff --git a/CreateGameCtrl.h b/CreateGameCtrl.h
deleted file mode 100644
index f09af68..0000000
--- a/CreateGameCtrl.h
+++ /dev/null
@@ -1,15 +0,0 @@
-#pragma once
-
-#include <drogon/HttpSimpleController.h>
-
-using namespace drogon;
-
-class CreateGameCtrl : public drogon::HttpSimpleController<CreateGameCtrl>
-{
-  public:
-    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
-    PATH_LIST_BEGIN
-    // list path definitions here;
-    PATH_ADD("/guess_number_game:start", Post);
-    PATH_LIST_END
-};
diff --git a/GameCtrls.cc b/GameCtrls.cc
new file mode 100644
index 0000000..142dac3
--- /dev/null
+++ b/GameCtrls.cc
@@ -0,0 +1,36 @@
+#include "GameCtrls.h"
+#include "CreateGameUseCase.h"
+#include <nlohmann/json.hpp>
+#include "GameRepository.h"
+
+using json = nlohmann::json;
+
+
+void GameCtrls::asyncHandleHttpRequest(const HttpRequestPtr &req,
+                                       std::function<void(const HttpResponsePtr &)> &&callback) {
+    CreateGameUseCase uc;
+    Output output;
+
+    auto data = json::parse(req->getBody());
+    uc.execute(Input(data["player_name"]), output);
+
+    auto resp = HttpResponse::newHttpResponse();
+    resp->setStatusCode(k200OK);
+    resp->setContentTypeCode(CT_TEXT_HTML);
+    resp->setBody(output.to_json());
+    resp->setContentTypeCode(CT_APPLICATION_JSON);
+    callback(resp);
+}
+
+void GuessNumberCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
+                                             std::function<void(const HttpResponsePtr &)> &&callback) {
+
+    // TODO invoke use-case here
+
+    auto resp = HttpResponse::newHttpResponse();
+    resp->setStatusCode(k200OK);
+    resp->setContentTypeCode(CT_TEXT_HTML);
+    resp->setBody("{}");
+    resp->setContentTypeCode(CT_APPLICATION_JSON);
+    callback(resp);
+}
diff --git a/GameCtrls.h b/GameCtrls.h
new file mode 100644
index 0000000..b51950b
--- /dev/null
+++ b/GameCtrls.h
@@ -0,0 +1,25 @@
+#pragma once
+
+#include <drogon/HttpSimpleController.h>
+
+using namespace drogon;
+
+class GameCtrls : public drogon::HttpSimpleController<GameCtrls> {
+public:
+    void
+    asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) override;
+
+    PATH_LIST_BEGIN
+        PATH_ADD("/guess_number_game:start", Post);
+    PATH_LIST_END
+};
+
+class GuessNumberCtrl : public drogon::HttpSimpleController<GameCtrls> {
+public:
+    void
+    asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) override;
+
+    PATH_LIST_BEGIN
+        PATH_ADD("/guess_number_game:guess", Post);
+    PATH_LIST_END
+};
diff --git a/test/CMakeLists.txt b/test/CMakeLists.txt
index 4c864a9..5ac3a4d 100644
--- a/test/CMakeLists.txt
+++ b/test/CMakeLists.txt
@@ -1,7 +1,7 @@
 cmake_minimum_required(VERSION 3.5)
 project(gaas_cpp_test CXX)
 
-add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc ../CreateGameCtrl.cc ../CreateGameUseCase.cc ../GameRepository.cc ../Models.cpp)
+add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc ../GameCtrls.cc ../CreateGameUseCase.cc ../GameRepository.cc ../Models.cpp)
 
 # add dependencies
 include(../cmake/CPM.cmake)
```

接著，來實作 UseCase 囉 。。。中間有諸多的修改就先略過，直接來看一下目前的 Test Case：

- 成功 `建立遊戲`
- 第 1 次猜數字，就 `碰巧` 猜到底牌。獲得 `4A0B`。

```cpp
DROGON_TEST(GameAPITest) {

    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;

    std::string gameId = "";
    {
        auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
        auto req = HttpRequest::newHttpRequest();
        req->setMethod(HttpMethod::Post);

        req->setPath("/guess_number_game:start");
        auto createGameData = json{{"player_name", "I have no name"}};
        req->setBody(createGameData.dump());

        client->sendRequest(req, [TEST_CTX, &gameId, &mtx, &finished, &cv](ReqResult res, const HttpResponsePtr &resp) {
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);

            CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
            CHECK(resp->contentType() == CT_APPLICATION_JSON);

            auto result = json::parse(resp->getBody());
            auto game = gameRepository.findGameById(result["game_id"]);

            CHECK("I have no name" == result["player_name"]);
            CHECK("I have no name" == game->playerName);
            CHECK(result["history"].empty());

            std::unique_lock<std::mutex> lock(mtx);
            finished = true;
            cv.notify_all();
            gameId = game->id;
        });
    }

    // Wait for the callback to complete
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]() { return finished; });

    {
        auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
        auto req = HttpRequest::newHttpRequest();
        req->setMethod(HttpMethod::Post);

        req->setPath("/guess_number_game:guess");
        auto guessNumberData = json{{"game_id", gameId},
                                    {"number",  1234}};

        req->setBody(guessNumberData.dump());
        client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr &resp) {
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);

            CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
            CHECK(resp->contentType() == CT_APPLICATION_JSON);

            auto result = json::parse(resp->getBody());
            auto game = gameRepository.findGameById(result["game_id"]);
            CHECK(result["history"].size() == 1);

            auto expected = json{{"guess", 1234}, {"respond", "4A0B"}};
            CHECK(expected == result["history"][0]);
        });
    }

}
```

## 實作出牌心得：感受到 forces 了！！！

### 語言特性不熟而引發的慘案：解構子太活躍了

在實作到「出牌」的過程中，糟遇了巨大的亂流，有多大？[請看 commit log](https://github.com/qtysdk/gaas_cpp/commit/b17d35c32e96cb0ceb4395a7caeec5b727872fa5)。主要的原因是，當 Game 物件到處流通至專案中後，Test Case 一直無法測試，因為一直遇到了這樣的 Error：

```cpp
pointer being freed was not allocated
set a breakpoint in malloc_error_break to debug
```

發現案情不單純，上網查了一下也問了 ChatGPT。得到一個結論，因為 C++ 將變數透過 Function 傳遞時，會做一次 Copy 的動作。要避免這個動作，可以用 pointer 或 reference 來取單單純的 value variable。

那為什麼這個 Copy 的動作會影響程式呢？因為一個類別的解構子，有二個主要的呼叫時機：

- 被 `delete` operator 呼叫
- 該變數的 scope 由 stack frame 中 pop 出去 (out-of-scope)

我遇到的情況是 Function Call 結束，所以要 pop `stack frame` 而觸發了解構子。由於這個原因，我一個生命週期還不應該結束的 `Game` 物件，被反覆 `free` 多次促發了 crash。在這個巨大的亂流中，就是在將 Game 物件的傳遞，都依賴 `指標` 來進行。避免因為 out-of-scope 觸發解構子。

心得：語言不熟是最大的 force 啊 (吶喊)

### 測試框架不熟而發生的失敗

如果有仔細看上面的 Test Case，會發現我在二組 HTTP Request / HTTP Response 之間，做了一個等待的動作。主要是用 `mutex`：

```cpp
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;
```

會這麼寫是因為，參考範例而來的 HTTP Client 用法，它是在 async context 下活動的。二組會「同時出發，誰也不讓誰」，但我是希望它要依序執行的，因為我在模擬 `建立遊戲` 與 `猜數字` 的正常玩家互動的流程。暫時還不確定如例修改它，先依直覺上網查了讓它等待的寫法。後續如果知道了優化的方式，再來改它唄。
