// 이름순, 학번순, 평균순 세 가지 파일을 각각 관리한다.
// 평균순 파일 관리 함수는 별도로 있다.

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include "student.h"	// 학생정보, 평균 구조체 헤더파일
#include "average.h"

void Open_File();	// 파일 오픈
void Register();	// 이름순, 학번순 정렬 파일 관리 함수
void Delete();
int Delete_Sort();
void Sort();
void Sort_Print();
void PrintData();

void Register_Avg();	// 평균순 정렬 파일 관리 함수
void Sort_Avg();
void Delete_Avg();
void PrintData_Avg();

long data_number;	// 전체 데이터 수
long students_number;	// 학생 수

int main(){
	FILE *fp;
	Student rec;
	Average avg;

	data_number = 0L;
	students_number = 0L;

	if((fp = fopen("nameSortStudent", "r")) == NULL){
		fprintf(stderr, "Error\n");
		return 0;
	}else{
		while(fread(&rec, sizeof(rec), 1, fp) > 0 ){	// 현재 데이터 수 저장
			data_number += 1L;
		}
	}
	fclose(fp);
	
	if((fp = fopen("avgSortStudent", "r")) == NULL){
		fprintf(stderr, "Error\n");
		return 0;
	}else{
		while(fread(&avg, sizeof(avg), 1, fp) > 0 ){	// 현재 학생 수 저장
			students_number += 1L;
		}
	}
	fclose(fp);

	int choice;
	
	while(1){	
		printf("---Menu---\n");		// 전체 메뉴 출력
		printf("1. Register\n");
		printf("2. Delete\n");
		printf("3. Sort\n");
		printf("4. Exit\n");
		printf("Select : ");

		scanf("%d", &choice);
		
		switch(choice){
			case 1:
				Register(fp);
				continue;
			case 2:
				Delete(fp);
				continue;
			case 3:
				Sort_Print(fp);
				continue;
			case 4:
				break;
			default:
				printf("Error!");
				continue;
		}
		break;
	}
	return 0;
}

void Open_File(FILE *fp, char str[]){	// 파일 오픈
	if((fp = fopen(str, "rb+")) == NULL){
	        fprintf(stderr, "Error\n");
	}
}

void Register(FILE *fp){ // 성적 추가
	printf("---Register---\n");
	printf("Name, Id, Subject, Score : ");

	Student rec;
	Student rec_temp;
	Average avg;

	long index = 0L;	// 데이터를 삽입 위치 저장
	
	while(scanf("%s %d %s %d", rec.name, &rec.id, rec.subject, &rec.score) == 4){
		Open_File(fp, "nameSortStudent"); // 이름순 정렬 파일 오픈		
		while(fread(&rec_temp, sizeof(rec_temp), 1, fp) > 0){
			if(strcmp(rec.name, rec_temp.name) < 0){ // 새로운 데이터 삽입 위치를 정하기 위한 조건문
				break;
			}
			index++;
		}
		Sort(fp, &rec, index);
		fclose(fp);
				
		index = 0;
		Open_File(fp, "idSortStudent"); // 학번순 정렬 파일 오픈
		while(fread(&rec_temp, sizeof(rec_temp), 1, fp) > 0){
			if(rec.id < rec_temp.id) break; // 학번 내림차순 정렬 삽입 위치를 정하는 조건문
			index++;
		}
		Sort(fp, &rec, index);
		fclose(fp);
		data_number++;
		
		Open_File(fp, "avgSortStudent"); // 평균순 정렬 파일 오픈
		Register_Avg(fp, &rec, &avg);
		fclose(fp);
		break;
	}
}

void Sort(FILE *fp, Student rec, long index){ // 이름순, 학번순 정렬 함수
	Student rec_temp;

	for(int i = data_number; i >= index; i--){ 		 // 저장된 데이터를 index번호까지 뒤로 한 칸씩 밀고 저장
		fseek(fp, (i - 1) * sizeof(rec_temp), SEEK_SET);
		fread(&rec_temp, sizeof(rec_temp), 1, fp);
		fseek(fp, i * sizeof(rec_temp), SEEK_SET);
		fwrite(&rec_temp, sizeof(rec_temp), 1, fp);
	}

	fseek(fp, index * sizeof(rec), SEEK_SET);		// index번호에서 데이터 삽입
	fwrite(&rec, sizeof(rec), 1, fp);
}

void Register_Avg(FILE *fp, Student rec, Average avg){ // 평균순 파일 데이터 삽입 함수
	Average avg_temp;
	int index = 0;
	double current;
	
	while(fread(&avg, sizeof(avg), 1, fp) > 0){ // 신규가 아닌 삽입의 경우
		if(avg.id == rec.id) {
			current = avg.avg;	// 변경 전 평균을 저장

			avg.count++;		// 신규데이터로 평균을 다시 계산
			avg.total += rec.score;
			avg.avg = (double)avg.total / avg.count;

			fseek(fp, index * sizeof(avg), SEEK_SET);
			fwrite(&avg, sizeof(avg), 1, fp);

			if(current < avg.avg){	// 변경 후 평균과 비교하여 정렬에 용이하게 이용
				Sort_Avg(fp, &avg, index, 1);	// 변경 후의 평균 값이 오르면 1, 내리면 0 인자 전달
			}else if(current > avg.avg){
				Sort_Avg(fp, &avg, index, 0);
			}
			return;
		}
		index++;
	}
		
	strcpy(avg.name, rec.name);	// 신규 삽입의 경우
	avg.id = rec.id;		// 신규 데이터 추가하고 평균 계산
	avg.total = rec.score;
	avg.count = 1;
	avg.avg = (double)avg.total;

	fseek(fp, index * sizeof(avg), SEEK_SET); // 현재 학생 중 마지막 데이터로 저장
	fwrite(&avg, sizeof(avg), 1, fp);
	students_number++;			  // 총 학생 수 증가

	if(index == 0) return;		// 첫 번째 등록된 학생은 정렬 없이 종료
	
	int i = 1;
	while(1){
		fseek(fp, (index - i) * sizeof(avg), SEEK_SET);		// 파일의 끝 데이터부터 뒤로 한 칸씩 밀고
		fread(&avg_temp, sizeof(avg_temp), 1, fp);		// 삽입 데이터의 위치를 잡는다.
		fseek(fp, (index - i + 1) * sizeof(avg), SEEK_SET);
		if(avg.avg > avg_temp.avg){
			fwrite(&avg_temp, sizeof(avg_temp), 1, fp);
			i++;
		}else break;
	
		if(index - i < 0){ // 파일의 첫 데이터 index에 도착하면 첫 주소를 저장하고 반복문을 빠져나감
			fseek(fp, 0, SEEK_SET);
			break;
		}
	}
	fwrite(&avg, sizeof(avg), 1, fp);	// 반복문을 나와서 삽입위치에 데이터를 삽입한다.
}

void Sort_Avg(FILE *fp, Average avg, int index, int stat){ // 평균순 파일의 데이터 정렬 함수
	Average avg_temp;

	int i;
	
	if(stat == 1){	// 변경 전 평균보다 변경 후 평균이 높으면 1을 인자로 받는다.
		i = 1;
		while(1){
			fseek(fp, (index - i) * sizeof(avg), SEEK_SET); // 1을 인자로 받았을 때 현재 위치에서
			fread(&avg_temp, sizeof(avg_temp), 1, fp);	// 파일 시작 위치쪽으로만 확인한다.
			fseek(fp, (index - i + 1) * sizeof(avg), SEEK_SET);
			if(avg.avg > avg_temp.avg){			// 앞의 데이터의 평균과 비교하며 위치를 잡는다.
				fwrite(&avg_temp, sizeof(avg_temp), 1, fp);
				i++;
			}else break;				
			
			if(index - i < 0){				// 파일의 시작 데이터를 넘어가지 않도록
				fseek(fp, 0, SEEK_SET);			// 파일 시작 위치로 이동하고 반복문 종료
				break;
			}
		}
	}else if(stat == 0){
		i = 1;
		while(1){						// 0을 인자로 받았을 때 현재 위치에서
			fseek(fp, (index + i) * sizeof(avg), SEEK_SET); // 파일 끝 방향으로만 확인한다.
			if(fread(&avg_temp, sizeof(avg_temp), 1, fp) <= 0){ // 파일의 다음 데이터가 없으므로 정렬 완료 상태
				return;
			}
			fseek(fp, (-2) * sizeof(avg), SEEK_CUR);	// 임의 위치를 뒤로 두 개 주소 분량만큼 이동
			if(avg.avg < avg_temp.avg){		
				fwrite(&avg_temp, sizeof(avg_temp), 1, fp);
				i++;
			}else break;
			
			if(index + i >= students_number){				// 파일의 끝을 넘어가지 않도록
				fseek(fp, (index + i - 1) * sizeof(avg), SEEK_SET);	// 파일 끝에서 한 칸 앞으로
				break;							// 위치를 이동하고 반복문 종료
			}
		}
	}
	fwrite(&avg, sizeof(avg), 1, fp);		// 이동한 위치에서 파일의 데이터 저장
}

void Delete(FILE *fp){		// 학번과 과목이름을 입력받고 삭제
	Student rec;
	Average avg;
	long index = 0L;
	int score;

	printf("---Delete Menu---\n");
	printf("Delete ID : ");
	
	int id;
	scanf("%d", &id);
	
	printf("Delete Subject : ");
	char sub[20];
	scanf("%s", sub);
	

	Open_File(fp, "nameSortStudent");			// 이름순 정렬 파일 오픈
	if(Delete_Sort(fp, id, "nameSortStudent", sub) == 1){	// 삭제 후 재정렬
		printf("None ID\n");				// 1을 반환하면 존재하지 않는 학번으로 종료된다.
		return;
	}
	fclose(fp);

	Open_File(fp, "idSortStudent");						// 학번순 정렬 파일 오픈
	while(fread(&rec, sizeof(rec), 1, fp) > 0){		
		if(rec.id == id)						// 평균순 정렬을 위해
			if(strcmp(rec.subject, sub) == 0) score = rec.score;	// 제거되는 과목의 점수를 저장
	}
	Delete_Sort(fp, id, "idSortStudent", sub);				// 삭제 후 재정렬
	fclose(fp);

	data_number -= 1;				// 전체 데이터 수 감소
	
	Open_File(fp, "avgSortStudent");		// 평균순 정렬 파일 오픈
	Delete_Avg(fp, id, score);			
	fclose(fp);
}

int Delete_Sort(FILE *fp, int id, char str[], char sub[]){ // 입력받은 데이터 삭제
	Student rec;
	long index = 0L;
	
	fseek(fp, 0, SEEK_SET);
	while(fread(&rec, sizeof(rec), 1, fp) > 0){ // 입력받은 학번과, 과목명 비교하여 위치를 index에 저장
		if(rec.id == id)
			if(strcmp(rec.subject, sub) == 0) break;
		index++;
	}
	
	if(index == data_number) return 1;	// 데이터의 위치가 파일 끝에 도착하면 일치하는 데이터가 없으므로 종료
	
	for(int i = 1; index + i < data_number; i++){ // index의 위치에서 파일 끝까지 데이터를 덮어쓰기
		fseek(fp, (index + i) * sizeof(rec), SEEK_SET);
		fread(&rec, sizeof(rec), 1, fp);
		fseek(fp, (index + i - 1) * sizeof(rec), SEEK_SET);
		fwrite(&rec, sizeof(rec), 1, fp);
	}
	
	truncate(str, (data_number - 1) * sizeof(rec)); // 삭제 된 데이터 크기만큼 파일 크기 줄이기
	return 0;
}

void Delete_Avg(FILE *fp, int id, int score){ // 평균순 정렬 파일에서 데이터 삭제
	Average avg;
	long index = 0L;
	double current;
	
	fseek(fp, 0, SEEK_SET);
	while(fread(&avg, sizeof(avg), 1, fp) > 0){ // 입력받은 학번과 일치하는 학번의 위치를 index에 저장
		if(avg.id == id) break;
		index++;
	}
	
	current = avg.avg;	// 변경 전 평균 저장

	avg.total -= score;	// 해당 학생의 총점에서 삭제 된 과목의 점수 빼기
	avg.count--;		// 해당 학생의 과목 수 -1

	if(avg.count <= 0){	// 해당 학생의 과목 수가 0이 되면 파일에서 학생의 정보를 삭제
		for(int i = 1; index + i < students_number; i++){
			fseek(fp, (index + i) * sizeof(avg), SEEK_SET);
			fread(&avg, sizeof(avg), 1, fp);
			fseek(fp, (index + i - 1) * sizeof(avg), SEEK_SET);
			fwrite(&avg, sizeof(avg), 1, fp);
		}	
	
		truncate("avgSortStudent", (students_number - 1) * sizeof(avg)); // 삭제한 크기만큼 파일 크기 줄이기
		students_number--;	// 전체 학생 수 감소
		return;
	}
							// 과목 수가 0이 아닐 경우
	avg.avg = (double)avg.total / avg.count;	// 해당 학생의 평균을 다시 계산 후 저장

	fseek(fp, index * sizeof(avg), SEEK_SET);
	fwrite(&avg, sizeof(avg), 1, fp);

	if(current < avg.avg){				// 평균이 올랐으면 1을 인자로, 내렸으면 0을 인자로 함수 사용
		Sort_Avg(fp, avg, index, 1);
	}else if(current > avg.avg){
		Sort_Avg(fp, avg, index, 0);
	}
}

void Sort_Print(FILE *fp){ // 이름순, 학번순, 평균순으로 정렬된 데이터 출력 메뉴
	printf("---Sort Menu---\n");
	printf("1. Name Sort\n");
	printf("2. Id Sort\n");
	printf("3. Average Sort\n");
	printf("4. Exit\n");
	
	int choice;
	scanf("%d", &choice);

	switch(choice){
		case 1:
			PrintData(fp, "nameSortStudent");
			break;
		case 2:
			PrintData(fp, "idSortStudent");
			break;
		case 3:
			PrintData_Avg(fp);
			break;
		default:
			printf("Continue");
			break;
	}
}

void PrintData(FILE *fp, char str[]){ // 이름순, 학번순 파일 출력 함수
	Student rec;
	Open_File(fp, str);

	fseek(fp, 0, SEEK_SET);
	while(fread(&rec, sizeof(rec), 1, fp) > 0){		    // 파일의 데이터 한 줄 씩 읽기
		printf("%s %d %s %d\n", rec.name, rec.id, rec.subject, rec.score);
	}
	fclose(fp);
}

void PrintData_Avg(FILE *fp){	// 평균순 파일 출력 함수
	Average avg;
	Open_File(fp, "avgSortStudent");

	fseek(fp, 0, SEEK_SET);
	while(fread(&avg, sizeof(avg), 1, fp) > 0){ 		    // 파일의 데이터가 존재하는 동안 한 줄 씩 읽기
		printf("%s %d %.2lf\n", avg.name, avg.id, avg.avg); // 읽은 데이터 출력
	}
	fclose(fp);
}
