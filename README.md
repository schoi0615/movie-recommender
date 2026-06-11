# 영화 추천 시스템 (Movie Recommender) — M4

C++ Programming 최종 과제  
숭실대학교 컴퓨터학부
20251842 최건

---

## 프로젝트 개요

영화 추천 시스템.
M3까지 구현한 추천 엔진 위에 장르 필터, 통계 기능, 스트레스 처방전 추천을
M4 확장 기능으로 추가했습니다.

---

## 실행 방법

```bash
./movie_recommender
```

반드시 `data/` 디렉토리가 같은 위치에 있어야 합니다.  
프로그램 시작 시 `data/movies.csv`, `data/users.csv`, `data/ratings.csv`를 자동 로드하고,
종료(0) 시 자동 저장합니다.

---

## 주요 기능

| 번호 | 기능 | 설명 |
|------|------|------|
| 1 | 영화 추가 | ID 중복 검사 포함 |
| 2 | 제목 검색 | 부분 일치 검색 |
| 3 | 전체 목록 출력 | 입력 순서 유지 |
| 4 | 평점순 출력 | 원본 배열 변경 없이 정렬 복사본 출력 |
| 5 | 사용자 추가 | ID 중복 검사 포함 |
| 7 | 평점 입력 | 1~5점 범위 검증 |
| 9 | 맞춤 추천 | 협업 필터링 (K-이웃, Top-N) |

---

## M4 확장 기능

### 10. 장르 필터
원하는 장르의 영화만 필터링해 출력합니다.  
내부 구현: `std::copy_if` + 람다, `std::set`으로 사용 가능 장르 자동 수집.

### 11. 통계 서브메뉴
서브메뉴 방식(while + try-catch)으로 세 가지 통계를 제공합니다.

- 전체 평균 평점: `std::accumulate` + 람다
- 장르별 평균 평점: `std::map` + C++17 구조적 바인딩 `auto& [k, v]`
- 평점 TOP 10: 복사본 정렬 후 상위 N개 슬라이싱

### 12. 스트레스 처방전 추천
사용자의 감정 상태(Affective Computing)를 고려한 감성 기반 추천 시스템입니다.
스트레스 지수(0~100%)를 입력받아 구간별로 다른 장르를 처방합니다.

| 구간 | 상태 | 추천 장르 |
|------|------|-----------|
| 0~40% | 두뇌 가동 가능 | Mystery / SF / Thriller / Drama |
| 41~80% | 힐링 필요 | Comedy / Family / Animation / Romance |
| 81~100% | 폭발 직전 | Action / Adventure / Horror |

---

## 클래스 구조

```
BaseManager (추상 클래스)
├── MovieManager   — 영화 데이터 + 필터/통계 (M4 확장)
├── UserManager    — 사용자 데이터
└── RatingManager  — 평점 데이터

Recommender          — 협업 필터링 추천 알고리즘
SimilarityCalculator — 유사도 계산 (Recommender에서 위임)
StressRecommender    — 스트레스 기반 감성 추천 (M4 추가)

데이터 클래스: Movie / User / Rating
```

---

## 유사도 계산 공식

두 사용자 A, B의 유사도:

```
similarity(A, B) = (공통 평점 영화 수 × 10) − (평점 차이 합계)
공통 영화 없으면 → NO_COMMON_RATING(-100) 반환
```

공통 영화가 많고 평점 취향이 비슷할수록 높은 점수를 받습니다.

---

## STL 주요 활용 목록

| STL | 사용 위치 |
|-----|-----------|
| `std::vector` | 영화/사용자/평점 컨테이너 |
| `std::map` | 장르별 통계, 영화 점수 집계 |
| `std::set` | 내가 본 영화 집합, 장르 중복 제거 |
| `std::sort` | 유사도/평점 정렬 |
| `std::copy_if` | 장르 필터링 |
| `std::accumulate` | 평균 평점 계산 |
| `std::back_inserter` | copy_if 결과 삽입 |

---
