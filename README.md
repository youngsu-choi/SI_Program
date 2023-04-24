# SI_Program 
학생 정보를 관리하는 프로그램입니다. 이름, 학번, 과목, 점수를 입력, 삭제가 가능하고 이름순, 평균순, 학번순으로 정렬한 결과를 출력할 수 있습니다.
대용량의 레코드를 관리할 목적으로 설계되었고, 현재는 파일 한 개 분량의 학생을 관리할 수 있습니다.


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
  + C 언어를 이용해서 표준 입출력 및 파일 관련 라이브러리 외에는 직접 구현
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

> + 데이터 추가과정의 정렬에는 두 가지 알고리즘을 사용했다.
>
> + 첫 번째, 추가할 데이터의 위치(index)를 저장하고 있을 경우의 알고리즘
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
>        fseek(fp, index * sizeof(rec), SEEK_SET);               // index번호에서 데이터 추가
>        fwrite(&rec, sizeof(rec), 1, fp);
> }
> ```
> 대용량의 데이터를 하나의 파일에서 관리하는 경우 해당 알고리즘은 파일의 시작부터 추가 위치까지 데이터를 하나씩 검사하고, 파일에 추가하기 위해 다시 탐색하는 두 번의 과정을 거친다.
> 
> + 두 번째, 추가할 데이터와 주변 데이터의 값을 비교하며 삽입할 위치로 향하는 알고리즘
> ```
> int i = 1;
> while(1){
>        fseek(fp, (index - i) * sizeof(avg), SEEK_SET);         // 파일의 끝 데이터부터 뒤로 한 칸씩 밀고
>        fread(&avg_temp, sizeof(avg_temp), 1, fp);              // 추가 할 데이터의 위치를 잡는다.
>        fseek(fp, (index - i + 1) * sizeof(avg), SEEK_SET);
>        if(avg.avg > avg_temp.avg){
>                fwrite(&avg_temp, sizeof(avg_temp), 1, fp);
>                i++;
>        }else break;                    // 정렬에 필요한 비교가 끝나면 반복문을 빠져나간다.
>        
>        if(index - i < 0){              // 파일의 첫 데이터 index에 도착하면 첫 주소를 저장하고 반복문을 빠져나간다.
>                fseek(fp, 0, SEEK_SET);
>                break;
>        }
> }
> fwrite(&avg, sizeof(avg), 1, fp);       // 반복문을 나와서 추가 위치에 데이터를 삽입한다.
> ```
> 반복문 안에서 새로운 데이터의 위치를 설정하고 빠져나온다.
> 마지막 줄에서 해당 위치에 입력 데이터를 쓰기 하면 정렬이 끝난다.
> 첫 번째 알고리즘과 다르게 데이터의 추가와 정렬을 동시에 수행하는 프로그램은 추가 할 데이터 위치(index) 확인을 위해 파일을 두 번 탐색하는 과정을 줄일 수 있다.


> + 데이터 삭제과정의 정렬 알고리즘이다.
> ```
> for(int i = 1; index + i < data_number; i++){ // index의 위치에서 파일 끝까지 데이터를 덮어쓰기
>               fseek(fp, (index + i) * sizeof(rec), SEEK_SET);
>               fread(&rec, sizeof(rec), 1, fp);
>               fseek(fp, (index + i - 1) * sizeof(rec), SEEK_SET);
>               fwrite(&rec, sizeof(rec), 1, fp);
> }
>
> truncate(str, (data_number - 1) * sizeof(rec)); // 삭제 된 데이터 크기만큼 파일 크기 줄이기
> ```
> 1. 삭제할 데이터의 위치를 저장(index)
> 2. index 다음 위치의 데이터를 index 위치에 덮어쓰고 파일의 끝까지 반복한다.
> 3. 삭제 된 데이터의 크기만큼 파일의 크기를 줄인다.


## 미구현 내용
+ 파일 관리 시스템
  + 대용량 파일 관리에서 검색에 용이하도록 파일을 분할해서 관리.
    + 파일 구조체를 동적로 생성하고 파일의 크기를 제한한다. 데이터가 입력됬을 때, 파일의 크기가 초과되면 새로운 파일을 생성.
    + 파일 구조체 항목은 파일의 이름, 시작 데이터, 끝 데이터로 설정.
  + 데이터 검색, 삭제
    + 파일 구조체 배열의 항목과 비교.
    + 시작 데이터와 끝 데이터 항목과 비교하고 포함되지 않는 값이면 다음 배열과 비교.
  + 데이터 추가
    + 데이터가 추가 될 때, 파일의 제한 크기를 넘어가면 끝 데이터를 다음 배열의 시작 데이터로 추가.
    + 파일 구조체 배열을 수정하고, 파일의 데이터도 수정.
    
+ 모듈의 재사용성 검토
