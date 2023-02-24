## CreateGame UseCase 開局

```diff
diff --git a/CMakeLists.txt b/CMakeLists.txt
index 7597d03..300067e 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -19,7 +19,7 @@ endif ()
 set(CMAKE_CXX_STANDARD_REQUIRED ON)
 set(CMAKE_CXX_EXTENSIONS OFF)
 
-add_executable(${PROJECT_NAME} main.cc NothingCtrl.cc CreateGameCtrl.cc)
+add_executable(${PROJECT_NAME} main.cc NothingCtrl.cc CreateGameCtrl.cc CreateGameUseCase.cc)
 
 # ##############################################################################
 # If you include the drogon source code locally in your project, use this method
diff --git a/CreateGameCtrl.cc b/CreateGameCtrl.cc
index 1ee7579..8f8b5fc 100644
--- a/CreateGameCtrl.cc
+++ b/CreateGameCtrl.cc
@@ -1,13 +1,19 @@
 #include "CreateGameCtrl.h"
+#include "CreateGameUseCase.h"
 
-void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
-{
-    //write your application logic here
-    auto resp=HttpResponse::newHttpResponse();
-    //NOTE: The enum constant below is named "k200OK" (as in 200 OK), not "k2000K".
+
+void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
+                                            std::function<void(const HttpResponsePtr &)> &&callback) {
+    CreateGameUseCase uc;
+    Output output;
+
+    // TODO extract payload from json body
+    uc.execute(Input(req->bodyData()), output);
+
+    auto resp = HttpResponse::newHttpResponse();
     resp->setStatusCode(k200OK);
     resp->setContentTypeCode(CT_TEXT_HTML);
-    resp->setBody("{\"hello\": \"world\"}");
+    resp->setBody(output.to_json());
     resp->setContentTypeCode(CT_APPLICATION_JSON);
     callback(resp);
 }
diff --git a/CreateGameUseCase.cc b/CreateGameUseCase.cc
new file mode 100644
index 0000000..35ccc37
--- /dev/null
+++ b/CreateGameUseCase.cc
@@ -0,0 +1,15 @@
+#include "CreateGameUseCase.h"
+
+using namespace std;
+
+Input::Input(string player_name) {
+    this->player_name = player_name;
+}
+
+void CreateGameUseCase::execute(Input input, Output output) {
+
+}
+
+string Output::to_json() {
+    return "";
+}
diff --git a/CreateGameUseCase.h b/CreateGameUseCase.h
new file mode 100644
index 0000000..1ba1494
--- /dev/null
+++ b/CreateGameUseCase.h
@@ -0,0 +1,25 @@
+#pragma once
+
+#include <string>
+
+using namespace std;
+
+class Input {
+
+public:
+    string player_name;
+
+    Input(string player_name);
+};
+
+class Output {
+
+public:
+    string to_json();
+};
+
+class CreateGameUseCase {
+
+public:
+    void execute(Input input, Output output);
+};
diff --git a/test/CMakeLists.txt b/test/CMakeLists.txt
index 61a69b4..b8efdb0 100644
--- a/test/CMakeLists.txt
+++ b/test/CMakeLists.txt
@@ -1,7 +1,7 @@
 cmake_minimum_required(VERSION 3.5)
 project(gaas_cpp_test CXX)
 
-add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc ../CreateGameCtrl.cc)
+add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc ../CreateGameCtrl.cc ../CreateGameUseCase.cc)
 
 # ##############################################################################
 # If you include the drogon source code locally in your project, use this method
diff --git a/test/test_main.cc b/test/test_main.cc
index ebf5e37..dc7ddd6 100644
--- a/test/test_main.cc
+++ b/test/test_main.cc
@@ -1,48 +1,45 @@
 #define DROGON_TEST_MAIN
+
 #include <drogon/drogon_test.h>
 #include <drogon/drogon.h>
 
 using namespace drogon;
 
-DROGON_TEST(NothingAPITest)
-{
+DROGON_TEST(NothingAPITest) {
     auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
     auto req = HttpRequest::newHttpRequest();
     req->setPath("/");
-    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp) {
-        // There's nothing we can do if the request didn't reach the server
-        // or the server generated garbage.
+    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr &resp) {
         REQUIRE(res == ReqResult::Ok);
         REQUIRE(resp != nullptr);
 
         CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
         CHECK(resp->contentType() == CT_APPLICATION_JSON);
+
+        // TODO check the response in that format
+        // {"game_id": "", "player_name": "", "history": ""}
     });
 
 }
 
-DROGON_TEST(CreateGameAPITest)
-{
+DROGON_TEST(CreateGameAPITest) {
     auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
     auto req = HttpRequest::newHttpRequest();
     req->setPath("/guess_number_game:start");
     req->setMethod(HttpMethod::Post);
-    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp) {
-        // There's nothing we can do if the request didn't reach the server
-        // or the server generated garbage.
+    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr &resp) {
         REQUIRE(res == ReqResult::Ok);
         REQUIRE(resp != nullptr);
 
         CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
         CHECK(resp->contentType() == CT_APPLICATION_JSON);
+
     });
 
 }
 
 
-
-int main(int argc, char** argv) 
-{
+int main(int argc, char **argv) {
     using namespace drogon;
 
     std::promise<void> p1;
```

在這回的修改中，我們新增 `CreateGameUseCase` 類別跟它相依的 Input 與 Output 類別。Input 如同我們先前的設計，需要接收 `player_name` 進來開始遊戲，而 Output 類別則應該回應 Read Model 期望看到的狀態。

目前都只是先把應用的架勢擺出來，我們還沒有真的開始實作功能。開始放上邏輯前，先處理一下 JSON 的問題唄！特別是想試看看在 Dragon 安裝頁面提到的 [CMake Dependency Manager：CPM](https://github.com/cpm-cmake/CPM.cmake)。

詢問了 Google 與 ChatGPT 都推薦的 JSON Library 都有 `nlohmann/json` 那就來用它吧！

- [https://ithelp.ithome.com.tw/articles/10223947](https://ithelp.ithome.com.tw/articles/10223947)
- [https://github.com/nlohmann/json](https://github.com/nlohmann/json)

加入 JSON Library 後，我們可以做到：

- 在 Controller 內，把 HTTP request body 的資料轉成 JSON 物件，正確地初始化 Input。
- 將 Input 塞入 UseCase 後，…[經過了應該要實作的邏輯 (TODO)]…，Output 可以再將它轉成 JSON 放入 HTTP response body。
- 實作的過程中，也順手修改了測試案例，讓它可以查驗 Server 傳回的內容。

```diff
diff --git a/CreateGameCtrl.cc b/CreateGameCtrl.cc
index 8f8b5fc..1ea618f 100644
--- a/CreateGameCtrl.cc
+++ b/CreateGameCtrl.cc
@@ -1,14 +1,16 @@
 #include "CreateGameCtrl.h"
 #include "CreateGameUseCase.h"
+#include <nlohmann/json.hpp>
 
+using json = nlohmann::json;
 
 void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                             std::function<void(const HttpResponsePtr &)> &&callback) {
     CreateGameUseCase uc;
     Output output;
 
-    // TODO extract payload from json body
-    uc.execute(Input(req->bodyData()), output);
+    auto data = json::parse(req->getBody());
+    uc.execute(Input(data["player_name"]), output);
 
     auto resp = HttpResponse::newHttpResponse();
     resp->setStatusCode(k200OK);
diff --git a/CreateGameUseCase.cc b/CreateGameUseCase.cc
index 35ccc37..bfcfb95 100644
--- a/CreateGameUseCase.cc
+++ b/CreateGameUseCase.cc
@@ -1,15 +1,21 @@
 #include "CreateGameUseCase.h"
+#include <nlohmann/json.hpp>
 
 using namespace std;
+using json = nlohmann::json;
 
-Input::Input(string player_name) {
-    this->player_name = player_name;
+Input::Input(string playerName) {
+    this->playerName = playerName;
 }
 
-void CreateGameUseCase::execute(Input input, Output output) {
-
+void CreateGameUseCase::execute(Input input, Output &output) {
+    output.gameId = "5566";
+    output.playerName = input.playerName;
 }
 
 string Output::to_json() {
-    return "";
+    // TODO add real history
+    return json{{"game_id",     this->gameId},
+                {"player_name", this->playerName},
+    }.dump();
 }
diff --git a/CreateGameUseCase.h b/CreateGameUseCase.h
index 1ba1494..9ae95b4 100644
--- a/CreateGameUseCase.h
+++ b/CreateGameUseCase.h
@@ -1,25 +1,33 @@
 #pragma once
 
 #include <string>
+#include <list>
 
 using namespace std;
 
 class Input {
 
 public:
-    string player_name;
+    string playerName;
 
-    Input(string player_name);
+    Input(string playerName);
 };
 
 class Output {
 
+
 public:
+    string gameId;
+    string playerName;
+
+//    TODO define the history entry
+//    list<Record> history;
+
     string to_json();
 };
 
 class CreateGameUseCase {
 
 public:
-    void execute(Input input, Output output);
+    void execute(Input input, Output &output);
 };
diff --git a/test/test_main.cc b/test/test_main.cc
index dc7ddd6..9b9e8e3 100644
--- a/test/test_main.cc
+++ b/test/test_main.cc
@@ -2,8 +2,10 @@
 
 #include <drogon/drogon_test.h>
 #include <drogon/drogon.h>
+#include <nlohmann/json.hpp>
 
 using namespace drogon;
+using json = nlohmann::json;
 
 DROGON_TEST(NothingAPITest) {
     auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
@@ -17,7 +19,7 @@ DROGON_TEST(NothingAPITest) {
         CHECK(resp->contentType() == CT_APPLICATION_JSON);
 
         // TODO check the response in that format
-        // {"game_id": "", "player_name": "", "history": ""}
+        // {"game_id": "", "playerName": "", "history": ""}
     });
 
 }
@@ -25,8 +27,13 @@ DROGON_TEST(NothingAPITest) {
 DROGON_TEST(CreateGameAPITest) {
     auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
     auto req = HttpRequest::newHttpRequest();
+
     req->setPath("/guess_number_game:start");
     req->setMethod(HttpMethod::Post);
+
+    auto data = json{{"player_name", "I have no name"}};
+    req->setBody(data.dump());
+
     client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr &resp) {
         REQUIRE(res == ReqResult::Ok);
         REQUIRE(resp != nullptr);
@@ -34,6 +41,9 @@ DROGON_TEST(CreateGameAPITest) {
         CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
         CHECK(resp->contentType() == CT_APPLICATION_JSON);
 
+        auto result = json::parse(resp->getBody());
+        CHECK("5566" == result["game_id"]);
+        CHECK("I have no name" == result["player_name"]);
     });
 
 }
```