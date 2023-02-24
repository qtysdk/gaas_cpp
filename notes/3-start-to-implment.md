## 實作開始遊戲

在動手實作前，我們先想一下要怎麼跟 Game Server 說想開始，例如打一個 `HTTP POST` 給它，再跟他說玩家的名稱呢？

```jsx
POST /guess_number_game:start

{"player_name": "56不能亡"}
```

看起來這已經夠用了，反正它是簡單的單人遊戲，有人要來玩自己就可以開始了，頂多是讓系統知道玩家的暱稱叫什麼而已。那我們期過系統回應的就是 Read Model 相關的內容了，猜測的記錄先不要多想，就放個空的 List 唄：

```jsx
{"game_id": "", "player_name": "", "history": []}
```

明白主要目標後，我們先不要多想，單純把 Controller 建出來就行了：

- 一組 Controller
- 一組 Test Case

有了先前的經驗後，行動方案相當明確：

- 先用 `drogon_ctl` 生出 Controller
- 把 Controller 加入編譯清單
- 複製一份 Test Case 確定 Controller 有被測試到
- 修改 Test Case 單純讓它 pass 就行，因為我們還沒有實作真的邏輯 (你要保持紅燈也沒有問題)

```diff
diff --git a/CMakeLists.txt b/CMakeLists.txt
index cf823fc..7597d03 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -19,7 +19,7 @@ endif ()
 set(CMAKE_CXX_STANDARD_REQUIRED ON)
 set(CMAKE_CXX_EXTENSIONS OFF)
 
-add_executable(${PROJECT_NAME} main.cc NothingCtrl.cc)
+add_executable(${PROJECT_NAME} main.cc NothingCtrl.cc CreateGameCtrl.cc)
 
 # ##############################################################################
 # If you include the drogon source code locally in your project, use this method
diff --git a/CreateGameCtrl.cc b/CreateGameCtrl.cc
new file mode 100644
index 0000000..1ee7579
--- /dev/null
+++ b/CreateGameCtrl.cc
@@ -0,0 +1,13 @@
+#include "CreateGameCtrl.h"
+
+void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
+{
+    //write your application logic here
+    auto resp=HttpResponse::newHttpResponse();
+    //NOTE: The enum constant below is named "k200OK" (as in 200 OK), not "k2000K".
+    resp->setStatusCode(k200OK);
+    resp->setContentTypeCode(CT_TEXT_HTML);
+    resp->setBody("{\"hello\": \"world\"}");
+    resp->setContentTypeCode(CT_APPLICATION_JSON);
+    callback(resp);
+}
diff --git a/CreateGameCtrl.h b/CreateGameCtrl.h
new file mode 100644
index 0000000..f09af68
--- /dev/null
+++ b/CreateGameCtrl.h
@@ -0,0 +1,15 @@
+#pragma once
+
+#include <drogon/HttpSimpleController.h>
+
+using namespace drogon;
+
+class CreateGameCtrl : public drogon::HttpSimpleController<CreateGameCtrl>
+{
+  public:
+    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
+    PATH_LIST_BEGIN
+    // list path definitions here;
+    PATH_ADD("/guess_number_game:start", Post);
+    PATH_LIST_END
+};
diff --git a/test/CMakeLists.txt b/test/CMakeLists.txt
index 6f53151..61a69b4 100644
--- a/test/CMakeLists.txt
+++ b/test/CMakeLists.txt
@@ -1,7 +1,7 @@
 cmake_minimum_required(VERSION 3.5)
 project(gaas_cpp_test CXX)
 
-add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc)
+add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc ../CreateGameCtrl.cc)
 
 # ##############################################################################
 # If you include the drogon source code locally in your project, use this method
diff --git a/test/test_main.cc b/test/test_main.cc
index 0a26ce3..ebf5e37 100644
--- a/test/test_main.cc
+++ b/test/test_main.cc
@@ -4,7 +4,7 @@
 
 using namespace drogon;
 
-DROGON_TEST(RemoteAPITest)
+DROGON_TEST(NothingAPITest)
 {
     auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
     auto req = HttpRequest::newHttpRequest();
@@ -21,11 +21,26 @@ DROGON_TEST(RemoteAPITest)
 
 }
 
-DROGON_TEST(BasicTest)
+DROGON_TEST(CreateGameAPITest)
 {
-    // Add your tests here
+    auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
+    auto req = HttpRequest::newHttpRequest();
+    req->setPath("/guess_number_game:start");
+    req->setMethod(HttpMethod::Post);
+    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp) {
+        // There's nothing we can do if the request didn't reach the server
+        // or the server generated garbage.
+        REQUIRE(res == ReqResult::Ok);
+        REQUIRE(resp != nullptr);
+
+        CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
+        CHECK(resp->contentType() == CT_APPLICATION_JSON);
+    });
+
 }
 
+
+
 int main(int argc, char** argv) 
 {
     using namespace drogon;
```

做完了「殻」之後呢？就可以來寫跟遊戲比較相關的部分了。若是以 Clean Architecture 為偏好的話，我們接著得做：

- Controller → UseCase
- UseCase → Repository
- Repository → Domain Model

製作一個 UseCase 來 `查改存推` 我們的 Domain Model，如果想早一點關心到 `I/O` 那 Repository 也可以提早做。即便，此時此刻還沒有多想要不要弄 Database 或 Storage，但它的「意圖」可以先透過 Repository 表現出來。接著 Repository 是真正管理到 Domain Model 生命週期變化的人，在這段就得具體實作出相關的 Domain Model 囉。

簡而言之，接下來要做的事情就是「正式版的 Walking skeleton」，在前一版的 Walking skeleton 是單純在試驗 Tech Stack 能不能打通的情況。接著我要做第 1 個簡單的 Happy Path：**玩家開始遊戲的 walking skeleton**。