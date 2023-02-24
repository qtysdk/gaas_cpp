# 感受世界的痛楚吧！也試著用 C++ 寫看看好了。

## 為什麼有這個念頭呢？

看著遊戲微服務計劃的各組在努力著，有的組很快速地完成前期的設計並進入了 ATDD 實戰開發階段，有些組則進度較為遲一些。其中，較吸引眾人眼球的專案之一，莫過於決定使用 C++ 開發的組別了。秉持著唯有實際體驗才能理解對方的難處，在心中萌起了是不是自己也用著相同的 Tech Stack 也寫看看的想法！

在動手實作之前，先想一下目標是什麼？是要寫一樣的遊戲嗎？其實不是，重點應該不是遊戲，因為遊戲主要是一個有豐富 Domain 主題，讓參與計劃的人可以聚焦，用它來做 Practice Stack 部分的學習與箇中流程的體驗。所以，採用 C++ 來寫看看想要體驗的應該是 Tech Stack 搭建的難度以及實戰 ATDD 流程的便利性評估。

Tech Stack 部分，會參考 [骰子街 C++ 組](https://github.com/Game-as-a-Service/Machi-Koro-Cpp) 的 Tech Stack：

- C++
- [Framework: Drogon](https://github.com/drogonframework/drogon)
- DB: MongoDB
- 前端：Vue

-----

## 各小節摘要

### 開發環境準備工作

* 傳送門 👉 [先由搭建環境開始](0-development-setup.md) ✨✨✨

有了動機後，我們要怎麼讓事情發生呢？先不要想太多深刻的計劃，先讓身體動起來再說。把開發環境弄好，該裝的語言工具，該安裝的函式庫都裝起來吧！在這小節，會走過編譯 Drogon Framework 到建立出第一個 `NothingCtrl` 的流程。

### Walking Skeleton

* 傳送門 👉 [開始研究 ATDD 基礎建設部分](1-walking-skeleton.md) ✨✨✨

當基礎的開發工具備妥後，我們得做一些準備工作，讓自己的團隊站朝著「舒適日常開發環境」前向，那就是 Walking Skeleton。利用這個走路的骨架，來確認一下由應用程式的最外層可以打通到內裡，接上測試的端點 (end-to-end)。

在這裡，得研習如何讓 Drogon Framework 的測試打通：`HTTP Request` 👉 `HTTP Response` 是通透，且可以驗證結果的 👍

### 遊戲微服務計劃 Practice Stack

* 傳送門 👉 [讓我們選個簡單的遊戲實戰吧！](2-guess-number-game.md) ✨✨✨

在前一節我們用 ATDD 貫通了開發所需的基本練習，這能讓你開始有勇氣用目前的 Tech Stack 來開發「具體的題目」。在這裡，我選擇一個極為迷你的「猜數字」遊戲，因為主要還是透過實戰來體驗，像我這樣不熟悉 `C++` 的人，生硬地試著用它寫寫看會是什麼樣的感受。

同時，我們也透過這一小節快速複習在[《遊戲微服務計劃》](https://www.facebook.com/groups/wsa.tw/posts/604202378070147/)學習到軟體工程方法論相關的 Practice Stack：

* Event Storming
* OOA/D

### 第一隻 Controller (API)

* 傳送門 👉 [實作開始遊戲](3-start-to-implment.md) ✨✨✨

先前的實作是為了熟悉 Drogon Framework 為目的，接著要做的是遊戲開局的 `CreateGameCtrl` 這個 Controller，它接受 `HTTP Post`：

```
/guess_number_game:start
```

而 Request Body 會是這樣的型式：

```json
{"player_name": "56不能亡"}
```

得到的 Response Body 為：
```json
{"game_id": "abcd", "player_name": "56不能亡", "history": []}
```

我們就只做到刻出 Controller 即可，後續相依的類別先放在心上就好。

### 以終為始 ATDD

* 傳送門 👉 [CreateGame UseCase 開局](4-atdd.md) ✨✨✨

在上一小節，我們最後整理出了後續的 **TODOs** 部分。

> 做完了「殻」之後呢？就可以來寫跟遊戲比較相關的部分了。若是以 Clean Architecture 為偏好的話，我們接著得做：

- Controller → UseCase
- UseCase → Repository
- Repository → Domain Model

因此，這節的內容會開始實作玩家操作流程，會使用到的第 1 個 Command (Use Case)，那就是 CreateGame。並且克制自己，先不要實作到 Repository 之後的部分。即是小步快跑，卻又逐夢踏實。跨太大步常讓失敗的風險提昇。

這個限制在開發初期最好能放在心上，因為有太多相關的類別沒有開出來，一次開太多會讓人迷失在複雜的關係中，就依測試會測到的再開就好。這樣就不會有「預先開了很多類別，但其實測試時發現好像用不上」的酒駕情況。

✨ ATDD 會指引你光明的未來 ✨

### Aggregate Root 的守護者

* 傳送門 👉 [Repository 與 Aggregate Root](5-repo-agg.md) ✨✨✨

當我們打點好了 Use Case 地基主後，可以來想像一下美好的未來：

```cpp
void CreateGameUseCase::execute(Input input, Output &output) {
    // 用 repo 查改存推
    auto game = gameRepository.create(input.playerName);
    
    output.gameId = game.id;
    output.playerName = game.playerName;
}
```

我們後弄個 `gameRepository` 讓它替我們管理 `Game` Domain Model 的一生！一旦，這條路打通後，其他的 Use Case 實作就會很容易了。

### 我的猜數字，終於可以猜數字

* 傳送門 👉 [GuessNumber Use Case](6-guess-game.md) ✨✨✨

當你以為一帆風順時，下一秒就可能翻船了。本來以為實作第 2 個 Use Case 應該也是簡單的任務，但遇上了此專案最大的逆流。由於對 `C++` 特性不熟的原因，而得回頭修改物件的傳遞方式。

>> 我等了一個晚上，就是在等這種大場面啊 from 破壞之王

>> 我要看到血流成河啊 from PTT鄉民

另外，得提一下到這實作完成時，我們仍是使用 **hardcode** 的謎底，先讓整個實作流程串起來，再來用 TDD 實作細緻的邏輯部分。

### 在 TDD 保護下實作程式邏輯

* 傳送門 👉 [這是我最後的波紋：TDD](7-tdd.md) ✨✨✨

TDD 是許多開發者耳熟能詳的《探索設計的策略》，也是許多人認識實作單元測試的起點。在 ATDD 以「實際使用者的視角」開闢完主要幹道後，緊接著就是上刺刀，跟著最小的邏輯單元進行肉摶戰的時刻了！

## 打完收工

* 沒有傳送門，就是這裡了。 ✨✨✨

這裡的「打完」並不是完工，而是完成用 `C++` 類的 Tech Stack 完成了 GaaS 實作層面的體驗。我自己會挫折的部分，主要來自語言與他的生態不熟悉，其它的層面挫折不大。畢竟，在 Google 與 ChatGPT 雙排輔助的前提下，不會寫的部分就大膽地問吧！甚至，我會貼一段 Compilation 不會過的 Code，請 ChatGPT 幫我修看看。再由它給的建議，回推我自己哪邊寫錯了。

那麼談談客觀一點的阻力，那就是 Drogon Framework 真的太新了，至少它的 Test Framework 並不能被 CLion 或 VS Code 認得，只能單純依賴著 CMake Tests 的結果與 IDE 溝通。若是換成比較熱門的 gtest 應該有機會可以單獨指定要跑的 Test Case，而非每次都全部執行。

那麼比較個人化的阻力會是什麼？以開發經驗不太多的情況，也許會卡關在 Toolchain 與 Build Tool 的設定。由於曾經有過一段與 Cross Compile 奮鬥的日子，這部分我大多不太會有阻力就是了。