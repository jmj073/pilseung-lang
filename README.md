# 필?승!랭

본 프로그램은 ubuntu에서 작성되었으며, ubuntu에서 실행될 것을 가정하고 작성하였다.

## 빌드

```bash
make
```

## 실행

```
./interpreter
```

## 트러블슈팅

### locale 관련 문제 발생시

```bash
sudo apt-get install -y language-pack-ko
sudo locale-gen ko_KR.UTF-8
```
