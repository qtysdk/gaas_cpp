## 開始研究 ATDD 基礎建設部分

在 Drogon 專案的 Wiki Page 中有介紹他的 [Testing Framework](https://github.com/drogonframework/drogon/wiki/ENG-18-Testing-Framework)。若是把它複製到透過 `dragon_ctl` 建好的 `test/test_main.cc` 內，是無法直接使用的。因為除了 `namespace` 沒有適當的引用，還有缺少了 method call 的 `()` 外，在 Web Application 啟動的部分，缺少了 Listener 無法接收外來的 HTTP Request。

為了達成第一個 end-to-end test，我做了下面的修改，摘要如下：

- NothingCtrl 改回傳 JSON 格式
- 在 test 的 CMakeLists.txt 將 NothingCtrl 加入 compile 清單
- [由 Wiki 中複製 `RemoteAPITest` 範例](https://github.com/drogonframework/drogon/wiki/ENG-18-Testing-Framework#asynchronous-testing)，並修正無法編譯的內容
- 追加 `addListener` 與指定要使用的 port
- 修改 Http Client 連線位置，由 `localhost` 改成 `127.0.0.1`（因為實測沒有自動 resolve domain name）。

```diff
diff --git a/NothingCtrl.cc b/NothingCtrl.cc
index bf8c458..6b0a6de 100644
--- a/NothingCtrl.cc
+++ b/NothingCtrl.cc
@@ -7,6 +7,7 @@ void NothingCtrl::asyncHandleHttpRequest(const HttpRequestPtr& req, std::functio
     //NOTE: The enum constant below is named "k200OK" (as in 200 OK), not "k2000K".
     resp->setStatusCode(k200OK);
     resp->setContentTypeCode(CT_TEXT_HTML);
-    resp->setBody("Hello World!");
+    resp->setBody("{\"hello\": \"world\"}");
+    resp->setContentTypeCode(CT_APPLICATION_JSON);
     callback(resp);
 }
diff --git a/test/CMakeLists.txt b/test/CMakeLists.txt
index 8d9b0f9..6f53151 100644
--- a/test/CMakeLists.txt
+++ b/test/CMakeLists.txt
@@ -1,7 +1,7 @@
 cmake_minimum_required(VERSION 3.5)
 project(gaas_cpp_test CXX)
 
-add_executable(${PROJECT_NAME} test_main.cc)
+add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc)
 
 # ##############################################################################
 # If you include the drogon source code locally in your project, use this method
diff --git a/test/test_main.cc b/test/test_main.cc
index 7370b29..0a26ce3 100644
--- a/test/test_main.cc
+++ b/test/test_main.cc
@@ -2,6 +2,25 @@
 #include <drogon/drogon_test.h>
 #include <drogon/drogon.h>
 
+using namespace drogon;
+
+DROGON_TEST(RemoteAPITest)
+{
+    auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
+    auto req = HttpRequest::newHttpRequest();
+    req->setPath("/");
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
+}
+
 DROGON_TEST(BasicTest)
 {
     // Add your tests here
@@ -18,6 +37,7 @@ int main(int argc, char** argv)
     std::thread thr([&]() {
         // Queues the promise to be fulfilled after starting the loop
         app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
+        app().addListener("127.0.0.1", 8848);
         app().run();
     });
```

## ATDD 前哨站

到目前為止，我們做出了第 1 隻 Controller 與它的「單蠢」測試案例。若要更進一步，讓這個流程方便，有幾個地方可以改善：

- 怎麼更方便地處理 JSON 呢？在 Controller 內要 encode 成字串，在 Test Case 內要 decode 回 model
- 當新增 Controller 時，怎麼自動去維護二組的 `CMakeLists.txt`？或是這個問題不嚴重，我們不會一直新增 Controller。

第 1 個問題看起來比較值得先搞定，因為它出現的頻率，會比加新的 Controller 要來得多！目前，我們可以放在心上就好，等真的要處理 Model 的輸出再來處理。

第 2 個問題就是 DX (developer experience) 的問題了，儘管在這篇文章中的實驗都是卡在「新增 Controller」，但除了 Controller 本身，我們還有因實作需要的 Domain Model 或是 Use Case，當然還會有各種 Utilities 得照顧到。

目前是「正式開工」前，直接感受到的開發不便之處。但就先記在帳上，在不影響時程的情況下，適時地欠債有助專案發展。