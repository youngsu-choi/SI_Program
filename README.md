# SI_Program 
학생 정보를 관리하는 프로그램입니다. 이름, 학번, 과목, 점수를 입력, 삭제가 가능하고 이름순, 평균순, 학번순으로 정렬한 결과를 출력할 수 있습니다.
대용량의 레코드를 관리할 목적으로 설계되었지만, 미구현되어 파일 한 개 분량의 학생을 관리할 수 있습니다.


## 과제 설명
+ 문제 설명
  + 성적 관리 프로그램
  + 파일을 읽어서, 기본 자료 생성
  + 수백 기가 바이트 이상의 레코드를 관리할 것을 고려하여 설계
  + 데이터 베이스 혹은 기타 오픈 소스 저장소를 활용할 수 없음


+ 기능적 요구 사항
  + 사용자가 새로운 자료 입력/삭제 가능
  + 입력 데이터는 이름, 학번, 과목, 점수. 추가적인 정보는 임의로 정의 가능
  + 이름순, 평균순, 학번순으로 정렬한 결과 출력 가능


+ 개발 환경
  + C 언어를 이용해서 표준 입출력 및 파일 관련 라이브러리 외에는 직접 구현 할 것
    + Gcc를 이용한 빌드 환경 이용
    + Github를 이용하여 소스코드 형상 관리
    + Makefile or CMake 를 이용한 build configuration 구성


## 개발 환경
 + C Language


## 파일 설명
+ `main.c` main source code
+ `student.h` 학생 정보 헤더 파일
+ `average.h` 평균 관리 헤더 파일
+ `nameSortStudent` 이름순으로 정렬 파일
+ `idSortStudent` 학번순으로 정렬 파일
+ `avgSortStudent` 평균순으로 정렬 파일


## 사용 방법

+ 실행 파일 이름 : app.out
```
$ make
$ ./app.out
```

+ 입출력 파일 설정
```c
#define NAME_SORT_FILE "nameSortStudent"
#define ID_SORT_FILE "idSortStudent"
#define AVG_SORT_FILE "avgSortStudent"
```
  + 이름, 학번, 평균순으로 정렬하여 저장할 파일명을 설정

번호 입력으로 메뉴 선택
+ 학생 등록 메뉴
  + 이름, 학번, 과목, 성적 순으로 입력
  + 이름순, 학번순, 평균순으로 정렬되어 파일의 입력

+ 삭제 메뉴
  + 학번, 과목 순으로 입력
  + 학번, 과목이 일치하는 학생의 과목과 성적 삭제

+ 정렬 메뉴
  + 이름순, 학번순, 평균순으로 정렬된 데이터를 출력

1. 학생 등록 메뉴에서 데이터를 삽입 할 때 정렬하여 삽입되도록 설계되어 있다.
2. 학생 등록, 삭제 행동을 할 때 3개의 파일에서 각각 데이터를 재정렬한다.
3. 평균 데이터의 정렬은 추가적인 과정이 필요해 별도의 함수를 사용한다.


## 주요 코드

> 정렬에는 두 가지 알고리즘을 사용했다.
>
> 첫 번째는 삽입할 데이터의 위치(index)를 저장하고 있을 경우의 알고리즘
> ```
> void Sort(FILE *fp, Student rec, long index){                   // 이름순, 학번순 정렬 함수
>        Student rec_temp;
>
>       for(int i = data_number; i >= index; i--){              // 파일의 데이터를 파일 끝 방향으로 index 까지 한 칸씩 밀고 저장
>              fseek(fp, (i - 1) * sizeof(rec_temp), SEEK_SET);
>              fread(&rec_temp, sizeof(rec_temp), 1, fp);
>              fseek(fp, i * sizeof(rec_temp), SEEK_SET);
>              fwrite(&rec_temp, sizeof(rec_temp), 1, fp);
>        }
>
>        fseek(fp, index * sizeof(rec), SEEK_SET);               // index번호에서 데이터 삽입
>        fwrite(&rec, sizeof(rec), 1, fp);
> }
> ```
> 대용량의 데이터를 하나의 파일에서 관리하는 경우 해당 알고리즘은 파일의 시작부터 삽입위치까지 데이터를 하나씩 검사하며 이동하기 때문에 만약 삽입 위치가 파일의 끝부분에 있다면 적합하지 않은 알고리즘이다.
> 
> 두 번째는 삽입할 데이터와 주변 데이터의 값을 비교하며 삽입할 위치로 향하는 알고리즘
> ```
> 
> ```
> 데이터를 삽입할 때 정렬을 완료하는 프로그램 특성상 삽입할 데이터 위치(index) 확인을 위해 파일을 두 번 검사하는 노력을 덜 수 있다.
