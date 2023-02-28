# 後期施工記錄

「後期」就是在完成第一版之後的事情了，
想回以那版的視角看專案的朋友，可以用 [v0.2 tag](https://github.com/qtysdk/gaas_cpp/tree/v0.2) 查看。

## v0.3

* 邁向 `Modern C++` 以 smart pointer 取代直接使用 `new` https://github.com/qtysdk/gaas_cpp/pull/1
* 設定 GitHub Action 讓新的 PR 生出來時，都跑過測試案例 https://github.com/qtysdk/gaas_cpp/pull/2
* 整理成「會尖叫的架構」就是看目錄跟檔案上的名稱，就能略知一二這些為什麼而設計的 https://github.com/qtysdk/gaas_cpp/pull/3
* 製作簡單的 Console App 展示 CA 架構下，只要換掉最外層的內核是可以重用的 https://github.com/qtysdk/gaas_cpp/pull/8
* 替換多餘的 Json Library，直接使用 Drogon Framework 相依的 jsoncpp https://github.com/qtysdk/gaas_cpp/pull/9
* 採用 google-test 進行測試，更方便 IDE 整合 https://github.com/qtysdk/gaas_cpp/pull/11
* 利用 Python Runtime 帶起 MongoDB 的 test-container https://github.com/qtysdk/gaas_cpp/pull/12
* 提供 MongoDB 版本的 GameRepository (雖然沒實裝，但測試案例驗證了可行性) https://github.com/qtysdk/gaas_cpp/pull/14

## v0.4

(ongoing) https://github.com/qtysdk/gaas_cpp/issues/15
