## コードに大規模な変更を加える際の手順

1. `stable`ブランチから`develop`ブランチを作成する。
2. `develop`ブランチで作業を行う。
3. 作業が完了したら、`develop`ブランチを`stable`ブランチにマージする。
4. `develop`ブランチは削除する。

### stableブランチ

- 安定したコードのみ含まれるブランチである。
- `develop`ブランチが存在する場合、このブランチで直接コミットすることはなく、マージのみ行う。
- 十分に動作確認された変更のみがこのブランチにマージされる。

### developブランチ

- 新しい機能の開発や既存の機能の修正をするためのブランチである。
- このブランチでの変更は、動作確認がされるまで`stable`ブランチにはマージされない。
