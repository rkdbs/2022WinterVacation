#include <stdio.h>
#include <mysql/mysql.h> // libmysql:x64-windows
#include "db_config.h" // mysql 계정정보

int main(void) {
	MYSQL mysql;
	mysql_init(&mysql);
	// mysql 연결시도, 실패시 NULL 반환
	MYSQL* connection = mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

	if (connection == NULL)
		printf("DB 연결 실패");
	else
		printf("DB 연결 성공");

	mysql_close(&mysql); // mysql 종료
}