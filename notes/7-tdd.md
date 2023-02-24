## 這是我最後的波紋：TDD

經歷了上回的大亂流後，我們還是要重振旗鼓。把 `answer` 自動產生的部分做完。而這部分，別是「玩家 (aka 使用者)」不會直接用的部分。相關的實作會有：

- 謎底的產生器
- 數字合法性的驗證 (可以用在 answer 檢查，或玩家猜的數字的檢查)
- 產生 `1A2B` 這樣回應的函式

在使用者可由外部感知的部分，我們由 ATDD 來確認結果可以被接受，但內部的部分我們就跑 TDD 囉！

### Respond 的測試案例

```cpp
DROGON_TEST(GAME_RespondTest) {

    CHECK("4A0B" == createRespond(2345, 2345));
    CHECK("2A2B" == createRespond(1234, 2134));
    CHECK("0A1B" == createRespond(1234, 4567));
    CHECK("0A0B" == createRespond(1234, 5678));
}
```

### Number Validator 的測試案例

```cpp
 DROGON_TEST(GAME_NumberValidatorTest) {

    CHECK(false == validateNumber(1));
    CHECK(false == validateNumber(12456));

    CHECK(false == validateNumber(5666));
    CHECK(false == validateNumber(5566));
    CHECK(false == validateNumber(5556));
    CHECK(false == validateNumber(5555));

    CHECK(true == validateNumber(1234));
    CHECK(true == validateNumber(9527));

}
```

### Answer Generator 的測試案例

```cpp
DROGON_TEST(GAME_AnswerGeneratorTest) {

    for (int i = 0; i < 100; ++i) {
        CHECK(validateNumber(createAnswer()));
    }

}
```

### 最後的組裝：awswer 與 respond

```diff
diff --git a/Models.cc b/Models.cc
index 12ad723..b12140f 100644
--- a/Models.cc
+++ b/Models.cc
@@ -13,16 +13,14 @@ Game::~Game() {
 }
 
 Game::Game() {
-    this->answer = 1234;
+    this->answer = createAnswer();
 }
 
 bool Game::guessNumber(int number) {
-    if (number == this->answer) {
-        Record *r = new Record;
-        r->guess = number;
-        r->respond = "4A0B";
-        history.push_back(r);
-    }
+    Record *r = new Record;
+    r->guess = number;
+    r->respond = createRespond(this->answer, number);
+    history.push_back(r);
     return false;
 }
 
diff --git a/test/test_main.cc b/test/test_main.cc
index 174343e..01dcc04 100644
--- a/test/test_main.cc
+++ b/test/test_main.cc
@@ -111,6 +111,12 @@ DROGON_TEST(GameAPITest) {
                                     {"number",  1234}};
 
         req->setBody(guessNumberData.dump());
+
+        // arrange the final answer
+        Game *game = gameRepository.findGameById(gameId);
+        game->answer = 1234;
+
+
         client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr &resp) {
             REQUIRE(res == ReqResult::Ok);
             REQUIRE(resp != nullptr);
@@ -119,7 +125,6 @@ DROGON_TEST(GameAPITest) {
             CHECK(resp->contentType() == CT_APPLICATION_JSON);
 
             auto result = json::parse(resp->getBody());
-            auto game = gameRepository.findGameById(result["game_id"]);
             CHECK(result["history"].size() == 1);
 
             auto expected = json{{"guess",   1234},
```

最後，只要把 hardcode 的部分，換成真實的 Function 就完工囉！另外，一個小技巧是偷偷用 Repository 改掉 `answer` 讓測試符合我們期望的情境。